#include "pch.h"
#include "textures.h"
#include "logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

void texture_init(Texture *texture, const char* filePath)
{
  texture_initPro(texture, filePath, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST);
}

void texture_initPro(Texture *texture, const char* filePath, int wrapping, int minFilter, int magFilter)
{
  glGenTextures(1, &texture->ID);
  glBindTexture(GL_TEXTURE_2D, texture->ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

  int width, height, nChannels;
  char* data = (char*)stbi_load(filePath, &width, &height, &nChannels, 4);
  if (!data)
  {
    NX_ASSERT(false, "Failed to Open the texuture File: %s", filePath);
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);
}

void texture_bind(const Texture *texture)
{
  glBindTexture(GL_TEXTURE_2D, texture->ID);
}

void texture_unbind()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_cleanup(Texture *texture)
{
  glDeleteTextures(1, &texture->ID);
}
