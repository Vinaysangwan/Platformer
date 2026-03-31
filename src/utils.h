#pragma once
#include <Nexus_c.h>

#define DEG_2_RAD 0.0174532925199433

char *read_file(const char* filePath);

// #############################################################################
//                           Mat4
// #############################################################################
typedef struct
{
  union
  {
    float values[16];

    struct
    {
      float ax;
      float bx;
      float cx;
      float dx;

      float ay;
      float by;
      float cy;
      float dy;

      float az;
      float bz;
      float cz;
      float dz;

      float aw;
      float bw;
      float cw;
      float dw;
    };
  };
} Mat4;

const static Mat4 IDENTITY_MAT4 = {
  1, 0, 0,
  0, 1, 0,
  0, 0, 1
};

Mat4 orthographic_project(float left, float right, float top, float bottom);
Mat4 transformation_matrix(Vec2 pos, float rot, Vec2 scale);
Mat4 view_matrix(Vec2 pos, float rot, float zoom, int screenW, int screenH);
