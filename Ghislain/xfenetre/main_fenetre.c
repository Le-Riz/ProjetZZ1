#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

srand(time(NULL));

if (SDL_Init(SDL_INIT_VIDEO) != 0) // Initialisation SDL
{
  SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
  exit(EXIT_FAILURE);
}


SDL_Window ** mob = calloc(5,sizeof(SDL_Window*)); // Creation pnjs
SDL_Renderer** mob_color = calloc(5,sizeof(SDL_Renderer*));

int** pos_mob = calloc(5,sizeof(int*));

int i;
for(i=0;i<5;i++)
{
  int h = rand()%590+50;
  int l = rand()%1200+50;
  mob[4-i] = SDL_CreateWindow("Player",l,h,40+20*(4-i),40+20*(4-i),SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN);

  mob_color[4-i] = SDL_CreateRenderer(mob[4-i],-1,SDL_RENDERER_ACCELERATED);
  int r = rand()%255;
  int g = rand()%255;
  int b = rand()%255;
  SDL_SetRenderDrawColor(mob_color[4-i],r,g,b,255);
  SDL_RenderClear(mob_color[4-i]);
  SDL_RenderPresent(mob_color[4-i]);

  pos_mob[4-i] = calloc(3,sizeof(int));
  pos_mob[4-i][0] = 1;
  pos_mob[4-i][1] = h;
  pos_mob[4-i][2] = l;

}

SDL_Window * window_player = NULL; // Creation joueur

window_player = SDL_CreateWindow("Player",500,300,50,50,SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);

if (window_player == NULL)
{
  SDL_Log("Error : SDL window player creation - %s\n",SDL_GetError());
  SDL_Quit();      
  exit(EXIT_FAILURE);
}
SDL_Renderer* player = SDL_CreateRenderer(window_player,-1,SDL_RENDERER_ACCELERATED);
SDL_SetRenderDrawColor(player,0,255,0,255);
SDL_RenderClear(player);
SDL_RenderPresent(player);


SDL_bool program_on = SDL_TRUE;       // Gestion des evenments
SDL_Event event;                           

while (program_on){

  if (SDL_PollEvent(&event)){

    switch(event.type){                       
      case SDL_KEYDOWN :
        int x;
        int y;
        SDL_GetWindowPosition(window_player,&x,&y);
        switch(event.key.keysym.scancode)
        {
          case SDL_SCANCODE_DOWN :
            y += 10;
            SDL_SetWindowPosition(window_player,x,y);
            break;
          case SDL_SCANCODE_UP :
            y -= 10;
            SDL_SetWindowPosition(window_player,x,y);
            break;
          case SDL_SCANCODE_LEFT :
            x -= 10;
            SDL_SetWindowPosition(window_player,x,y);
            break;
          case SDL_SCANCODE_RIGHT :
            x += 10;
            SDL_SetWindowPosition(window_player,x,y);
            break;
          case SDL_SCANCODE_ESCAPE :
            program_on = SDL_FALSE;
            break;
          default:
            break;
        }
        int h_player;
        int l_player;
        SDL_GetWindowSize(window_player,&h_player,&l_player);
        int xcentreplayer = x+l_player/2;
        int ycentreplayer = y+h_player/2;

        for(i=0;i<5;i++)
        {
          int ycentremob = pos_mob[4-i][1] + 20 + 10*(4-i);
          int xcentremob = pos_mob[4-i][2] + 20 + 10*(4-i);

          if((xcentremob-xcentreplayer)*(xcentremob-xcentreplayer)+(ycentremob-ycentreplayer)*(ycentremob-ycentreplayer) <(20+10*(4-i))*(20+10*(4-i)))
          {
            if (pos_mob[4-i][0])
            {
              if (40+20*(4-i) < h_player)
              {
                pos_mob[4-i][0] = 0;
                SDL_SetWindowSize(window_player,h_player+20,l_player+20);
                SDL_DestroyRenderer(mob_color[4-i]);
                SDL_DestroyWindow(mob[4-i]);
                SDL_SetRenderDrawColor(player,0,255,0,255);
                SDL_RenderClear(player);
                SDL_RenderPresent(player);
              }
              else
              {
                program_on = SDL_FALSE;
              }
            }
          }
        }
        break;

    default:  
      break;
        }
      }
    } 

SDL_DestroyRenderer(player);
SDL_DestroyWindow(window_player);
for(i=0;i<5;i++)
{
  if(pos_mob[4-i][0])
  {
    SDL_DestroyRenderer(mob_color[i]);
    SDL_DestroyWindow(mob[i]);
  }
  free(pos_mob[4-i]);
}
free(pos_mob);

SDL_Quit();

return 0;
}