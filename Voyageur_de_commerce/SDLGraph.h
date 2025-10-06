#ifndef __SDLGRAPH_H__
#define __SDLGRAPH_H__

typedef struct cheminLi{
  int sommet;
  struct cheminLi * next;
} cheminLi;

typedef struct donnees{
  int sommet;
  int * lastSommet;
  int nbSommet;
  int ** arrete;
  cheminLi * chemin;
  int * score;
} donnees;

#include "collision.h"
#include "draw.h"
#include "SDL.h"
#include "texture.h"


extern donnees DonneesPartie;
extern abrDraw ** Sommets;

void initVerticeTexture(Window * win);

void actionVertice(int x,int y,collision * c);

void initDonnees();

void freeDonnees();

void createVertice(Window * win, propRect pRect, int indice);

void createAllVertice(Window * win);

cheminLi * initChemin();

void freeChemin(cheminLi ** pprec);

void addChemin(cheminLi ** pere, int sommet);

void delChemin(cheminLi ** pprec);

int rechChemin(cheminLi ** pere,int sommet);

int allSommet(cheminLi ** pere);

void drawEdge(Window * win);

void percentDraw(Window * win);

#endif
