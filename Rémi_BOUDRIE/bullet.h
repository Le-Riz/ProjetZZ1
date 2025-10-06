#ifndef __BULLET_H__
#define __BULLET_H__

#include <SDL2/SDL.h>

typedef  struct windLi_c{
  SDL_Window * win;
  int posX;
  int posY;
  int height;
  int nombreTour;
  float animI;
  SDL_Renderer * renderer;
  struct windLi_c * next;
} windLi_c;

windLi_c * initListBullet();

void addListBullet(windLi_c **, windLi_c);

void delListBullet(windLi_c **);

void freeWindLi_c(windLi_c *);

void freeListBullet(windLi_c **);

#endif
