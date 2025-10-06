#ifndef __SDL_H__
#define __SDL_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
typedef struct Window Window;
#include "draw.h"
#include "collision.h"
#include "texture.h"


typedef struct Window{
  SDL_Window * win;
  int width;
  int height;
  SDL_Renderer * renderer;
  TextureSave * text;
  int nbText;
  int lastText;
  SDL_Event event;
  int reDraw;
  abrDraw * abr;
  colLi ** collision;
  int nbCol;
  TTF_Font * font;
} Window;

Window * initSDL(SDL_Point p, int w, int h, char * nom, char * font);

void freeSDL(Window * win);

int setRenderDrawColor(SDL_Renderer * renderer, SDL_Color c);

SDL_Color colorC(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

SDL_Point pointC(int x, int y);

SDL_Rect rectC(int x, int y, int w, int h);

float toRad(float deg);

void drawFillCircle(SDL_Renderer * c, SDL_Point p, float radius);

void drawCircle(SDL_Renderer * renderer, SDL_Point p, float radius);

void drawArc(SDL_Renderer * renderer, SDL_Point p, float radius, float startAngle, float endAngle);

void drawAround(SDL_Renderer * renderer, SDL_Point p1, SDL_Point p2, SDL_Point around);

Uint32 getTime();

void fpsFix(Uint32 timeStart, int fps);

void eventLoop(Window * win, int *  program_on);

void loopCalc(Window * win, int * program_on);

void strDraw(Window * win, SDL_Rect rect, SDL_Color color, char * str);

#endif
