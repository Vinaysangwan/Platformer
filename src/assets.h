#pragma once
#include <Nexus_c.h>

// #############################################################################
//                           Sprites
// #############################################################################
typedef enum
{
  SPRITE_KNIGHT,
  
  SPRITE_COUNT
} SpriteID;

typedef struct
{
  Vec2i atlasOffset;
  Vec2i size;
} Sprite;

static const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_KNIGHT] = {.atlasOffset = {32, 0}, .size = {32, 32}},
};
