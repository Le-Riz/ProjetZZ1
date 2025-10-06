#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "SDL.h"
#include "draw.h"
#include "collision.h"
#include "SDLGraph.h"
#include "graph.h"

#define FPS 1000/60


int main(int argc, char ** argv){
  (void) argc;
  (void) argv;
  
  srand(time(NULL));
  Window * win = initSDL(pointC(0, 0), 720, 540, "Voyageur","consola.ttf");
  initVerticeTexture(win);
  initDonnees();
  createAllVertice(win);
  
  int program_on = 1;

  while(program_on){
    Uint32 time = getTime();
    
    eventLoop(win, &program_on);
    loopCalc(win, &program_on);
    
    fpsFix(time, FPS);
  }/*

  srand(time(NULL));

  int i;

  int** res = calloc(T,sizeof(int*));
  int** mat = calloc(T,sizeof(int*));
  for(i=0;i<T;i++)
  {
    res[i] = calloc(T,sizeof(int));
    mat[i] = calloc(T,sizeof(int));
  }

  mat[0][1] = 1;
  mat[0][3] = 1;
  mat[0][4] = 1;
  mat[1][2] = 1;
  mat[2][5] = 1;
  mat[3][6] = 1;
  mat[4][5] = 1;
  mat[4][6] = 1;
  mat[6][7] = 1;
  mat[5][8] = 1;
  mat[7][8] = 1;

  
  creationGprime(mat,res);

  float dist;
  int* chemin = calloc(T,sizeof(int));

  min_fourmi(res,&dist,chemin);

  printf("%f\n",dist);
  
  for(i=0;i<T;i++)
  {
    printf("%d ",chemin[i]);
    free(res[i]);
    free(mat[i]);
  }
  printf("\n");
  
  free(res);
  free(mat);
  free(chemin);*/
  
  freeSDL(win);
  freeDonnees();

  return 0;
}
