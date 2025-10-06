#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "collision.h"
#include "SDL.h"

void * circleColC(float x, float y, int r){
  circleData * c = calloc(1,sizeof(circleData));
  c->x = x;
  c->y = y;
  c->r = r;
  return c;
}

int checkCircle(int x, int y, void * data, Window * win){
  (void) win; // Pas eu le temps de mettre en place en fonction de la taille de la fenetre
  circleData * d = (circleData *) data;
  //printf("data %p\n",d);
  double dist = sqrt((x-d->x)*(x-d->x) + (y-d->y)*(y-d->y));
  if(dist <= d->r){
    return 1;
  }
  return 0;
}

void * boxColC(float x, float y, float w, float h, float a){
  boxData * b = calloc(1,sizeof(boxData));
  b->x = x;
  b->y = y;
  b->w = w;
  b->h = h;
  b->a = a;
  return b;
}

int checkBox(int x, int y, void * data, Window * win){
  boxData * d = (boxData *) data;
  float NewX,NewY,NewW,NewH;
  NewX = d->x * (float)win->widthWin/100.;
  NewY = d->y * (float)win->heightWin/100.;
  NewW = d->w * (float)win->widthWin/100.;
  NewH = d->h * (float)win->heightWin/100.;
  float tx = (float)x - NewX;
  float ty = (float)y - NewY;
  float rx = tx * cosf(toRad(-d->a+0)) + ty * sinf(toRad(-d->a+0));
  float ry = -tx * sinf(toRad(-d->a+0)) + ty * cosf(toRad(-d->a+0));
  if(rx/NewW >=0 && rx/NewW <= 1 && ry/NewH >=0 && ry/NewH <= 1){
    return 1;
  }
  return 0;
};

int (*genCheck(collisionType c))(int x, int y, void * data, Window * win){
  if(c == Circle){
    return &checkCircle;
  } else{
    return &checkBox;
  }
}

void checkAllCol(colLi ** col, int x, int y){
  if(*col){
    if(*col && (*col)->next){
      checkAllCol(&((*col)->next), x, y);
    }
    if(*col && (*col)->col && (*col)->col->active){
      if((*col)->col && (*col)->col->check && (*col)->col->data){
	      if((*col)->col->check(x,y,(*col)->col->data, (*col)->col->win)){
	        (*col)->col->action(x,y,(*col)->col);
	      }
      }
    }
  }
}

collision generCol(Window * win, abrDraw ** abr, int nbAbr, void * data, int active, collision ** linkedColli, int nbCollision, collisionType type,void (*action)(int x,int y,struct collision * c)/*, donnees d*/){
  collision c;
  c.data = data;
  c.type = type;
  c.win = win;
  c.abr = abr;
  c.abrNb = nbAbr;
  c.action = action;
  c.check = genCheck(type);
  c.linkedColli = linkedColli;
  c.nbCollision = nbCollision;
  //c.donnee = d;
  c.active = active;
  
  return c;
}

colLi * initcolLi(){
  return NULL;
}

collision * addColLi(colLi ** tete, collision col){
  colLi * new = calloc(1,sizeof(colLi));
  new->col = calloc(1,sizeof(collision));

  new->col->abr = col.abr;
  new->col->action = col.action;
  new->col->check = col.check;
  new->col->data = col.data;
  new->col->type = col.type;
  new->col->win = col.win;
  new->col->active = col.active;
  new->col->abrNb = col.abrNb;
  new->col->linkedColli = col.linkedColli;
  new->col->nbCollision = col.nbCollision;
  //new->col->donnee = col.donnee;
  
  new->next = *tete;
  *tete = new;

  return new->col;
}

void delColLi(colLi ** pprec){
  colLi * save = *pprec;
  *pprec = save->next;
  free(save->col);
  free(save);
}

void freeColLi(colLi ** pprec){
  if(*pprec){
    if((*pprec)->next){
      freeColLi(&(*pprec)->next);
    }
    free((*pprec)->col->data);
    (*pprec)->col->data = NULL;
    free((*pprec)->col);
    (*pprec)->col = NULL;
    free(*pprec);
    *pprec = NULL;
  }
}
