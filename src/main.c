#include "pch.h"
#include "config.h"
#include "logger.h"
#include "renderer.h"

typedef struct
{
  SpriteID spriteID;
  Vec2 pos;
  float rot;
  float scale;

  float speed;
} Entity;

int main(void)
{
  if(!window_create("Platformer", WINDOW_WIDTH, WINDOW_HEIGHT))
  {
    NX_ASSERT(false, "Failed to Create Window");
  }
  window_set_vsync(true);

  Renderer2D renderer;
  renderer2D_init(&renderer, "assets/shaders/quad.vert", "assets/shaders/quad.frag", TEXTURE_ATLAS);

  Entity player = (Entity){
    .spriteID = SPRITE_KNIGHT,
    .pos = (Vec2){0, 0},
    .rot = 0.0f,
    .scale = 2.0f,
    .speed = 2.0f
  };

  // Game Loop
  while(window_isOpen())
  {
    // Game Update
    if(key_down(KEY_ESCAPE))
    {
      window_stop();
    }

    if (key_down(KEY_W)) player.pos.y -= player.speed;
    if (key_down(KEY_S)) player.pos.y += player.speed;
    if (key_down(KEY_A)) player.pos.x -= player.speed;
    if (key_down(KEY_D)) player.pos.x += player.speed;

    // Game Rendering
    clear_background(&COLOR_BLACK);

    renderer2D_drawSpritePro(&renderer, player.spriteID, player.pos, player.rot, player.scale);

    window_swap_buffers();
  }

  renderer2D_cleanup(&renderer);
  window_close();
  
  return 0;
}
