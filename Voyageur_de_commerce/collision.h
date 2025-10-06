#ifndef __COLLISION_H__
#define __COLLISION_H__

typedef struct colLi colLi;
typedef struct collision collision;

#include "SDL.h"
#include "draw.h"
#include "SDLGraph.h"

typedef enum{
  Box,
  Circle
} collisionType;

typedef struct circleData{
  int x;
  int y;
  int r;
} circleData;

typedef struct boxData{
  int x;
  int y;
  int w;
  int h;
  float a;
} boxData;

typedef struct collision{
  void * data;
  collisionType type;
  abrDraw * abr;
  Window * win;
  donnees donnee;
  int (*check)(int x,int y, void * data);
  void (*action)(int x,int y,struct collision * c);
} collision;

typedef struct colLi{
  collision * col;
  struct colLi * next;
} colLi;

void * circleColC(int x, int y, int r);

int checkCircle(int x, int y, void * data);

void * boxColC(int x, int y, int w, int h, float a);

int checkBox(int x, int y, void * data);

int (*genCheck(collisionType c))(int x, int y, void * data);

void checkAllCol(colLi ** col, int x, int y);

collision generCol(Window * win, abrDraw * abr, void * data,collisionType type,void (*action)(int x,int y,struct collision * c),donnees d);

colLi * initcolLi();

void addColLi(colLi ** tete, collision col);

void delColLi(colLi ** pprec);

void freeColLi(colLi ** pprec);

#endif
