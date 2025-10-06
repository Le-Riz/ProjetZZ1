#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "SDL.h"
#include "draw.h"
#include "collision.h"
#include "geometrie.h"
#include "Menu.h"
#include "game.h"

#define FPS 1000/60


int main(int argc, char ** argv){
  (void) argc;
  (void) argv;
  
  srand(time(NULL));
  Window * win = initSDL(pointC(0, 0), 1366, 704, "Limule's Adventure","consola.ttf","icon.png");
  //printf("Boucle %d \n",win->program_on);
  initStartingScreen(win);
  initGameTexture(win);

  while(win->program_on){
    Uint32 time = getTime();
    
    eventLoop(win);
    loopCalc(win);
    
    fpsFix(time, FPS);
  }
  freeSDL(win);

  return 0;
}
