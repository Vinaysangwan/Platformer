#include "pch.h"
#include "renderer.h"

void renderer_draw(const Shader* shader, const VAO* vao)
{
  shader_start(shader);
  vao_bind(vao);

  glDrawElements(GL_TRIANGLES, vao->vertexCount, GL_UNSIGNED_INT, NULL);

  vao_unbind();
  shader_stop();
}

void renderer_drawTexture(const Shader* shader, const VAO* vao, const Texture *texture)
{ 
  shader_start(shader);
  vao_bind(vao);

  glActiveTexture(GL_TEXTURE0);
  texture_bind(texture);

  glDrawElements(GL_TRIANGLES, vao->vertexCount, GL_UNSIGNED_INT, NULL);

  texture_unbind();
  vao_unbind();
  shader_stop();
}
