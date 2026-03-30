#pragma once
#include <Nexus_c.h>

// #############################################################################
//                           Sprites
// #############################################################################
typedef enum
{
  SPRITE_CAR,
  
  SPRITE_COUNT
} SpriteID;

typedef struct
{
  Vec2i atlasOffset;
  Vec2i size;
} Sprite;

static const Sprite SPRITES[SPRITE_COUNT] = {
  [SPRITE_CAR] = {.atlasOffset = {0, 0}, .size = {640, 480}},
};
