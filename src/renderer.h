#pragma once
#include "gl/shader.h"
#include "gl/vao.h"
#include "gl/textures.h"

void renderer_draw(const Shader* shader, const VAO* vao);
void renderer_drawTexture(const Shader* shader, const VAO* vao, const Texture *texture);
