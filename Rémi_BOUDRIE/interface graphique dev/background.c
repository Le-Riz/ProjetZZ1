#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "background.h"

SDL_Rect inputRect(BackgroundTexture bg){
    SDL_Rect rect;
    rect.x = bg.pos.x * bg.caseWidth + (bg.pos.x-1) * bg.caseSep + (bg.currentMap%bg.nbMapLine) * (bg.nbCaseW * bg.caseWidth + (bg.nbCaseW-1) * bg.caseSep);
    rect.y = bg.pos.y * bg.caseHeight + (bg.pos.y-1) * bg.caseSep + (int)(bg.currentMap/bg.nbMapLine) * (bg.nbCaseH * bg.caseHeight+ (bg.nbCaseH-1) * bg.caseSep);
    rect.w = bg.pos.w * bg.caseWidth + (bg.pos.w-1) * bg.caseSep;
    rect.h = bg.pos.h * bg.caseHeight + (bg.pos.h-1) * bg.caseSep;
    return rect;
}

int backgroundWidth(BackgroundTexture bg){
    return (bg.caseWidth * bg.nbCaseW + bg.nbCaseW * (1-bg.caseSep))* bg.nbMapLine;
}

int backgroundHeight(BackgroundTexture bg){
    return (bg.caseHeight * bg.nbCaseH + bg.nbCaseH * (1-bg.caseSep)) * bg.nbMapColumn;
}

BackgroundTexture * createBackground(SDL_Texture * texture,SDL_Rect pos,int caseWidth,int caseHeight,int caseSep,int nbCaseW, int nbCaseH, int nbMapLine, int nbMapColumn, int currentMap){
    BackgroundTexture * bgText = calloc(1,sizeof(BackgroundTexture));
    bgText->texture = texture;
    bgText->pos = pos;
    bgText->caseWidth = caseWidth;
    bgText->caseHeight = caseHeight;
    bgText->caseSep = caseSep;
    bgText->nbCaseW = nbCaseW;
    bgText->nbCaseH = nbCaseH;
    bgText->nbMapLine = nbMapLine;
    bgText->nbMapColumn = nbMapColumn;
    bgText->currentMap = currentMap;
    return bgText;
}