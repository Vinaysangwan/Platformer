#pragma once
#include "assets.h"

typedef struct Entity
{
  SpriteID spriteID;
  Vec2 pos;
  float rot;
  float scale;
} Entity;

typedef struct AnimEntity
{
  SpriteID spriteID;
  Vec2 pos;
  float rot;
  float scale;
  float timer;
  int currentFrame;
} AnimEntity;
