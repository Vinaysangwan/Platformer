#pragma once
#include "gl/shader.h"
#include "gl/vao.h"
#include "gl/textures.h"
#include "assets.h"

void renderer_draw(const Shader* shader, const VAO* vao);
void renderer_drawTexture(const Shader* shader, const VAO* vao, const Texture *texture);

typedef struct
{
  Shader shader;
  VAO vao;
  EBO ebo;
  Texture textureAtlas;

  GLint location_projectionMatrix;
  GLint location_transformationMatrix;
  GLint location_spriteSize;
  GLint location_atlasOffset;
} Renderer2D;

void renderer2D_init(Renderer2D* renderer, const char* vertFilePath, const char* fragFilePath, const char* textureAtlasPath);
void renderer2D_drawSpritePro(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float rot, float scale);
void renderer2D_drawSprite(Renderer2D* renderer, SpriteID spriteID, Vec2 pos, float scale);
void renderer2D_cleanup(Renderer2D* renderer);
