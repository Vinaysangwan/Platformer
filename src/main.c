#include "pch.h"
#include "logger.h"
#include "utils.h"
#include "gl/shader.h"
#include "gl/vao.h"
#include "renderer.h"

int main(void)
{
  if(!window_create("Platformer", 1280, 720))
  {
    NX_ASSERT(false, "Failed to Create Window");
  }

  Shader shader;
  shader_init(&shader, "assets/shaders/quad.vert", "assets/shaders/quad.frag");
  
  float vertices[] = {
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
  };

  unsigned int indices[] = {
    0, 1, 3,
    3, 1, 2
  };

  VAO vao;
  vao_init(&vao);

  EBO ebo;
  ebo_init(&ebo, &indices[0], sizeof(indices) / sizeof(unsigned int));
  vao_setupEBO(&vao, &ebo);

  VBO vbo;
  vbo_init(&vbo, &vertices[0], sizeof(vertices) / sizeof(float));
  vao_insertVBO(&vao, &vbo, 3);

  vao_unbind();
  vbo_cleanup(&vbo);

  while(window_isOpen())
  {
    if(key_down(KEY_ESCAPE))
    {
      window_stop();
    }

    clear_background(&COLOR_BLACK);

    renderer_draw(&shader, &vao);

    shader_stop();

    window_swap_buffers();
  }

  ebo_cleanup(&ebo);
  vao_cleanup(&vao);
  shader_cleanup(&shader);
  window_close();
  
  return 0;
}
