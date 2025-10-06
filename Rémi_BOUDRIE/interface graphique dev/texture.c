#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.h"
#include "draw.h"
#include "SDL.h"


Texture * makeSprite(Window * win, Texture ** allSprite,int number, int waitTime){
  Texture * new = calloc(1, sizeof(Texture));
  new->texture = calloc(1,sizeof(TextureSave *));
  new->nbLine = 1;
  new->waitTime = waitTime;
  new->waitCounter = waitTime;
  int maxWidth=0;

  for(int i=0;i<number;i++){
    int curWidth = allSprite[i]->width, curHeight = allSprite[i]->height;

    if(allSprite[i]->nbFramePerLine != 1){
      curWidth = allSprite[i]->width/allSprite[i]->nbFramePerLine;
    }
    if(allSprite[i]->nbLine != 1){
      curHeight = allSprite[i]->height/allSprite[i]->nbLine;
    }
    if(maxWidth < curWidth){
      maxWidth = curWidth;
    }
    if(new->height < curHeight){
      new->height = curHeight;
    }

    new->nbFramePerLine += allSprite[i]->nbFramePerLine*allSprite[i]->nbLine;
  }
  new->width = maxWidth*new->nbFramePerLine;
  
  new->texture->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, new->width, new->height);
  SDL_SetTextureBlendMode(new->texture->texture, SDL_BLENDMODE_BLEND);
  new->rect.x = 0;
  new->rect.y = 0;
  new->rect.h = new->height;
  new->rect.w = maxWidth;
  new->scale = maxWidth/new->height;

  SDL_SetRenderTarget(win->renderer, new->texture->texture);
  for(int k=0; k<number;k++){
    allSprite[k]->currentAnim = 0;
    
    for(int i=0; i<allSprite[k]->nbLine; i++){
      allSprite[k]->rect.y = allSprite[k]->rect.h*(int)(allSprite[k]->currentAnim/allSprite[k]->nbFramePerLine);
      for(int j=0; j<allSprite[k]->nbFramePerLine;j++){
	      allSprite[k]->rect.x = allSprite[k]->rect.w*(allSprite[k]->currentAnim%allSprite[k]->nbFramePerLine);
	      new->rect.x = new->rect.w * (new->currentAnim);

	      //printf("Ori: %d %d %d %d, Dest: %d %d %d %d\n",allSprite[k]->rect.x,allSprite[k]->rect.y,allSprite[k]->rect.w,allSprite[k]->rect.h, new->rect.x,new->rect.y,new->rect.w,new->rect.h);
	
	      SDL_RenderCopy(win->renderer, allSprite[k]->texture->texture, &(allSprite[k]->rect), &(new->rect));
	
	      allSprite[k]->currentAnim++;
	      new->currentAnim++;
      }
      
    }
    
    allSprite[k]->currentAnim = 0;
    allSprite[k]->rect.x = 0;
    allSprite[k]->rect.y = 0;
  }
  new->currentAnim = 0;
  new->rect.x = 0;
  new->rect.y = 0;
  //SDL_RenderPresent(win->renderer);
  //SDL_SetRenderTarget(win->renderer, NULL);
  
  return new;
}

int saveTexture(Window * win,Texture * text){
  win->text[win->lastText].texture = text->texture->texture;
  printf("Texture pendant save %p, %p\n",text->texture->texture,win->text[win->lastText].texture);
  win->text[win->lastText].height = text->height;
  win->text[win->lastText].width = text->width;
  win->text[win->lastText].nbFramePerLine = text->nbFramePerLine;
  win->text[win->lastText].nbLine = text->nbLine;
  win->text[win->lastText].waitTime = text->waitTime;
  free(text->texture);
  text->texture = &(win->text[win->lastText]);
  win->lastText++;
  return win->lastText-1;
}

int checkTextureSave(Window * win, TextureSave * text){
  int i=0;
  while(i<win->nbText && text->texture != win->text[i].texture){
    i++;
  }
  return text->texture == win->text[i].texture;
}

TextureSave * createEmptyTexture(SDL_Texture * text){
  TextureSave * out = calloc(1,sizeof(TextureSave));
  out->texture = text;
  return out;
}

void freeTexture(Window * win,TextureSave * text){
  if(text && text->texture){
    SDL_DestroyTexture(text->texture);
    text->texture = NULL;
    if(!checkTextureSave(win, text)){
      free(text);
    }
  }
}

Texture * loadTexture(Window * win, int id){
  printf("========LOAD TEXTUR========\n");
  Texture * out = calloc(1,sizeof(Texture));
  out->height = win->text[id].height;
  printf("Height %d, ",out->height);
  out->width = win->text[id].width;
  printf("Width %d, ",out->width);
  out->nbFramePerLine = win->text[id].nbFramePerLine;
  printf("nbFramePerLine %d, ",out->nbFramePerLine);
  out->nbLine = win->text[id].nbLine;
  printf("nbLine %d, ",out->nbLine);
  out->texture = &(win->text[id]);
  printf("texture %p, ",out->texture->texture);
  out->waitTime = win->text[id].waitTime;
  printf("waitTime %d, ",out->waitTime);
  out->waitCounter = out->waitTime;
  printf("waitCounter %d, ",out->waitCounter);
  out->rect = rectC(0,0,(float)out->width/(float)out->nbFramePerLine,(float)out->height/(float)out->nbLine);
  printf("rect (%d,%d) %d %d, ",out->rect.x,out->rect.y,out->rect.w,out->rect.h);
  out->scale = (float)out->rect.w/(float)out->rect.h;
  printf("scale %f\n",out->scale);
  return out;
}

