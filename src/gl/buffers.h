#pragma once
#include <Nexus_c.h>

// #############################################################################
//                           VBO
// #############################################################################
typedef struct
{
  GLuint ID;
  float *data;
  size_t size;
} VBO;

void vbo_init(VBO *vbo, float *data, size_t size);
void vbo_initDynamic(VBO *vbo, float *data, size_t size);
void vbo_upload_subData(VBO *vbo, int offset, size_t size, float *data);
void vbo_bind(const VBO *vbo);
void vbo_unbind();
void vbo_cleanup(VBO *vbo);

// #############################################################################
//                           EBO
// #############################################################################
typedef struct
{
  GLuint ID;
  GLuint *data;
  size_t size;
} EBO;

void ebo_init(EBO *ebo, GLuint *data, size_t size);
void ebo_bind(const EBO *ebo);
void ebo_unbind();
void ebo_cleanup(EBO *ebo);
