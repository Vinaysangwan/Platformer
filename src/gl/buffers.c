#include "pch.h"
#include "buffers.h"

// #############################################################################
//                           VBO
// #############################################################################
void vbo_init(VBO *vbo, float *data, size_t size)
{
  vbo->data = data;
  vbo->size = size / sizeof(float);
  
  glGenBuffers(1, &vbo->ID);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void vbo_initDynamic(VBO *vbo, float *data, size_t size)
{
  vbo->data = data;
  vbo->size = size / sizeof(float);

  glGenBuffers(1, &vbo->ID);
  glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void vbo_upload_subData(VBO *vbo, int offset, size_t size, float *data)
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo->ID);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
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
  ebo->size = size / sizeof(unsigned int);

  glGenBuffers(1, &ebo->ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo->ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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