void strDraw(Window * win, abrDraw * pere,SDL_Rect rect, SDL_Color color, char * str){
  SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(win->font, str, color,0);
  if (surface == NULL){
    SDL_Log("Can't create text surface, %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_Texture * strText = SDL_CreateTextureFromSurface(win->renderer, surface);
  int strTextW, strTextH, width,height;
  //SDL_GetWindowSize(win->win, &width, &height);
  getSizeAbr(win,pere->lv,&width,&height);
  SDL_QueryTexture(strText, NULL, NULL, &strTextW, &strTextH);
  if (strText == NULL){
    SDL_Log("Can't create texture from surface, %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  rect.h = rect.w * strTextH / strTextW;
  //SDL_RenderCopy(win->renderer, strText, NULL, &rect);
  Texture * text = calloc(1,sizeof(Texture));
  text->currentAnim = 0;
  text->height = strTextH;
  text->width = strTextW;
  text->nbFramePerLine = 1;
  text->nbLine = 1;
  text->rect = rectC(0, 0, strTextW, strTextH);
  text->scale = (float)strTextW / (float)strTextH;
  text->texture = createEmptyTexture(strText);
  text->waitTime = -1;
  text->waitCounter = -1;
  abrDraw * abr = createAbrDraw(pere, NULL, NULL, TEXTURE, text, propRectC(rect.x/(float)width*100., rect.y/(float)height*100., rect.w/(float)width*100., rect.h/(float)height*100., 0, 0, 1), 1,1,NULL);
  printf("%d %d Str %f %f %f %f\n",width,height,abr->pos.x,abr->pos.y,abr->pos.w,abr->pos.h);
  if(pere){
    addAbrDraw(&pere->lv, abr);
  }else{
    addAbrDraw(&win->abr, abr);
  }
  //requestReDraw(win, abr);
  
}


/// @brief 
/// @param win
/// @param fils addresse du père à qui ont veut attribuer la texture
/// @param width longueur de la texture si -1 longueur du père
/// @param height largeur de la texture si -1 largeur du père
/// @return addresse de la texture crée
Texture * createPereTexture(Window * win,abrDraw * fils,int width, int height){
  Texture * out = calloc(1,sizeof(Texture));
  out->currentAnim = 0;
  out->nbFramePerLine = 1;
  out->nbLine = 1;
  out->waitCounter = -1;
  out->waitTime = -1;
  if(width == -1 || height == -1){
    getSizeAbr(win,fils,&out->width,&out->height);
    if(width != -1){
      out->width = width;
    }else if(height != -1){
      out->height = height;
    }
  }else{
    out->width = width;
    out->height = height;
  }
  
  out->texture = createEmptyTexture(SDL_CreateTexture(win->renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,out->width,out->height));
  out->rect = rectC(0,0,out->width,out->height);
  out->scale = (float)out->width/(float)out->height;

  return out;
}

Texture * createTexture(SDL_Texture * texture, SDL_Rect rect, int width, int height, int currentAnim, int nbFramePerLine, int nbLine, int waitTime, int waitCounter){
  Texture * out = calloc(1,sizeof(Texture));
  out->texture = createEmptyTexture(texture);
  out->rect = rect;
  out->width = width;
  out->height = height;
  out->scale = (float)width/(float)nbFramePerLine/(float)height/(float)nbLine;
  out->currentAnim = currentAnim;
  out->nbFramePerLine = nbFramePerLine;
  out->nbLine = nbLine;
  out->waitTime = waitTime;
  out->waitCounter = waitCounter;
  return out;
}

Increment * createIncrement(int * variable, int wait, int increment, int stop, int origine, int reboot, abrDraw * abr, int active){
  Increment * inc = calloc(1,sizeof(Increment));
  inc->next = NULL;
  inc->var = variable;
  inc->wait = wait;
  inc->waitOri = wait;
  inc->increment = increment;
  inc->origine = origine;
  inc->stop = stop;
  inc->reboot = reboot;
  inc->abr = abr;
  inc->active = active;
  return inc;
}

void freeIncrement(Increment ** inc){
  if((*inc)){
    if((*inc)->next){
      freeIncrement(&(*inc)->next);
    }
    free((*inc));
  }
}

void addIncrement(Increment ** pprec, Increment * cour){
  cour->next = *pprec;
  *pprec = cour;
}

void delIncrement(Increment ** pprec){
  if(*pprec){
    Increment * save = *pprec;
    *pprec = save->next;
    free(save);
  }
}

void evalIncrement(Window * win, Increment ** pprec){
  if(*pprec){
    if((*pprec)->active){
      if((*pprec)->wait == 0){
        requestReDraw(win,(*pprec)->abr);
        (*pprec)->wait = (*pprec)->waitOri;
        if(*(*pprec)->var == (*pprec)->stop){
          if((*pprec)->reboot){
            *((*pprec)->var) = (*pprec)->origine;
          }else{
            delIncrement(pprec);
          }
        }else{
          *((*pprec)->var) += (*pprec)->increment;
        }
      }else{
        //printf("wait %d\n",(*pprec)->wait);
        (*pprec)->wait--;
      }
    }
    evalIncrement(win,&(*pprec)->next);
  }
}