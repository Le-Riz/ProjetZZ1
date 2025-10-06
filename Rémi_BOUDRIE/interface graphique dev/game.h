#ifndef __GAME_H__
#define __GAME_H__

#include "SDL.h"

#define SRC_SPRITE "art&sprite/"
#define WALL_POS 0

void initGameTexture(Window * win);

void createWall(Window * win, int x, int y, abrDraw * bg);

void placeSpriteOnGrid(Window * win, int x, int y, abrDraw * Grid, Texture * sprite, Increment * spriteInc);

#endif