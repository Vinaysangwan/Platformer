#include "pch.h"
#include "renderer.h"
#include "utils.h"
#include "config.h"

void renderer_draw(const Shader* shader, const VAO* vao)
{
  shader_start(shader);
  vao_bind(vao);

  glDrawElements(GL_TRIANGLES, vao->vertexCount, GL_UNSIGNED_INT, NULL);

  vao_unbind();
  shader_stop();
}

void renderer_drawTexture(const Shader* shader, const VAO* vao, const Texture *texture)
{ 
  shader_start(shader);
  vao_bind(vao);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(texture);

  glDrawElements(GL_TRIANGLES, vao->vertexCount, GL_UNSIGNED_INT, NULL);

  texture_unbind();
  vao_unbind();
  shader_stop();
}

// #############################################################################
//                           Renderer2D
// #############################################################################
void renderer2D_init(Renderer2D* renderer, const char* vertFilePath, const char* fragFilePath, const char* textureAtlasPath)
{
  shader_init(&renderer->shader, vertFilePath, fragFilePath);
  texture_init(&renderer->textureAtlas, textureAtlasPath);
  renderer->quadCount = 0;

  GLuint indices[MAX_QUADS * IDX_PER_QUAD];
  for (int i = 0; i < MAX_QUADS; i++)
  {
    int idx = i * IDX_PER_QUAD;
    int v = i * VERT_PER_QUAD;

    indices[idx + 0] = v + 0;
    indices[idx + 1] = v + 1;
    indices[idx + 2] = v + 3;
    indices[idx + 3] = v + 3;
    indices[idx + 4] = v + 1;
    indices[idx + 5] = v + 2;
  }

  vao_init(&renderer->vao);

  ebo_init(&renderer->ebo, &indices[0], sizeof(indices));
  vao_setupEBO(&renderer->vao, &renderer->ebo);
  
  vbo_initDynamic(&renderer->vboPos, NULL, SIZE_VBO_POS * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboPos, FLOATS_PER_POS);

  vbo_initDynamic(&renderer->vboUV, NULL, SIZE_VBO_UV * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboUV, FLOATS_PER_UV);

  vao_unbind();

  shader_start(&renderer->shader);
  // Uniform Locations
  {
    renderer->location_projectionMatrix = shader_get_uniformLocation(&renderer->shader, "uProjectionMatrix");
    renderer->location_viewMatrix = shader_get_uniformLocation(&renderer->shader, "uViewMatrix");
  }

  // Load Unform Locations
  {
    Mat4 projectionMatrix = orthographic_project(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    shader_loadMat4(renderer->location_projectionMatrix, projectionMatrix);
  }
  shader_stop();
}

void renderer2D_beginCamera(Renderer2D *renderer, Camera2D *cam)
{
  renderer->quadCount = 0;
  shader_start(&renderer->shader);
  vao_bind(&renderer->vao);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(&renderer->textureAtlas);
 
  Mat4 viewMatrix = view_matrix(cam->pos, cam->rot, cam->zoom, WINDOW_WIDTH, WINDOW_HEIGHT);
  shader_loadMat4(renderer->location_viewMatrix, viewMatrix);
}

void renderer2D_begin(Renderer2D* renderer)
{
  renderer->quadCount = 0;
  shader_start(&renderer->shader);
  vao_bind(&renderer->vao);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(&renderer->textureAtlas);

  Mat4 viewMatrix = view_matrix((Vec2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 0, 1.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
  shader_loadMat4(renderer->location_viewMatrix, viewMatrix);
}

void renderer2D_flush(Renderer2D* renderer)
{
  if (renderer->quadCount == 0) return;

  vbo_upload_subData(
    &renderer->vboPos, 
    0, 
    renderer->quadCount * VERT_PER_QUAD * FLOATS_PER_POS * sizeof(float), 
    &renderer->posBuffer[0]
  );

  vbo_upload_subData(
    &renderer->vboUV, 
    0, 
    renderer->quadCount * VERT_PER_QUAD * FLOATS_PER_UV * sizeof(float), 
    &renderer->uvBuffer[0]
  );

  glDrawElements(GL_TRIANGLES, renderer->vao.vertexCount, GL_UNSIGNED_INT, NULL);
}

void renderer2D_end(Renderer2D* renderer)
{
  renderer2D_flush(renderer);
  vao_unbind();
  
  texture_unbind();
  shader_stop();
}

void renderer2D_cleanup(Renderer2D* renderer)
{
  vbo_cleanup(&renderer->vboPos);
  vbo_cleanup(&renderer->vboUV);
  ebo_cleanup(&renderer->ebo);
  vao_cleanup(&renderer->vao);
  texture_cleanup(&renderer->textureAtlas);
  shader_cleanup(&renderer->shader);
}

void renderer2D_drawSpritePro(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale, float rot)
{
  if(renderer->quadCount >= MAX_QUADS)  return;
  
  Sprite sprite = SPRITES[spriteID];
  float hw = sprite.size.x * scale * 0.5f;
  float hh = sprite.size.y * scale * 0.5f;

  float lx[4] = {-hw, -hw, hw, hw};
  float ly[4] = {hh, -hh, -hh, hh};

  float rad = DEG_2_RAD * rot;
  float cosV = cosf(rad);
  float sinV = sinf(rad);
  int posIDX = renderer->quadCount * VERT_PER_QUAD * FLOATS_PER_POS;
  for (int i=0; i < 4; i++)
  {
    renderer->posBuffer[posIDX + i * 2 + 0] = pos.x + lx[i] * cosV - ly[i] * sinV;
    renderer->posBuffer[posIDX + i * 2 + 1] = pos.y + lx[i] * sinV + ly[i] * cosV;
  }

  float x0 = sprite.atlasOffset.x;
  float x1 = sprite.atlasOffset.x + sprite.size.x;
  float y0 = sprite.atlasOffset.y;
  float y1 = sprite.atlasOffset.y + sprite.size.y;

  int uvIDX = renderer->quadCount * VERT_PER_QUAD * FLOATS_PER_UV;
  renderer->uvBuffer[uvIDX + 0] = x0; renderer->uvBuffer[uvIDX + 1] = y1;
  renderer->uvBuffer[uvIDX + 2] = x0; renderer->uvBuffer[uvIDX + 3] = y0;
  renderer->uvBuffer[uvIDX + 4] = x1; renderer->uvBuffer[uvIDX + 5] = y0;
  renderer->uvBuffer[uvIDX + 6] = x1; renderer->uvBuffer[uvIDX + 7] = y1;

  renderer->quadCount++;
}

void renderer2D_drawSprite(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale)
{
  renderer2D_drawSpritePro(renderer, spriteID, pos, scale, 0);
}
