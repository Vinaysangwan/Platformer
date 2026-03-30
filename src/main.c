#include "pch.h"
#include "logger.h"
#include "utils.h"
#include "renderer.h"

int main(void)
{
  if(!window_create("Platformer", 1280, 720))
  {
    NX_ASSERT(false, "Failed to Create Window");
  }

  Renderer2D renderer;
  renderer2D_init(&renderer, "assets/shaders/quad.vert", "assets/shaders/quad.frag", "assets/textures/car.jpg");

  while(window_isOpen())
  {
    if(key_down(KEY_ESCAPE))
    {
      window_stop();
    }

    clear_background(&COLOR_BLACK);

    renderer2D_drawSprite(&renderer, SPRITE_CAR, (Vec2){1280 / 2.0f, 720 / 2.0f}, 1.0f);

    window_swap_buffers();
  }

  renderer2D_cleanup(&renderer);
  window_close();
  
  return 0;
}
