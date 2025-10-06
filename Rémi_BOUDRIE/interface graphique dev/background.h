#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include <SDL2/SDL.h>

typedef struct BackgroundTexture
{
    SDL_Texture * texture;
    SDL_Rect pos;
    int caseWidth;
    int caseHeight;
    int caseSep;
    int nbCaseW;
    int nbCaseH;
    int nbMapLine;
    int nbMapColumn;
    int currentMap;
} BackgroundTexture;

SDL_Rect inputRect(BackgroundTexture bg);

int backgroundWidth(BackgroundTexture bg);

int backgroundHeight(BackgroundTexture bg);

BackgroundTexture * createBackground(SDL_Texture * texture,SDL_Rect pos,int caseWidth,int caseHeight,int caseSep,int nbCaseW, int nbCaseH, int nbMapLine, int nbMapColumn, int currentMap);

#endif