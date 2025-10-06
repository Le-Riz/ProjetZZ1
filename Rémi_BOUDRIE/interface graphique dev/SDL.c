#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#include "SDL.h"
#include "collision.h"
#include "geometrie.h"

void init_window(SDL_Window ** window, int posX, int posY, int width, int height, char * nom,SDL_WindowFlags SDL_Window_Flag){
  *window = SDL_CreateWindow(
       nom,                    // codage en utf8, donc accents possibles
       posX, posY,                                  // coin haut gauche en haut gauche de l'écran
       width, height,                              // largeur = width, hauteur = height
       SDL_Window_Flag);

  if (*window == NULL) {
     SDL_Log("Error : SDL window 1 creation - %s\n", 
                 SDL_GetError());                 // échec de la création de la fenêtre
     SDL_Quit();                              // On referme la SDL
     exit(EXIT_FAILURE);
       }
}

Window * initSDL(SDL_Point p, int w, int h, char * nom, char * font, char * icon){
  Window * win = calloc(1,sizeof(Window));
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
	    SDL_GetError()); 
    exit(EXIT_FAILURE);
  }
  if (IMG_Init(IMG_INIT_PNG) == 0){
    SDL_Log("Error : IMG initialisation - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  if (TTF_Init() < 0){
    SDL_Log("Couldn't initialize SDL TTF, %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  
  SDL_Window * taille;
  init_window(&taille, 0, 0, 0, 0, "", SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN | SDL_WINDOW_SKIP_TASKBAR);
  SDL_GetWindowSize(taille, &(win->widthScreen), &(win->heightScreen));
  SDL_DestroyWindow(taille);

  init_window(&(win->win), p.x, p.y, w, h, nom, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  SDL_Surface * iconSurf = IMG_Load(icon);
  SDL_SetWindowIcon(win->win,iconSurf);
  SDL_FreeSurface(iconSurf);
  win->widthWin = w;
  win->heightWin = h;

  win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);

  win->nbText = 128;
  win->lastText = 0;
  win->text = calloc(128,sizeof(TextureSave));
  win->abr = initAbrDraw();
  win->collision = calloc(16, sizeof(colLi *));
  for(int i=0;i<16;i++){
    win->collision[i] = initcolLi();
    
  }
  win->nbCol = 0;
  win->reDraw = 1;
  win->font = TTF_OpenFont(font, 32);
  win->program_on = 1;
  win->inc = NULL;

  return win;
}

void freeSDL(Window * win){
  if(win->abr){
    freeAbrDraw(win,&(win->abr));
  }
  for(int i=0;i<16;i++){
    if(win->collision[i]){
      freeColLi(win->collision+i);
    }
  }
  for(int i=0;i<win->nbText;i++){
    if(win->text[i].texture){
      SDL_DestroyTexture(win->text[i].texture);
    }
  }
  free(win->text);
  freeIncrement(&win->inc);
  SDL_DestroyRenderer(win->renderer);
  SDL_DestroyWindow(win->win);
  SDL_Quit();
  IMG_Quit();
  
}

Uint32 getTime(){
  return SDL_GetTicks();
}

void fpsFix(Uint32 timeStart, int fps){
  Uint32 now = SDL_GetTicks();
  if(now < timeStart+fps){
    SDL_Delay(timeStart+fps-now);
  }
}

int waitResize = -1;

void eventLoop(Window * win){
  while(SDL_PollEvent(&(win->event))){
    switch(win->event.type){
      
    case SDL_QUIT :
      printf("Escape \n");
      win->program_on = 0;
      break;

    case SDL_WINDOWEVENT:
      switch(win->event.window.event){
      case SDL_WINDOWEVENT_SIZE_CHANGED:
        //SDL_GetWindowSize(win->win,&win->widthWin,&win->heightWin);
	      //requestReDrawResize(win);
        waitResize = 20;
	      break;

      default:
	      break;
      }
      break;
    
    case SDL_KEYUP:
      switch(win->event.key.keysym.scancode){
	
      case SDL_SCANCODE_ESCAPE:
        printf("Escape \n");
	      win->program_on = 0;
	      break;
	
      default:
	break;
      }
      break;

    case SDL_MOUSEBUTTONUP:
      switch(win->event.button.button){

      case SDL_BUTTON_LEFT:
	      checkAllCol(&win->collision[0], win->event.button.x, win->event.button.y);
	      break;

      default:
	      break;
      }
      break;
      
    default:
      break;
    } 
  }
  
}

void loopCalc(Window * win){
  if(waitResize == 0){
    requestReDrawResize(win);
    SDL_GetWindowSize(win->win,&win->widthWin,&win->heightWin);
    //printf("window %d %d\n",win->widthWin, win->heightWin);
  }

  if(win->reDraw){
    SDL_SetRenderTarget(win->renderer,NULL);
    SDL_SetRenderDrawColor(win->renderer, 0, 0, 0, 255);
    SDL_RenderClear(win->renderer);
    updateTextAbrDraw(win->abr, win);
  }
  
  if(win->reDraw){
    SDL_SetRenderTarget(win->renderer,NULL);
    SDL_RenderPresent(win->renderer);
    win->reDraw = 0;
  }
  if(waitResize >= 0){
    waitResize--;
  }
  evalIncrement(win,&win->inc);
}

int setRenderDrawColor(SDL_Renderer * renderer, SDL_Color c){
  return SDL_SetRenderDrawColor(renderer, c.a, c.g, c.b, c.a);
}

SDL_Color colorC(Uint8 r, Uint8 g, Uint8 b, Uint8 a){
  SDL_Color c = {r,g,b,a};
  return c;
}

SDL_Point pointC(int x, int y){
  SDL_Point p = {x,y};
  return p;
}

SDL_Rect rectC(int x, int y, int w, int h){
  SDL_Rect r = {x,y,w,h};
  return r;
}

float toRad(float deg){
  return deg * (M_PI / 180);
}

float toDeg(float rad){
  return (rad * 180.) / M_PI;
}

/*void drawAround(SDL_Renderer * renderer, SDL_Point p1, SDL_Point p2, SDL_Point around){
  int offset = 10;
  SDL_Point pointMi = pointC((p1.x+p2.x)/2, (p1.y+p2.y)/2);
  float varMiArX = around.x - pointMi.x, varMiArY = around.y - pointMi.y;
  float norm = sqrtf(varMiArX*varMiArX + varMiArY*varMiArY);
  float normX = varMiArX / norm;
  float normY = varMiArY / norm;
  SDL_Point p3 = pointC(around.x + offset*normX, around.y + offset*normY);

  double M_P1P2_X = (p1.x + p2.x) / 2.0;
  double M_P1P2_Y = (p1.y + p2.y) / 2.0;
  double M_P2P3_X = (p2.x + p3.x) / 2.0;
  double M_P2P3_Y = (p2.y + p3.y) / 2.0;
  double m_perp_P1P2 = -(p2.x - p1.x) / (double)(p2.y - p1.y);
  double m_perp_P2P3 = -(p3.x - p2.x) / (double)(p3.y - p2.y);
  SDL_Point centre;
  centre.x =(M_P2P3_Y - M_P1P2_Y + m_perp_P1P2 * M_P1P2_X - m_perp_P2P3 * M_P2P3_X) / (m_perp_P1P2 - m_perp_P2P3);
  centre.y = m_perp_P1P2 * (centre.x - M_P1P2_X) + M_P1P2_Y;
  double radius = sqrt((centre.x - p1.x) * (centre.x - p1.x) + (centre.y - p1.y) * (centre.y - p1.y));
  
  printf("%d %d\n",centre.x,centre.y);
  
  SDL_SetRenderDrawColor(renderer, 0, 0, 0,255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  //drawCircle(renderer, centre, (int) radius);
  
  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawPoint(renderer, p1.x, p1.y);
  SDL_RenderDrawPoint(renderer, p2.x, p2.y);
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderDrawPoint(renderer, around.x, around.y);

  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
  SDL_RenderDrawPoint(renderer, pointMi.x, pointMi.y);
  SDL_RenderDrawPoint(renderer, centre.x, centre.y);
  SDL_RenderDrawPoint(renderer, p3.x, p3.y);
  
  SDL_RenderPresent(renderer);
}*/
