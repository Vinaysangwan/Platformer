#pragma once
#include <Nexus_c.h>

// #############################################################################
//                           Sprites
// #############################################################################
typedef enum SpriteID
{
  SPRITE_QUAD,
  
  // Knight
  SPRITE_KNIGHT_IDLE,
  SPRITE_KNIGHT_RUN,
  SPRITE_KNIGHT_ROLL,

  // Tiles
  SPRITE_TILE_GRASS,
  
  SPRITE_COUNT
} SpriteID;

typedef struct Sprite
{
  Vec2i atlasOffset;
  Vec2i size;
  int frameCount;
} Sprite;

static const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_QUAD] = {.atlasOffset = {272, 0}, .size = {1, 1}, .frameCount = 1},
  
  // Knight
  [SPRITE_KNIGHT_IDLE] = {.atlasOffset = {0, 0}, .size = {32, 32}, .frameCount = 4},
  [SPRITE_KNIGHT_RUN] = {.atlasOffset = {0, 32}, .size = {32, 32}, .frameCount = 16},
  [SPRITE_KNIGHT_ROLL] = {.atlasOffset = {0, 64}, .size = {32, 32}, .frameCount = 8},

  // Tiles
  [SPRITE_TILE_GRASS] = {.atlasOffset = {608, 0}, .size = {16, 16}, .frameCount = 1},
};
