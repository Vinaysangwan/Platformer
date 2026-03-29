#pragma once
#include "buffers.h"

typedef struct
{
  GLuint ID;
  int vertexAttribArray;
  int vertexCount;
} VAO;

void vao_init(VAO *vao);
void vao_setupEBO(VAO *vao, EBO *ebo);
void vao_insertVBO(VAO *vao, VBO *vbo, int coordinateSize);
void vao_bind(const VAO *vao);
void vao_unbind();
void vao_cleanup(VAO *vao);
