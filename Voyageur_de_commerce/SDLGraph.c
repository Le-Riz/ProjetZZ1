#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "SDL.h"
#include "collision.h"
#include "draw.h"
#include "graph.h"
#include "texture.h"

donnees DonneesPartie;
abrDraw ** Sommets;

void initVerticeTexture(Window * win){

  int nbFrame = 4;
  Texture ** allState = calloc(nbFrame,sizeof(Texture *));

  for(int i=0;i<nbFrame;i++){
    allState[i] = calloc(1,sizeof(Texture));
    allState[i]->height = 500;
    allState[i]->width = 500;
    allState[i]->texture = createEmptyTexture( SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, allState[i]->width, allState[i]->height));
    SDL_SetTextureBlendMode(allState[i]->texture->texture, SDL_BLENDMODE_BLEND);
    allState[i]->nbFramePerLine = 1;
    allState[i]->nbLine = 1;
    allState[i]->rect = rectC(0, 0, allState[i]->width, allState[i]->height);
    allState[i]->waitTime = -1;
    allState[i]->waitCounter = allState[i]->waitTime;
  }

  SDL_SetRenderTarget(win->renderer, allState[0]->texture->texture);
  SDL_SetRenderDrawColor(win->renderer, 250, 250, 250, 255);
  drawFillCircle(win->renderer, pointC(250, 250), 250);
  SDL_RenderPresent(win->renderer);

  SDL_SetRenderTarget(win->renderer, allState[1]->texture->texture);
  SDL_SetRenderDrawColor(win->renderer, 255, 82, 82, 255);
  drawFillCircle(win->renderer, pointC(250, 250), 250);
  SDL_RenderPresent(win->renderer);

  SDL_SetRenderTarget(win->renderer, allState[2]->texture->texture);
  SDL_SetRenderDrawColor(win->renderer, 82, 255, 82, 255);
  drawFillCircle(win->renderer, pointC(250, 250), 250);
  SDL_RenderPresent(win->renderer);

  SDL_SetRenderTarget(win->renderer, allState[3]->texture->texture);
  SDL_SetRenderDrawColor(win->renderer, 82, 82, 255, 255);
  drawFillCircle(win->renderer, pointC(250, 250), 250);
  SDL_RenderPresent(win->renderer);
  
  SDL_SetRenderTarget(win->renderer, NULL);
  saveTexture(win, makeSprite(win, allState, nbFrame, -1));

  for(int i=0;i<nbFrame;i++){
    SDL_DestroyTexture(allState[i]->texture->texture);
    free(allState[i]->texture);
    free(allState[i]);
  }
  free(allState);
}

void actionVertice(int x,int y,collision * c){
  (void) x;
  (void) y;
  if(c->abr->text->currentAnim == 0 || c->abr->text->currentAnim == 1){
    if(DonneesPartie.arrete[*DonneesPartie.lastSommet][c->donnee.sommet] || DonneesPartie.arrete[c->donnee.sommet][*DonneesPartie.lastSommet]){
      c->abr->text->currentAnim = 2;
      addChemin(&DonneesPartie.chemin, c->donnee.sommet);
      *c->donnee.score += 1;
      Sommets[*DonneesPartie.lastSommet]->text->currentAnim = 1;
      *DonneesPartie.lastSommet = c->donnee.sommet;
      requestReDraw(c->win,c->abr);
    }
  }else{
    if(DonneesPartie.chemin->next){
      delChemin(&DonneesPartie.chemin);
      *DonneesPartie.lastSommet = DonneesPartie.chemin->sommet;
      Sommets[*DonneesPartie.lastSommet]->text->currentAnim = 2;
      if(rechChemin(&DonneesPartie.chemin, c->donnee.sommet)){
	c->abr->text->currentAnim = 1;
      }else{
	c->abr->text->currentAnim = 0;
      }
      (*DonneesPartie.score)--;
      requestReDraw(c->win,c->abr);
    }
  }
  if(allSommet(&DonneesPartie.chemin) && c->donnee.sommet == 0){
    percentDraw(c->win);
    freeColLi(c->win->collision);
    c->win->collision[0] = NULL;
  }
  
}

void initDonnees(){
  DonneesPartie.nbSommet = 6+rand()%28;
  DonneesPartie.lastSommet = calloc(1, sizeof(int));
  DonneesPartie.score = calloc(1,sizeof(int));
  DonneesPartie.arrete = calloc(DonneesPartie.nbSommet,sizeof(int *));
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    DonneesPartie.arrete[i] = calloc(DonneesPartie.nbSommet,sizeof(int));
  }
  DonneesPartie.chemin = initChemin();
  genere(DonneesPartie.arrete, 0, DonneesPartie.nbSommet-1);
  genere_Graph(DonneesPartie.arrete, 0.6/DonneesPartie.nbSommet, DonneesPartie.nbSommet);
  *DonneesPartie.score = 0;
  
  *DonneesPartie.lastSommet = 0;
  addChemin(&DonneesPartie.chemin, 0);
}

void freeDonnees(){
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    free(DonneesPartie.arrete[i]);
  }
  free(DonneesPartie.arrete);
  free(DonneesPartie.score);
  free(DonneesPartie.lastSommet);
}

