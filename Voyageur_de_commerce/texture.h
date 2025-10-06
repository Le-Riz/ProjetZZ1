#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
typedef struct TextureSave TextureSave;
typedef struct Texture Texture;
#include "SDL.h"

typedef struct TextureSave{
  SDL_Texture * texture;
  int width;
  int height;
  int nbFramePerLine;
  int nbLine;
  int waitTime;
} TextureSave;

typedef struct Texture{
  TextureSave * texture;
  SDL_Rect rect;
  int width;
  int height;
  float scale; // width/height 1 frame
  int currentAnim; // start at 0
  int nbFramePerLine;
  int nbLine;
  int waitTime;
  int waitCounter;
} Texture;

Texture * makeSprite(Window * win, Texture ** allSprite, int number, int waitTime);

int saveTexture(Window * win,Texture * texture);

int checkTextureSave(Window * win, TextureSave * texture);

TextureSave * createEmptyTexture(SDL_Texture * texture);

void freeTexture(Window * win,TextureSave * texture);

Texture * loadTexture(TextureSave * text);

#endif
