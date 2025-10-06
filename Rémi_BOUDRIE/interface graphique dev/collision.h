#ifndef __COLLISION_H__
#define __COLLISION_H__

typedef struct colLi colLi;
typedef struct collision collision;

#include "SDL.h"
#include "draw.h"

typedef enum{
  Box,
  Circle
} collisionType;

typedef struct circleData{
  float x;
  float y;
  int r;
} circleData;

typedef struct boxData{
  float x;
  float y;
  float w;
  float h;
  float a;
} boxData;

typedef struct collision{
  void * data;
  collisionType type;
  abrDraw ** abr;
  int abrNb;
  Window * win;
  unsigned char active : 1;
  struct collision ** linkedColli;
  int nbCollision;
  //donnees donnee;
  int (*check)(int x,int y, void * data, Window * win);
  void (*action)(int x,int y,struct collision * c);
} collision;

typedef struct colLi{
  collision * col;
  struct colLi * next;
} colLi;

void * circleColC(float x, float y, int r);

int checkCircle(int x, int y, void * data, Window * win);

void * boxColC(float x, float y, float w, float h, float a);

int checkBox(int x, int y, void * data, Window * win);

int (*genCheck(collisionType c))(int x, int y, void * data, Window * win);

void checkAllCol(colLi ** col, int x, int y);

collision generCol(Window * win, abrDraw ** abr, int nbAbr, void * data, int active, collision ** linkedColli, int nbCollision, collisionType type,void (*action)(int x,int y,struct collision * c)/*,donnees d*/);

colLi * initcolLi();

collision * addColLi(colLi ** tete, collision col);

void delColLi(colLi ** pprec);

void freeColLi(colLi ** pprec);

#endif