void createVertice(Window * win, propRect pRect, int indice){
  int width, height;
  SDL_GetWindowSize(win->win, &width, &height);
  SDL_Rect rect = propRectToRect(pRect, width, height);
  int rayon = rect.w/2.0;
  void * circle = circleColC(rect.x+rayon, rect.y+rayon, rayon);
  abrDraw * abr = createAbrDraw(NULL, NULL, NULL, loadTexture(win->text), pRect, 0);
  addAbrDraw(&(win->abr), abr);
  donnees newDonnees = DonneesPartie;
  newDonnees.sommet = indice;
  collision c = generCol(win, abr, circle, Circle, &actionVertice, newDonnees);
  addColLi(win->collision, c);
}

void createAllVertice(Window * win){
  int width,height;
  Sommets = calloc(DonneesPartie.nbSommet,sizeof(abrDraw *));
  SDL_GetWindowSize(win->win, &width, &height);
  propRect P = propRectC(0, 0, 80/DonneesPartie.nbSommet, 80/DonneesPartie.nbSommet, 0, 0, 1);
  int radius = 0.5*height-0.15*height;
  float StartAngle = rand()%360;
  float a = toRad(StartAngle);
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    a += toRad(360.0f/DonneesPartie.nbSommet);
    int x = cosf(a)*radius;
    int y = sinf(a)*radius;
    P.x = (x - P.w*height/200 + 0.5*width) / width * 100;
    P.y = (y - P.h*height/200 + 0.5*height) / height * 100;
    createVertice(win, P, i);
    Sommets[i] = win->abr;
  }
  Sommets[0]->text->currentAnim = 2;
}

cheminLi * initChemin(){
  return NULL;
}

void freeChemin(cheminLi ** pprec){
  if(*pprec){
    freeChemin(&(*pprec)->next);
    free(*pprec);
  }
}

void addChemin(cheminLi ** pere, int sommet){
  cheminLi * ch = calloc(1,sizeof(cheminLi));
  ch->sommet = sommet;
  ch->next = *pere;
  *pere = ch;
}

void delChemin(cheminLi ** pprec){
  if(*pprec){
    cheminLi * save = *pprec;
    *pprec = save->next;
    free(save);
  }
}

int rechChemin(cheminLi ** pere,int sommet){
  if(*pere && (*pere)->sommet == sommet){
    return 1;
  }
  if(!*pere){
    return 0;
  }
  return rechChemin(&(*pere)->next, sommet);
}

int allSommet(cheminLi ** pere){
  int * sommets = calloc(DonneesPartie.nbSommet,sizeof(int));
  while(*pere){
    sommets[(*pere)->sommet] ++;
    pere=&(*pere)->next;
  }
  int boolean=1;
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    boolean = boolean && sommets[i];
  }
  free(sommets);
  return boolean;
}

void drawEdge(Window * win){
  int width, height;
  SDL_GetWindowSize(win->win, &width, &height);
  int decal = Sommets[0]->pos.w*width/200;
  SDL_SetRenderDrawColor(win->renderer, 250, 250, 250, 255);
  
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    SDL_Point Dep = pointC(Sommets[i]->pos.x*width/100+decal, Sommets[i]->pos.y*height/100+decal);
    for(int j=0;j<i;j++){
      if(DonneesPartie.arrete[i][j] || DonneesPartie.arrete[j][i]){
	SDL_Point Arr = pointC(Sommets[j]->pos.x*width/100+decal, Sommets[j]->pos.y*height/100+decal);
	SDL_RenderDrawLine(win->renderer, Dep.x, Dep.y, Arr.x, Arr.y);
      }
    }
  }
  SDL_SetRenderDrawColor(win->renderer, 255, 82, 82, 255);
  if(DonneesPartie.chemin){
    cheminLi ** ch = &DonneesPartie.chemin->next;
    int lastSommet = DonneesPartie.chemin->sommet;
    SDL_Point dep = pointC(Sommets[lastSommet]->pos.x*width/100+decal, Sommets[lastSommet]->pos.y*height/100+decal);
    while(*ch){
      SDL_Point arr = pointC(Sommets[(*ch)->sommet]->pos.x*width/100+decal, Sommets[(*ch)->sommet]->pos.y*height/100+decal);
      SDL_RenderDrawLine(win->renderer, dep.x, dep.y, arr.x, arr.y);
      ch = &((*ch)->next);
      dep.x = arr.x;
      dep.y = arr.y;
    }
  }
}


void percentDraw(Window * win){
  
  int** res = calloc(DonneesPartie.nbSommet,sizeof(int*));
  for(int i=0;i<DonneesPartie.nbSommet;i++){
    res[i] = calloc(DonneesPartie.nbSommet,sizeof(int));
  }
  float dist;
  int* chemin = calloc(DonneesPartie.nbSommet,sizeof(int));
  creationGprime(DonneesPartie.arrete,res,DonneesPartie.nbSommet);
  min_fourmi(res,&dist,chemin,50,50.00,DonneesPartie.nbSommet);

  float percentDiff = dist/(float)*DonneesPartie.score * 100;
  SDL_Rect rect = rectC(0, 0, 450,0);
  char * percent = calloc(128,sizeof(char));
  sprintf(percent, "Vous avez %.2f %% de ressemblance avec\nla solution jugee optimale",percentDiff);
  strDraw(win, rect, colorC(82, 82, 255, 255), percent);

  for(int i=0;i<DonneesPartie.nbSommet;i++){
    free(res[i]);
  }
  free(res);
  free(chemin);
}
