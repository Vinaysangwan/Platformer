#include "pch.h"
#include "vao.h"

void vao_init(VAO *vao)
{
  vao->vertexAttribArray = 0;
  glGenVertexArrays(1, &vao->ID);
  glBindVertexArray(vao->ID);
}

void vao_setupEBO(VAO *vao, EBO *ebo)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ID);
  vao->vertexCount = ebo->size;
}

void vao_insertVBO(VAO *vao, VBO *vbo, int coordinateSize)
{
  glVertexAttribPointer(vao->vertexAttribArray, coordinateSize, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(vao->vertexAttribArray);
  vao->vertexAttribArray++;
}

void vao_bind(const VAO *vao)
{
  glBindVertexArray(vao->ID);
}

void vao_unbind()
{
  glBindVertexArray(0);
}

void vao_cleanup(VAO *vao)
{
  glDeleteVertexArrays(1, &vao->ID);
}
