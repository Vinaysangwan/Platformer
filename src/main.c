#include "pch.h"
#include "config.h"
#include "logger.h"
#include "renderer.h"

#define MAX_TILES 50

typedef struct
{
  SpriteID spriteID;
  Vec2 pos;
  float rot;
  float scale;
} Entity;

int main(void)
{
  if(!window_create("Platformer", WINDOW_WIDTH, WINDOW_HEIGHT))
  {
    NX_ASSERT(false, "Failed to Create Window");
  }

  Renderer2D renderer;
  renderer2D_init(&renderer, "assets/shaders/quad.vert", "assets/shaders/quad.frag", TEXTURE_ATLAS);

  Entity player = {
    .spriteID = SPRITE_KNIGHT_IDLE,
    .pos = {16, 16},
    .rot = 0.0f,
    .scale = 1.0f,
  };

  Camera2D camera = {
    .pos = player.pos,
    .rot = 0,
    .zoom = 2.0f,
  };

  float speed = 200;

  Entity tiles[MAX_TILES] = {0};    // 5 x 10 Grid Pattern
  for (int i = 0; i < MAX_TILES; i++)
  {
    int gridX = 5;
    int gridY = 10;

    int r = 16 * (i / 10 + gridY) + 16;
    int c = 16 * (i % 10 + gridX) + 16;
    
    Entity tile = {
      .spriteID = SPRITE_TILE_GRASS,
      .pos = {c, r},
      .rot = 0.0f,
      .scale = 1.0f,
    };

    tiles[i] = tile;
  }

  // Game Loop
  while(window_isOpen())
  {
    float dt = get_deltaTime();
    static float timer = 0;
    timer += dt;
    if (timer >= 1.0f)
    {
      printf("FPS: %d\n", get_FPS());
      timer = 0;
    }
    
    if(key_down(KEY_ESCAPE))
    {
      window_stop();
    }

    camera.pos = player.pos;
    if (key_down(KEY_W)) player.pos.y -= speed * dt;
    if (key_down(KEY_S)) player.pos.y += speed * dt;
    if (key_down(KEY_A)) player.pos.x -= speed * dt;
    if (key_down(KEY_D)) player.pos.x += speed * dt;

    if (key_down(KEY_Q)) player.rot -= 1.0f;
    if (key_down(KEY_E)) player.rot += 1.0f;
    if (key_down(KEY_LEFT)) camera.rot -= 1.0f;
    if (key_down(KEY_RIGHT)) camera.rot += 1.0f;

    clear_background(&COLOR_BLACK);

    // Game
    renderer2D_beginCamera(&renderer, &camera);
    {
      // Tiles
      for (int i=0; i < MAX_TILES; i++)
      {
        Entity tile = tiles[i];
        renderer2D_drawSprite(&renderer, tile.spriteID, tile.pos, tile.scale);
      }

      // Player
      renderer2D_drawSpritePro(&renderer, player.spriteID, player.pos, player.scale, player.rot);
    }
    renderer2D_end(&renderer);

    // UI
    renderer2D_begin(&renderer);
    {
      renderer2D_drawSprite(&renderer, SPRITE_QUAD, (Vec2){100, 100}, 50);
    }
    renderer2D_end(&renderer);

    window_swap_buffers();
  }

  renderer2D_cleanup(&renderer);
  window_close();
  
  return 0;
}
