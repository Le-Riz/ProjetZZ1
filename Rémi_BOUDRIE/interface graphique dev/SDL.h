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
  int widthScreen;
  int heightScreen;
  int widthWin;
  int heightWin;
  SDL_Renderer * renderer;
  TextureSave * text;
  int nbText;
  int lastText;
  SDL_Event event;
  unsigned char program_on : 1;
  unsigned char reDraw : 1;
  abrDraw * abr;
  colLi ** collision;
  int nbCol;
  Increment * inc;
  TTF_Font * font;
} Window;

Window * initSDL(SDL_Point p, int w, int h, char * nom, char * font, char * icon);

void freeSDL(Window * win);

int setRenderDrawColor(SDL_Renderer * renderer, SDL_Color c);

SDL_Color colorC(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

SDL_Point pointC(int x, int y);

SDL_Rect rectC(int x, int y, int w, int h);

float toDeg(float rad);

float toRad(float deg);

/*void drawAround(SDL_Renderer * renderer, SDL_Point p1, SDL_Point p2, SDL_Point around);*/

Uint32 getTime();

void fpsFix(Uint32 timeStart, int fps);

void eventLoop(Window * win);

void loopCalc(Window * win);

#endif
