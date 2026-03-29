#pragma once

typedef struct
{
  unsigned int ID;
} Shader;

void shader_init(Shader *shader, const char* vertFilePath, const char* fragFilePath);
void shader_start(const Shader* shader);
void shader_stop();
void shader_cleanup(Shader* shader);
