#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.h"
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
  SDL_RenderPresent(win->renderer);
  SDL_SetRenderTarget(win->renderer, NULL);
  
  return new;
}

int saveTexture(Window * win,Texture * text){
  win->text[win->lastText].texture = text->texture->texture;
  win->text[win->lastText].height = text->height;
  win->text[win->lastText].width = text->width;
  win->text[win->lastText].nbFramePerLine = text->nbFramePerLine;
  win->text[win->lastText].nbLine = text->nbLine;
  win->text[win->nbText].waitTime = text->waitTime;
  free(text->texture);
  text->texture = win->text + win->lastText;
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

Texture * loadTexture(TextureSave * text){
  Texture * out = calloc(1,sizeof(Texture));
  out->height = text->height;
  out->width = text->width;
  out->nbFramePerLine = text->nbFramePerLine;
  out->nbLine = text->nbLine;
  out->texture = text;
  out->waitTime = text->waitTime;
  out->waitCounter = out->waitTime;
  out->rect = rectC(0,0,(float)out->width/(float)out->nbFramePerLine,(float)out->height/(float)out->nbLine);
  out->scale = (float)out->rect.w/(float)out->rect.h;
  return out;
}

void strDraw(Window * win, SDL_Rect rect, SDL_Color color, char * str){
  SDL_Surface * surface = TTF_RenderText_Blended_Wrapped(win->font, str, color,0);
  if (surface == NULL){
    SDL_Log("Can't create text surface, %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  SDL_Texture * strText = SDL_CreateTextureFromSurface(win->renderer, surface);
  int strTextW, strTextH, width,height;
  SDL_GetWindowSize(win->win, &width, &height);
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
  abrDraw * abr = createAbrDraw(NULL, NULL, NULL, text, propRectC(rect.x/(float)width*100., rect.y/(float)height*100., rect.w/(float)width*100., rect.h/(float)height*100., 0, 0, 0), 0);
  addAbrDraw(&win->abr, abr);
  requestReDraw(win, abr);
  
}

