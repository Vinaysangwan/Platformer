#include "pch.h"
#include "logger.h"
#include "utils.h"
#include "gl/shader.h"
#include "gl/vao.h"
#include "renderer.h"
#include "gl/textures.h"

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

  float textureCoords[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
  };

  unsigned int indices[] = {
    0, 1, 3,
    3, 1, 2
  };

  Texture texture;
  texture_initPro(&texture, "assets/textures/car.jpg", GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);

  VAO vao;
  vao_init(&vao);

  EBO ebo;
  ebo_init(&ebo, &indices[0], sizeof(indices));
  vao_setupEBO(&vao, &ebo);

  VBO vboVertices;
  vbo_init(&vboVertices, &vertices[0], sizeof(vertices));
  vao_insertVBO(&vao, &vboVertices, 3);

  VBO vboTexCoords;
  vbo_init(&vboTexCoords, &textureCoords[0], sizeof(textureCoords));
  vao_insertVBO(&vao, &vboTexCoords, 2);

  vao_unbind();
  vbo_cleanup(&vboVertices);
  vbo_cleanup(&vboTexCoords);

  while(window_isOpen())
  {
    if(key_down(KEY_ESCAPE))
    {
      window_stop();
    }

    clear_background(&COLOR_BLACK);

    renderer_drawTexture(&shader, &vao, &texture);

    window_swap_buffers();
  }

  texture_cleanup(&texture);
  ebo_cleanup(&ebo);
  vao_cleanup(&vao);
  shader_cleanup(&shader);
  window_close();
  
  return 0;
}
