#ifndef __DRAW_H__
#define __DRAW_H__

#include <SDL2/SDL.h>
typedef struct abrDraw abrDraw;
typedef struct propRect propRect;
#include "SDL.h"
#include "texture.h"

typedef enum {
  TEXTURE,
  GEOMETRIE,
  BACKGROUND
} drawType;

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
  unsigned char reDraw : 1;
  unsigned char active : 1;
  drawType type;//bite
  void * object;
  propRect pos;//B====D
  Increment * increment;
} abrDraw;

propRect propRectC(float x, float y, float w, float h, int offsetX, int offsetY, int consScale);

SDL_Rect propRectToRect(propRect p,int widthOut, int heightOut, int widthOri, int heightOri);

abrDraw * initAbrDraw();

abrDraw * createAbrDraw(abrDraw * pere, abrDraw * lv, abrDraw * lh, drawType type, void * obj, propRect pos,int reDraw,int active, Increment * increment);

void addAbrDraw(abrDraw ** pere, abrDraw * fils);

void supAbrDraw(Window * win,abrDraw ** pprec);

void freeAbrDraw(Window * win,abrDraw ** a);

void updateTextAbrDraw(abrDraw * a, Window * win);

void requestReDraw(Window * win,abrDraw * abr);

void requestReDrawResize(Window * win);

void getSizeAbr(Window * win,abrDraw * abr, int * width, int * height);

void activateAbr(abrDraw * abr, int activate);

#endif
