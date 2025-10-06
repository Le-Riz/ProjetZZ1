#ifndef __DRAW_H__
#define __DRAW_H__

#include <SDL2/SDL.h>
typedef struct abrDraw abrDraw;
typedef struct propRect propRect;
#include "SDL.h"
#include "texture.h"

typedef struct propRect{
  float x;     //proportion sur 100 par rapport à la dimension du père
  float y;
  float w;
  float h;
  int offsetX; //nombre de pixel à ajouter sans tenir compte de la taille du père
  int offsetY;
  int consScale;
} propRect;

typedef struct abrDraw{
  struct abrDraw * pere;
  struct abrDraw * lh;
  struct abrDraw * lv;
  int reDraw;
  Texture * text;
  propRect pos;
} abrDraw;

propRect propRectC(float x, float y, float w, float h, int offsetX, int offsetY, int consScale);

SDL_Rect propRectToRect(propRect p,int width, int height);

abrDraw * initAbrDraw();

abrDraw * createAbrDraw(abrDraw * pere, abrDraw * lv, abrDraw * lh, Texture * text, propRect pos,int reDraw);

void addAbrDraw(abrDraw ** pere, abrDraw * fils);

void supAbrDraw(Window * win,abrDraw ** pprec);

void freeAbrDraw(Window * win,abrDraw ** a);

void updateTextAbrDraw(abrDraw * a, Window * win);

void requestReDraw(Window * win,abrDraw * abr);

#endif
