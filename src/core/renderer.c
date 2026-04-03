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

  vao_init(&renderer->vao);

  // inPos
  vbo_initDynamic(&renderer->vboPos, NULL, SIZE_VBO_POS * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboPos, FLOATS_PER_POS);

  // inSize
  vbo_initDynamic(&renderer->vboSize, NULL, SIZE_VBO_POS * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboSize, FLOATS_PER_POS);

  // inAtlasOffset
  vbo_initDynamic(&renderer->vboAtlasOffset, NULL, SIZE_VBO_POS * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboAtlasOffset, FLOATS_PER_POS);

  // inSpriteSize
  vbo_initDynamic(&renderer->vboSpriteSize, NULL, SIZE_VBO_POS * sizeof(float));
  vao_insertVBO(&renderer->vao, &renderer->vboSpriteSize, FLOATS_PER_POS);

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

  glActiveTexture(GL_TEXTURE0);
  texture_bind(&renderer->textureAtlas);
}

void renderer2D_beginCamera(Renderer2D *renderer, Camera2D *cam)
{
  renderer->quadCount = 0;
  shader_start(&renderer->shader);
  vao_bind(&renderer->vao);
 
  Mat4 viewMatrix = view_matrix(cam->pos, cam->rot, cam->zoom, WINDOW_WIDTH, WINDOW_HEIGHT);
  shader_loadMat4(renderer->location_viewMatrix, viewMatrix);
}

void renderer2D_begin(Renderer2D* renderer)
{
  renderer->quadCount = 0;
  shader_start(&renderer->shader);
  vao_bind(&renderer->vao);

  Mat4 viewMatrix = view_matrix((Vec2){WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2}, 0, 1.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
  shader_loadMat4(renderer->location_viewMatrix, viewMatrix);
}

void renderer2D_flush(Renderer2D* renderer)
{
  if (renderer->quadCount == 0) return;

  // inPos
  vbo_upload_subData(
    &renderer->vboPos, 
    0, 
    renderer->quadCount * FLOATS_PER_POS * sizeof(float), 
    &renderer->posBuffer[0]
  );

  // inSize
  vbo_upload_subData(
    &renderer->vboSize, 
    0, 
    renderer->quadCount * FLOATS_PER_POS * sizeof(float), 
    &renderer->sizeBuffer[0]
  );

  // inAtlasOffset
  vbo_upload_subData(
    &renderer->vboAtlasOffset, 
    0, 
    renderer->quadCount * FLOATS_PER_POS * sizeof(float), 
    &renderer->atlasOffsetBuffer[0]
  );

  // inSpriteSize
  vbo_upload_subData(
    &renderer->vboSpriteSize, 
    0, 
    renderer->quadCount * FLOATS_PER_POS * sizeof(float), 
    &renderer->spriteSizeBuffer[0]
  );

  glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderer->quadCount);
}

void renderer2D_end(Renderer2D* renderer)
{
  renderer2D_flush(renderer);
  vao_unbind();
  shader_stop();
}

void renderer2D_cleanup(Renderer2D* renderer)
{
  vbo_cleanup(&renderer->vboPos);
  vbo_cleanup(&renderer->vboSize);
  vbo_cleanup(&renderer->vboAtlasOffset);
  vbo_cleanup(&renderer->vboSpriteSize);

  vao_cleanup(&renderer->vao);
  texture_cleanup(&renderer->textureAtlas);
  shader_cleanup(&renderer->shader);
}

void renderer2D_drawSpritePro(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale, float rot)
{
  if(renderer->quadCount >= MAX_QUADS)  return;
  
  Sprite sprite = SPRITES[spriteID];
  int posIDX = renderer->quadCount * FLOATS_PER_POS;

  // inPos
  renderer->posBuffer[posIDX] = pos.x;
  renderer->posBuffer[posIDX + 1] = pos.y;

  // inSize
  renderer->sizeBuffer[posIDX] = sprite.size.x * scale;
  renderer->sizeBuffer[posIDX + 1] = sprite.size.y * scale;

  // inAtlasOffset
  renderer->atlasOffsetBuffer[posIDX] = sprite.atlasOffset.x;
  renderer->atlasOffsetBuffer[posIDX + 1] = sprite.atlasOffset.y;

  // inSpriteSize
  renderer->spriteSizeBuffer[posIDX] = sprite.size.x;
  renderer->spriteSizeBuffer[posIDX + 1] = sprite.size.y;

  renderer->quadCount++;
}

