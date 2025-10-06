#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "draw.h"
#include "SDL.h"
#include "texture.h"
#include "background.h"
#include "geometrie.h"

propRect propRectC(float x, float y, float w, float h, int offsetX, int offsetY, int consScale){
  propRect p = {x,y,w,h,offsetX,offsetY,consScale};
  return p;
}

SDL_Rect propRectToRect(propRect p,int widthOut, int heightOut, int widthOri, int heightOri){
  SDL_Rect Out;
  Out.x = p.offsetX + (float)(p.x*(widthOut - p.offsetX))/100.0;
  Out.y = p.offsetY + (float)(p.y*(heightOut - p.offsetY))/100.0;
  if(p.consScale){
    //printf("dimOri w:%d h:%d\n",widthOri,heightOri);
    float percentW = (float)p.w/100.;
    float percentH = (float)p.h/100.;
    float ratioW = (float)widthOri/(float)heightOri;
    float ratioH = (float)heightOri/(float)widthOri;
    float newW = percentW * widthOut;
    float newH = percentH * heightOut;
    float WwithH = newH * ratioW;
    float HwithW = newW * ratioH;
    //printf("new w:%f h:%f changed w:%f h:%f\n",newW,newH,WwithH,HwithW);
    if(newW > newH){
      Out.w = WwithH;
      Out.h = newH;
      Out.x += (float)(newW - WwithH)/2.;
    }else{
      Out.w = newW;
      Out.h = HwithW;
      Out.y += (float)(newH - HwithW)/2.;
    }
  }else{
    Out.w = p.w*widthOut/100.0;
    Out.h = p.h*heightOut/100.0;
  }
  return Out;
}

abrDraw * initAbrDraw(){
  return NULL;
}

void freeObject(Window * win, abrDraw * abr){
  switch(abr->type){
    case TEXTURE:
      freeTexture(win, abr->object);
      break;
    
    case GEOMETRIE:
      //à faire
      break;
    
    case BACKGROUND:
      //à faire
      break;
    
    default:
      break;
  }
}

abrDraw * createAbrDraw(abrDraw * pere, abrDraw * lv, abrDraw * lh, drawType type, void * obj, propRect pos, int reDraw, int active, Increment * increment){
  abrDraw * a = calloc(1, sizeof(abrDraw));

  a->pere = pere;
  a->lv = lv;
  a->lh = lh;
  a->type = type;
  a->object = obj;
  a->pos.x = pos.x;
  a->pos.y = pos.y;
  a->pos.w = pos.w;
  a->pos.h = pos.h;
  a->pos.offsetX = pos.offsetX;
  a->pos.offsetY = pos.offsetY;
  a->pos.consScale = pos.consScale;
  a->reDraw = reDraw;
  a->active = active;
  a->increment = increment;
  
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

  freeObject(win, save);
  free(save);
}

void freeAbrDraw(Window * win,abrDraw ** a){
  if((*a)->lh != NULL){
    freeAbrDraw(win,&((*a)->lh));
  }
  if((*a)->lv != NULL){
    freeAbrDraw(win,&((*a)->lv));
  }

  freeObject(win, (*a));
  free(*a);
  *a = NULL;
}

void resetDraw(Window * win,abrDraw * abr){
  if(abr->lv && abr->reDraw){
    if(abr->type == TEXTURE){
      Texture * text = (Texture * ) abr->object;
      SDL_SetRenderTarget(win->renderer, text->texture->texture);
      SDL_SetRenderDrawColor(win->renderer,0,0,0,0);
      SDL_RenderClear(win->renderer);
      SDL_SetTextureBlendMode(text->texture->texture, SDL_BLENDMODE_BLEND);
      
    }else if(abr->type == BACKGROUND){
      BackgroundTexture * bg = (BackgroundTexture * ) abr->object;
      SDL_SetRenderTarget(win->renderer, bg->texture);
      SDL_SetRenderDrawColor(win->renderer,0,0,0,0);
      SDL_SetTextureBlendMode(bg->texture, SDL_BLENDMODE_BLEND);
    }else{
      printf("Erreur reset geometrie\n");
    }
  }
}

void getSizeAbr(Window * win,abrDraw * abr, int * width, int * height){
  if(abr->pere){
    if(abr->pere->type == TEXTURE){
      printf("Pere texture %d %d\n",((Texture *) abr->pere->object)->width,((Texture *) abr->pere->object)->height);
      *width = ((Texture *) abr->pere->object)->width;
      *height = ((Texture *) abr->pere->object)->height;
    }else if(abr->pere->type == BACKGROUND){
      *height = backgroundHeight(*((BackgroundTexture *) abr->pere->object));
      *width = backgroundWidth(*((BackgroundTexture *) abr->pere->object));
    }else{
      printf("Erreur fils d'un Geometrie\n");
    }
  }else{
    *width = win->widthWin;
    *height = win->heightWin;
  }
}

