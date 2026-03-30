#include "pch.h"
#include "shader.h"
#include "logger.h"

GLuint get_shaderID(const char* filePath, GLenum type)
{
  const char* shaderSource = read_file(filePath);
  GLuint shaderID = glCreateShader(type);
  glShaderSource(shaderID, 1, &shaderSource, NULL);
  glCompileShader(shaderID);

  int success;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    char infoLog[1024] = {0};
    glGetShaderInfoLog(shaderID, 1024, NULL, &infoLog[0]);
    const char *shaderName = (type == GL_VERTEX_SHADER) ? "Vertex" : "fragment";

    NX_ASSERT(false, "Failed to Compile %s Shader: %s", shaderName, infoLog);
  }

  free((void*)shaderSource);
  return shaderID;
}

void shader_init(Shader *shader, const char* vertFilePath, const char* fragFilePath)
{
  GLuint vertID = get_shaderID(vertFilePath, GL_VERTEX_SHADER);
  GLuint fragID = get_shaderID(fragFilePath, GL_FRAGMENT_SHADER);

  shader->ID = glCreateProgram();
  glAttachShader(shader->ID,  vertID);
  glAttachShader(shader->ID, fragID);
  glLinkProgram(shader->ID);

  int success;
  glGetProgramiv(shader->ID, GL_LINK_STATUS, &success);
  if(success == GL_FALSE)
  {
    char infoLog[1024] = {0};
    glGetProgramInfoLog(shader->ID, 1024, NULL, &infoLog[0]);

    NX_ASSERT(false, "Failed to Link Shader Program[%u]: %s", shader->ID, infoLog);
  }

  glDetachShader(shader->ID, vertID);
  glDetachShader(shader->ID, fragID);
  glDeleteShader(vertID);
  glDeleteShader(fragID);
}

void shader_start(const Shader* shader)
{
  glUseProgram(shader->ID);
}

void shader_stop()
{
  glUseProgram(0);
}

void shader_cleanup(Shader* shader)
{
  glDeleteProgram(shader->ID);
}

// Uniforms
int shader_get_uniformLocation(Shader *shader, const char* name)
{
  int location = glGetUniformLocation(shader->ID, name);
  if (location < 0)
  {
    NX_ASSERT(false, "Failed to get Uniform Location: %s", name);
  }

  return location;
}

void shader_loadMat4(int location, Mat4 mat)
{
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat.values[0]);
}