void renderer2D_drawSprite(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale)
{
  renderer2D_drawSpritePro(renderer, spriteID, pos, scale, 0);
}

void renderer2D_drawAnimatedSpritePro(Renderer2D *renderer, SpriteID spriteID, Vec2 pos, float scale, float rot, float *timer, int *currentFrame, float fps)
{
  if(renderer->quadCount >= MAX_QUADS)  return;
  
  Sprite sprite = SPRITES[spriteID];
  int posIDX = renderer->quadCount * FLOATS_PER_POS;

  // Update Current Frame
  *timer += get_deltaTime();
  const float timeLimit = 1 / fps;
  if (*timer > timeLimit)
  {
    *timer -= timeLimit;
    *currentFrame = (*currentFrame + 1) % sprite.frameCount;
  }

  // inPos
  renderer->posBuffer[posIDX] = pos.x;
  renderer->posBuffer[posIDX + 1] = pos.y;

  // inSize
  renderer->sizeBuffer[posIDX] = sprite.size.x * scale;
  renderer->sizeBuffer[posIDX + 1] = sprite.size.y * scale;

  // inAtlasOffset
  renderer->atlasOffsetBuffer[posIDX] = sprite.atlasOffset.x + sprite.size.x * (*currentFrame);
  renderer->atlasOffsetBuffer[posIDX + 1] = sprite.atlasOffset.y;

  // inSpriteSize
  renderer->spriteSizeBuffer[posIDX] = sprite.size.x;
  renderer->spriteSizeBuffer[posIDX + 1] = sprite.size.y;

  renderer->quadCount++;
}

void renderer2D_drawAnimatedSprite(Renderer2D *renderer, SpriteID spriteID, Vec2 pos, float scale,  float *timer, int *currentFrame, float fps)
{
  renderer2D_drawAnimatedSpritePro(renderer, spriteID, pos, scale, 0, timer, currentFrame, fps);
}

void renderer2D_drawEntity(Renderer2D *renderer, Entity *entity)
{
  if(renderer->quadCount >= MAX_QUADS)  return;
  
  Sprite sprite = SPRITES[entity->spriteID];
  int posIDX = renderer->quadCount * FLOATS_PER_POS;

  // inPos
  renderer->posBuffer[posIDX] = entity->pos.x;
  renderer->posBuffer[posIDX + 1] = entity->pos.y;

  // inSize
  renderer->sizeBuffer[posIDX] = sprite.size.x * entity->scale;
  renderer->sizeBuffer[posIDX + 1] = sprite.size.y * entity->scale;

  // inAtlasOffset
  renderer->atlasOffsetBuffer[posIDX] = sprite.atlasOffset.x;
  renderer->atlasOffsetBuffer[posIDX + 1] = sprite.atlasOffset.y;

  // inSpriteSize
  renderer->spriteSizeBuffer[posIDX] = sprite.size.x;
  renderer->spriteSizeBuffer[posIDX + 1] = sprite.size.y;

  renderer->quadCount++;
}

void renderer2D_drawAnimEntity(Renderer2D *renderer, AnimEntity *entity)
{
  if(renderer->quadCount >= MAX_QUADS)  return;
  
  Sprite sprite = SPRITES[entity->spriteID];
  int posIDX = renderer->quadCount * FLOATS_PER_POS;

  // Update Current Frame
  entity->timer += get_deltaTime();
  const float timeLimit = 1 / entity->fps;
  if (entity->timer > timeLimit)
  {
    entity->timer -= timeLimit;
    entity->currentFrame = (entity->currentFrame + 1) % sprite.frameCount;
  }

  // inPos
  renderer->posBuffer[posIDX] = entity->pos.x;
  renderer->posBuffer[posIDX + 1] = entity->pos.y;

  // inSize
  renderer->sizeBuffer[posIDX] = sprite.size.x * entity->scale;
  renderer->sizeBuffer[posIDX + 1] = sprite.size.y * entity->scale;

  // inAtlasOffset
  renderer->atlasOffsetBuffer[posIDX] = sprite.atlasOffset.x + sprite.size.x * entity->currentFrame;
  renderer->atlasOffsetBuffer[posIDX + 1] = sprite.atlasOffset.y;

  // inSpriteSize
  renderer->spriteSizeBuffer[posIDX] = sprite.size.x;
  renderer->spriteSizeBuffer[posIDX + 1] = sprite.size.y;

  renderer->quadCount++;
}
