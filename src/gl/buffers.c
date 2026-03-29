#include "pch.h"
#include "buffers.h"

// #############################################################################
//                           VBO
// #############################################################################
void vbo_init(VBO *vbo, float *data, size_t size)
{
  vbo->data = data;
  vbo->size = size;
  
  glGenBuffers(1, &vbo->ID);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
  glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
}

void vbo_bind(const VBO *vbo)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
}

void vbo_unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_cleanup(VBO *vbo)
{
  glDeleteBuffers(1, &vbo->ID);
}

// #############################################################################
//                           EBO
// #############################################################################
void ebo_init(EBO *ebo, GLuint *data, size_t size)
{
  ebo->data = data;
  ebo->size = size;

  glGenBuffers(1, &ebo->ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void ebo_bind(const EBO *ebo)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ID);
}

void ebo_unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ebo_cleanup(EBO *ebo)
{
  glDeleteBuffers(1, &ebo->ID);
}
