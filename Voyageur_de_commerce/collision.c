#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "collision.h"
#include "SDL.h"

void * circleColC(int x, int y, int r){
  circleData * c = calloc(1,sizeof(circleData));
  c->x = x;
  c->y = y;
  c->r = r;
  return c;
}

int checkCircle(int x, int y, void * data){
  circleData * d = (circleData *) data;
  //printf("data %p\n",d);
  double dist = sqrt((x-d->x)*(x-d->x) + (y-d->y)*(y-d->y));
  if(dist <= d->r){
    return 1;
  }
  return 0;
}

void * boxColC(int x, int y, int w, int h, float a){
  boxData * b = calloc(1,sizeof(boxData));
  b->x = x;
  b->y = y;
  b->w = w;
  b->h = h;
  b->a = a;
  return b;
}

int checkBox(int x, int y, void * data){
  boxData * d = (boxData *) data;
  float tx = x - d->x;
  float ty = y - d->y;
  float rx = tx * cosf(toRad(-d->a)) + ty * sinf(toRad(-d->a));
  float ry = -tx * sinf(toRad(-d->a)) + ty * cosf(toRad(-d->a));
  if(rx/d->w >=0 && rx/d->w <= 1 && ry/d->h >=0 && ry/d->h <= 1){
    return 1;
  }
  return 0;
};

int (*genCheck(collisionType c))(int x, int y, void * data){
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
    if(col && *col){
      if((*col)->col && (*col)->col->check && (*col)->col->data){
	if((*col)->col->check(x,y,(*col)->col->data)){
	  (*col)->col->action(x,y,(*col)->col);
	}
      }
    }
    
  }
}

collision generCol(Window * win, abrDraw * abr, void * data,collisionType type,void (*action)(int x,int y,struct collision * c), donnees d){
  collision c;
  c.data = data;
  c.type = type;
  c.win = win;
  c.abr = abr;
  c.action = action;
  c.check = genCheck(type);
  c.donnee = d;
  
  return c;
}

colLi * initcolLi(){
  return NULL;
}

void addColLi(colLi ** tete, collision col){
  colLi * new = calloc(1,sizeof(colLi));
  new->col = calloc(1,sizeof(collision));

  new->col->abr = col.abr;
  new->col->action = col.action;
  new->col->check = col.check;
  new->col->data = col.data;
  new->col->type = col.type;
  new->col->win = col.win;
  new->col->donnee = col.donnee;
  
  new->next = *tete;
  *tete = new;
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
