#pragma once
#include "gl/shader.h"
#include "gl/vao.h"
#include "gl/textures.h"
#include "assets.h"
#include "entities.h"

void renderer_draw(const Shader* shader, const VAO* vao);
void renderer_drawTexture(const Shader* shader, const VAO* vao, const Texture *texture);

// #############################################################################
//                           Renderer2D
// #############################################################################
#define MAX_QUADS 10000
#define FLOATS_PER_POS 2
#define FLOATS_PER_UV 2
#define VERT_PER_QUAD 4
#define IDX_PER_QUAD 6

#define SIZE_VBO_POS MAX_QUADS * VERT_PER_QUAD * FLOATS_PER_POS
#define SIZE_VBO_UV MAX_QUADS * VERT_PER_QUAD * FLOATS_PER_UV

typedef struct
{
  Vec2 pos;
  float rot;
  float zoom;
} Camera2D;

typedef struct
{
  Shader shader;
  Texture textureAtlas;
  VAO vao;
  EBO ebo;
  VBO vboPos;
  VBO vboUV;

  float posBuffer[SIZE_VBO_POS];
  float uvBuffer[SIZE_VBO_UV];
  int quadCount;

  // uniform locations
  GLint location_projectionMatrix;
  GLint location_viewMatrix;
} Renderer2D;

void renderer2D_init(Renderer2D* renderer, const char* vertFilePath, const char* fragFilePath, const char* textureAtlasPath);
void renderer2D_beginCamera(Renderer2D* renderer, Camera2D *cam);
void renderer2D_begin(Renderer2D* renderer);
void renderer2D_end(Renderer2D* renderer);
void renderer2D_cleanup(Renderer2D* renderer);

void renderer2D_drawSpritePro(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale, float rot);
void renderer2D_drawSprite(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale);
// TODO: Fix this
void renderer2D_drawAnimatedSpritePro(Renderer2D *renderer, SpriteID spriteID, float fps, Vec2 pos, float scale, float rot);
void renderer2D_drawAnimatedSprite(Renderer2D *renderer, SpriteID spriteID, float fps, Vec2 pos, float scale);

void renderer2D_drawEntity(Renderer2D *renderer, Entity *entity);
void renderer2D_drawAnimEntity(Renderer2D *renderer, AnimEntity *entity, float duration);
