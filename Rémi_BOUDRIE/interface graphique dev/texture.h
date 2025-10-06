#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <SDL2/SDL.h>
typedef struct TextureSave TextureSave;
typedef struct Texture Texture;
typedef struct Increment Increment;
#include "SDL.h"
#include "draw.h"

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

typedef struct Increment{
  int * var;
  int wait;
  int waitOri;
  int increment;
  int stop;
  int origine;
  unsigned char reboot : 1;
  unsigned char active : 1;
  abrDraw * abr;
  struct Increment * next;
} Increment;

Texture * makeSprite(Window * win, Texture ** allSprite, int number, int waitTime);

int saveTexture(Window * win,Texture * texture);

int checkTextureSave(Window * win, TextureSave * texture);

TextureSave * createEmptyTexture(SDL_Texture * texture);

void freeTexture(Window * win,TextureSave * texture);

Texture * loadTexture(Window * win, int id);

void strDraw(Window * win, abrDraw * pere,SDL_Rect rect, SDL_Color color, char * str);

/// @brief 
/// @param win 
/// @param fils addresse du père à qui ont veut attribuer la texture
/// @param width longueur de la texture si -1 longueur du père
/// @param height largeur de la texture si -1 largeur du père
/// @return addresse de la texture crée
Texture * createPereTexture(Window * win,abrDraw * fils,int width, int height);

Texture * createTexture(SDL_Texture * texture, SDL_Rect rect, int width, int height, int currentAnim, int nbFramePerLine, int nbLine, int waitTime, int waitCounter);

Increment * createIncrement(int * variable, int wait, int increment, int stop, int origine, int reboot, abrDraw * abr, int active);

void freeIncrement(Increment ** inc);

void addIncrement(Increment ** pprec, Increment * cour);

void delIncrement(Increment ** pprec);

void evalIncrement(Window * win, Increment ** pprec);

#endif