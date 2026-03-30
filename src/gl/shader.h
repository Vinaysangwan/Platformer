#pragma once
#include "utils.h"

typedef struct
{
  unsigned int ID;
} Shader;

void shader_init(Shader *shader, const char* vertFilePath, const char* fragFilePath);
void shader_start(const Shader* shader);
void shader_stop();
void shader_cleanup(Shader* shader);

// Uniforms
int shader_get_uniformLocation(Shader *shader, const char* name);
void shader_loadVec2i(int location, Vec2i vec);
void shader_loadMat4(int location, Mat4 mat);
