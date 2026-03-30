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
  vao_init(&renderer->vao);

  float vertices[] = {
    -0.5f, 0.5f,
    -0.5f, -0.5f,
    0.5f, -0.5f,
    0.5f, 0.5f,
  };

  float textureCoords[] = {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
    1.0f, 1.0f,
  };

  GLuint indices[] = {
    0, 1, 3,
    3, 1, 2    
  };

  ebo_init(&renderer->ebo, &indices[0], sizeof(indices));
  vao_setupEBO(&renderer->vao, &renderer->ebo);
  
  VBO vboVertices;
  vbo_init(&vboVertices, &vertices[0], sizeof(vertices));
  vao_insertVBO(&renderer->vao, &vboVertices, 2);

  VBO vboTexCoords;
  vbo_init(&vboTexCoords, &textureCoords[0], sizeof(textureCoords));
  vao_insertVBO(&renderer->vao, &vboTexCoords, 2);

  vao_unbind();
  vbo_cleanup(&vboVertices);
  vbo_cleanup(&vboTexCoords);

  shader_start(&renderer->shader);
  // Uniform Locations
  {
    renderer->location_projectionMatrix = shader_get_uniformLocation(&renderer->shader, "uProjectionMatrix");
    renderer->location_transformationMatrix = shader_get_uniformLocation(&renderer->shader, "uTransformationMatrix");
    renderer->location_spriteSize = shader_get_uniformLocation(&renderer->shader, "uSpriteSize");
    renderer->location_atlasOffset = shader_get_uniformLocation(&renderer->shader, "uAtlasOffset");
  }

  // Load Unform Locations
  {
    Mat4 projectionMatrix = orthographic_project(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    shader_loadMat4(renderer->location_projectionMatrix, projectionMatrix);
  }
  shader_stop();
}

void renderer2D_drawSpritePro(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float rot, float scale)
{
  Sprite sprite = SPRITES[spriteID];
  
  shader_start(&renderer->shader);
  vao_bind(&renderer->vao);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(&renderer->textureAtlas);

  Mat4 transformationMatrix = transformation_matrix(pos, rot, (Vec2){sprite.size.x * scale, sprite.size.y * scale});
  shader_loadMat4(renderer->location_transformationMatrix, transformationMatrix);

  shader_loadVec2i(renderer->location_spriteSize, sprite.size);
  shader_loadVec2i(renderer->location_atlasOffset, sprite.atlasOffset);

  glDrawElements(GL_TRIANGLES, renderer->vao.vertexCount, GL_UNSIGNED_INT, NULL);
  
  texture_unbind();

  vao_unbind();
  shader_stop();
}

void renderer2D_drawSprite(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale)
{
  renderer2D_drawSpritePro(renderer, spriteID, pos, 0, scale);
}

void renderer2D_cleanup(Renderer2D* renderer)
{
  ebo_cleanup(&renderer->ebo);
  vao_cleanup(&renderer->vao);
  texture_cleanup(&renderer->textureAtlas);
  shader_cleanup(&renderer->shader);
}
