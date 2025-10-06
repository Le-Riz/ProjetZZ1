#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "bullet.h"

windLi_c * initListBullet(){
  return NULL;
}

void addListBullet(windLi_c ** tete, windLi_c elem){
  windLi_c * nouvEle = calloc(1,sizeof(windLi_c));
  
  nouvEle->renderer = elem.renderer;
  nouvEle->win = elem.win;
  nouvEle->posX = elem.posX;
  nouvEle->posY = elem.posY;
  nouvEle->nombreTour = elem.nombreTour;
  nouvEle->height = elem.height;
  nouvEle->animI = elem.animI;
  nouvEle->next = *tete;

  *tete = nouvEle;
}

void delListBullet(windLi_c ** pprec){
  windLi_c * save = *pprec;
  *pprec = save->next;
  freeWindLi_c(save);
}

void freeWindLi_c(windLi_c * elem){
  SDL_DestroyWindow(elem->win);
  SDL_DestroyRenderer(elem->renderer);
  free(elem);
}

void freeListBullet(windLi_c ** tete){
  windLi_c ** pprec = tete;
  while(*pprec != NULL){
    delListBullet(pprec);
  }
}
