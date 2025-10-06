#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "draw.h"
#include "SDL.h"
#include "texture.h"

propRect propRectC(float x, float y, float w, float h, int offsetX, int offsetY, int consScale){
  propRect p = {x,y,w,h,offsetX,offsetY,consScale};
  return p;
}

SDL_Rect propRectToRect(propRect p,int width, int height){
  SDL_Rect Out;
  float scale = width/height;
  Out.x = p.offsetX + (p.x*(width - p.offsetX))/100.0;
  Out.y = p.offsetY + (p.y*(height - p.offsetY))/100.0;
  if(p.consScale){
    int w = p.w*width/100.0;
    int h = p.h*height/100.0;
    int wh = scale*h;
    if(w > wh){
      w = wh;
      Out.x += (p.w*width/100.0-w)/2.;
      
    }else{
      h = w*(1.0/scale);
      Out.y += (p.h*height/100.0-h)/2.;
    }
    Out.w = w;
    Out.h = h;
  }else{
    Out.w = p.w*width/100.0;
    Out.h = p.h*height/100.0;
  }
  return Out;
}

abrDraw * initAbrDraw(){
  return NULL;
}

abrDraw * createAbrDraw(abrDraw * pere, abrDraw * lv, abrDraw * lh, Texture * text, propRect pos,int reDraw){
  abrDraw * a = calloc(1, sizeof(abrDraw));

  a->pere = pere;
  a->lv = lv;
  a->lh = lh;
  a->text = text;
  a->pos.x = pos.x;
  a->pos.y = pos.y;
  a->pos.w = pos.w;
  a->pos.h = pos.h;
  a->pos.offsetX = pos.offsetX;
  a->pos.offsetY = pos.offsetY;
  a->pos.consScale = pos.consScale;
  a->reDraw = reDraw;
  
  return a;
}

void addAbrDraw(abrDraw ** pere, abrDraw * fils){
  fils->lh = *pere;
  *pere = fils;
}

void supAbrDraw(Window * win, abrDraw ** pprec){
  abrDraw * save = *pprec;
  if(save->lv == NULL){
    freeAbrDraw(win,&(save->lv));
  }
  *pprec = save->lh;
  freeTexture(win, save->text->texture);
  free(save);
}

void freeAbrDraw(Window * win,abrDraw ** a){
  if((*a)->lh != NULL){
    freeAbrDraw(win,&((*a)->lh));
  }
  if((*a)->lv != NULL){
    freeAbrDraw(win,&((*a)->lv));
  }

  freeTexture(win, (*a)->text->texture);
  free(*a);
  *a = NULL;
}

void updateTextAbrDraw(abrDraw * a, Window * win){
  if(a->lh){
    updateTextAbrDraw(a->lh, win);
  }
  if(a->reDraw && a->lv){
    SDL_DestroyTexture(a->text->texture->texture);
    a->text->texture->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, a->text->width, a->text->height);
    SDL_SetTextureBlendMode(a->text->texture->texture, SDL_BLENDMODE_BLEND);
    updateTextAbrDraw(a->lv, win);
  }
  
  int pereWidth,pereHeight;
  if(a->pere){
    pereWidth = a->pere->text->width;
    pereHeight = a->pere->text->height;
  }else{
    SDL_GetWindowSize(win->win, &pereWidth, &pereHeight);
  }
  
  SDL_Rect Out = propRectToRect(a->pos,pereWidth,pereHeight);
  
  a->text->rect.x = a->text->rect.w * (a->text->currentAnim%a->text->nbFramePerLine);
  a->text->rect.y = a->text->rect.h * (int)(a->text->currentAnim/a->text->nbFramePerLine);
  if(a->pere){
    SDL_SetRenderTarget(win->renderer, a->pere->text->texture->texture);
  }
  //printf("%d %d,%d %d %d %d, %d %d %d %d\n",pereWidth,pereHeight,a->text->rect.x,a->text->rect.y,a->text->rect.w,a->text->rect.h,Out.x,Out.y,Out.w,Out.h);
  SDL_RenderCopy(win->renderer, a->text->texture->texture, &(a->text->rect), &Out);
  if(a->pere){
    SDL_RenderPresent(win->renderer);
  }
  SDL_SetRenderTarget(win->renderer, NULL);
}

void requestReDraw(Window * win,abrDraw * abr){
  if(abr->pere){
    requestReDraw(win,abr->pere);
  }
  abr->reDraw = 1;
  win->reDraw = 1;
}