void drawAbr(Window * win, abrDraw * abr){
  int Width, Height;
  getSizeAbr(win,abr,&Width,&Height);
  //printf("pere %p\n",abr->pere);
  SDL_Rect rectProp;
  //SDL_Texture * TextSave = SDL_GetRenderTarget(win->renderer);
  
  if(abr->pere){
    //TextSave = SDL_GetRenderTarget(win->renderer);
    SDL_SetRenderTarget(win->renderer, ((Texture *)abr->pere->object)->texture->texture);
    printf("Pere %p\n", ((Texture *)abr->pere->object)->texture->texture);
  }else{
    SDL_SetRenderTarget(win->renderer, NULL);
  }

  switch (abr->type){
  case TEXTURE:
    Texture * T = (Texture * ) abr->object;
    rectProp = propRectToRect(abr->pos,Width,Height,T->rect.w,T->rect.h);

    T->rect.x = T->rect.w * ( T->currentAnim % T->nbFramePerLine );
    T->rect.y = T->rect.h * (int) (T->currentAnim / T->nbFramePerLine);
    printf("x: %.2f, y:%.2f, w:%.2f, h:%.2f, dim:%d,%d\n",abr->pos.x,abr->pos.y,abr->pos.w,abr->pos.h,T->width,T->height);
    printf("%d %d %d %d %p\n",T->rect.x,T->rect.y,T->rect.w,T->rect.h,T->texture->texture);
    printf("Draw at %d %d %d %d\n",rectProp.x,rectProp.y,rectProp.w,rectProp.h);
    
    SDL_RenderCopy(win->renderer,T->texture->texture,&(T->rect), &rectProp);
    //SDL_RenderCopy(win->renderer,T->texture->texture,NULL, &rectProp);
    break;

  case BACKGROUND:
    BackgroundTexture * bg = (BackgroundTexture *) abr->object;
    SDL_Rect In = inputRect(*bg);
    printf("BACKGROUND\n");
    rectProp = propRectToRect(abr->pos,Width,Height,In.w,In.h);
    //printf("%d %d %d %d, W:%d, H:%d\n",rectProp.x, rectProp.y, rectProp.w, rectProp.h, Width,Height);
    SDL_RenderCopy(win->renderer,bg->texture,&In, &rectProp);
    break;

  case GEOMETRIE:
    abr->pos.consScale = 0;
    rectProp = propRectToRect(abr->pos,Width,Height,0,0);
    drawGeom(win,*((geometrie *)abr->object),rectProp, Width, Height);
    break;

  default:
    break;
  }

}

void updateTextAbrDraw(abrDraw * a, Window * win){
  if(a){
    if(a->lh){
      updateTextAbrDraw(a->lh, win);
    }
    if(a->reDraw && a->lv && a->active){
      resetDraw(win,a);
      updateTextAbrDraw(a->lv, win);
      if(a->type == TEXTURE){
        SDL_SetRenderTarget(win->renderer,((Texture *)a->object)->texture->texture);
      }
    }
  
    if(a->active){
      printf("====================================================\n");
      drawAbr(win,a);
      a->reDraw = 0;
    }
  }
}

void requestReDraw(Window * win,abrDraw * abr){
  if(abr->pere){
    requestReDraw(win,abr->pere);
  }
  abr->reDraw = 1;
  win->reDraw = 1;
}

void reDrawActive(Window * win, abrDraw ** pere){
  if(*pere){
    if((*pere)->active){
      if((*pere)->lv && ((Texture *)(*pere)->object)->height == win->heightWin && ((Texture *)(*pere)->object)->width == win->widthWin){
        printf("Redraw lv\n");
        reDrawActive(win,&(*pere)->lv);
        (*pere)->reDraw = 1;
        
      }
    }
    if((*pere)->lh){
      reDrawActive(win,&(*pere)->lh);
    }
  }
}

void requestReDrawResize(Window * win){
  win->reDraw = 1;
  reDrawActive(win,&win->abr);
}

void activateAbrRecu(abrDraw * abr, int activate){
  abr->active = activate;
  if(abr->increment){
    abr->increment->active = activate;
  }
  if(abr->lv){
    activateAbrRecu(abr->lv, activate);
  }
  if(abr->lh){
    activateAbrRecu(abr->lh, activate);
  }
}

void activateAbr(abrDraw * abr, int activate){
  abr->active = activate;
  if(abr->increment){
    abr->increment->active = activate;
  }
  if(abr->lv){
    activateAbrRecu(abr->lv,activate);
  }
}