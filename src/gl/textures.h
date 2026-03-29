#pragma once

typedef struct
{
  unsigned int ID;
} Texture;

void texture_init(Texture *texture, const char* filePath);
void texture_initPro(Texture *texture, const char* filePath, int wrapping, int minFilter, int magFilter);
void texture_bind(const Texture *texture);
void texture_unbind();
void texture_cleanup(Texture *texture);
