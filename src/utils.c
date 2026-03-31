#include "pch.h"
#include "utils.h"
#include "logger.h"

char *read_file(const char *filePath)
{
  FILE *file;
  char *output;

  if(fopen_s(&file, filePath, "rb") == 0)
  {
    long bufferCount;
    fseek(file, 0, SEEK_END);
    bufferCount = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc((bufferCount + 1) * sizeof(char));

    fread(buffer, sizeof(char), bufferCount, file);
    buffer[bufferCount] = '\0';
    output = buffer;

    fclose(file);
  }
  else
  {
    NX_ASSERT(false, "Failed to Open the File: %s", filePath);
  }

  return output;
}

// #############################################################################
//                           Mat4
// #############################################################################
Mat4 orthographic_project(float left, float right, float top, float bottom)
{
  Mat4 result = {0};

  result.values[0 + 0 * 4] = 2.0f / (right - left);
  result.values[1 + 1 * 4] = 2.0f / (top - bottom);
  result.values[2 + 2 * 4] = 1.0f;
  result.values[3 + 3 * 4] = 1.0f;

  result.aw = -(right + left) / (right - left);
  result.bw = -(top + bottom) / (top - bottom);
  result.cw = 0.0f;

  return result;
}

Mat4 transformation_matrix(Vec2 pos, float rot, Vec2 scale)
{
  Mat4 result = {0};

  float rad = rot * DEG_2_RAD;
  float cosV = cosf(rad);
  float sinV = sinf(rad);

  result.ax = scale.x * cosV;
  result.ay = -scale.y * sinV;
  result.aw = pos.x;

  result.bx = scale.x * sinV;
  result.by = scale.y * cosV;
  result.bw = pos.y;

  result.cz = 1.0f;

  result.dw = 1.0f;

  return result;
}

Mat4 view_matrix(Vec2 pos, float rot, float zoom, int screenW, int screenH)
{
  Mat4 view = {0};

  float rad = DEG_2_RAD * rot;
  float cosV = cosf(rad);
  float sinV = sinf(rad);
  float tx = -pos.x;
  float ty = -pos.y;

  view.values[0 + 0 * 4] = cosV * zoom;
  view.values[1 + 0 * 4] = -sinV * zoom;
  view.values[0 + 1 * 4] = sinV * zoom;
  view.values[1 + 1 * 4] = cosV * zoom;

  view.values[0 + 3 * 4] = (tx * cosV + ty * sinV) * zoom + screenW * 0.5f;
  view.values[1 + 3 * 4] = (tx * (-sinV) + ty * cosV) * zoom + screenH * 0.5f;

  view.values[2 + 2 * 4] = 1.0f;
  view.values[3 + 3 * 4] = 1.0f;

  return view;
}
