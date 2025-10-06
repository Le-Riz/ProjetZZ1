#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <sys/time.h>
#include <math.h>

#include "bullet.h"

#define FPS 16

void init_SDL2(SDL_Window ** window, int posX, int posY, int width, int height,SDL_WindowFlags SDL_Window_Flag,char * nom){
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    SDL_Log("Error : SDL initialisation - %s\n", 
	    SDL_GetError());                // l'initialisation de la SDL a échoué 
    exit(EXIT_FAILURE);
  }

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

void getMaxWindowSize(int * w, int * h){
  SDL_Window * taille;
  init_SDL2(&taille, 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_HIDDEN | SDL_WINDOW_SKIP_TASKBAR,"");
  
  SDL_GetWindowSize(taille, w, h);
  SDL_DestroyWindow(taille);

}

Uint32 timeLeft(Uint32 time){
  Uint32 now = SDL_GetTicks();
  if(now >= time+FPS){
    return 0;
  }
  return time+FPS-now;
}

void bulletDraw(windLi_c * wind){
  SDL_Rect bulletFront={20,20,20,10};
  SDL_Rect bulletBack={10,20,10,10};
  
  SDL_SetRenderDrawColor(wind->renderer, 255, 255, 255, 255);
  SDL_RenderClear(wind->renderer);
  SDL_SetRenderDrawColor(wind->renderer, 220, 197, 84, 255);
  SDL_RenderFillRect(wind->renderer, &bulletBack);
  SDL_SetRenderDrawColor(wind->renderer, 4, 2, 4, 255);
  SDL_RenderFillRect(wind->renderer, &bulletFront);

  SDL_Rect LightBulletBack = {10,20+wind->animI*2,10,2};
  SDL_Rect LightBulletFront = {20,20+wind->animI*2,20,2};

  SDL_SetRenderDrawColor(wind->renderer, 244, 218, 100, 255);
  SDL_RenderFillRect(wind->renderer, &LightBulletBack);
  SDL_SetRenderDrawColor(wind->renderer, 132, 130, 132, 255);
  SDL_RenderFillRect(wind->renderer, &LightBulletFront);
  
  wind->animI = fmod(wind->animI+0.1875, 5);
  SDL_RenderPresent(wind->renderer);
}

void createBullet(windLi_c ** tete,int posY, int height, int nombreTour){
  int BulletHeight = 50;
  windLi_c elem;
  elem.posX = height+5+(height/20);
  elem.posY = ((int) height/2) + posY - (BulletHeight/2);
  elem.height = BulletHeight;
  if(posY == 0){
    elem.posY += 27;
  }
  
  init_SDL2(&(elem.win), elem.posX, elem.posY, elem.height, elem.height, SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP,"Bullet");
  int y=0;
  SDL_GetWindowPosition(elem.win, NULL, &y);
  elem.nombreTour = nombreTour;
  elem.animI = 0;
  elem.renderer = SDL_CreateRenderer(elem.win, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(elem.renderer, 0, 0, 0, 255);
  SDL_RenderClear(elem.renderer);
  SDL_RenderPresent(elem.renderer);
  
  addListBullet(tete, elem);
  bulletDraw(*tete);
}

int main(int argc, char ** argv){
  (void) argc;
  (void) argv;

  IMG_Init(IMG_INIT_PNG);

  int MaxWidth=0,MaxHeight=0;
  getMaxWindowSize(&MaxWidth,&MaxHeight);

  int nombreLignes = 3;
  
  SDL_Window * player = NULL;
  init_SDL2(&player, 0, 0, (int) (MaxHeight/nombreLignes), (int) (MaxHeight/nombreLignes), SDL_WINDOW_BORDERLESS | SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP,"Fenêtre Shooter");
  int playerHeight =(int) (MaxHeight/nombreLignes);
  
  SDL_Renderer * playerRend = SDL_CreateRenderer(player, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawColor(playerRend, 0, 0, 150, 255);
  SDL_RenderClear(playerRend);
  
  int * PosPlayerY = calloc(nombreLignes,sizeof(int));
  for(int i=0;i<nombreLignes;i++){
    PosPlayerY[i] = (int) (i*(MaxHeight/nombreLignes));
  }
  
  SDL_Texture * gun = IMG_LoadTexture(playerRend, "spritesheet.png");
  int gunFrameNb = 14, GunWidth = 0, GunHeight = 0, gunAnmI = 1, gunAnimOn = 0;
  float gunScale = 0;
  SDL_QueryTexture(gun, NULL, NULL, &GunWidth, &GunHeight);
  SDL_SetTextureBlendMode(gun, SDL_BLENDMODE_BLEND);
  SDL_SetTextureAlphaMod(gun, 255);
  gunScale = ((float) GunHeight / (float) (GunWidth/gunFrameNb));
  SDL_Rect gunOut = {0,0,MaxHeight/nombreLignes,MaxHeight/nombreLignes * gunScale};
  SDL_Rect gunIn = {GunWidth - GunWidth/gunFrameNb,0,GunWidth/gunFrameNb,GunHeight};// 134
  gunOut.y = (MaxHeight/nombreLignes)/2 - 70*gunScale;
  SDL_RenderCopy(playerRend, gun, &gunIn, &gunOut);
  SDL_RenderPresent(playerRend);
  int gunAnimSpeed = 2, gunAnimWait = gunAnimSpeed;
  
  
  SDL_bool program_on = SDL_TRUE;               // Booléen pour dire que le programme doit continuer
  SDL_Event event;                              // c'est le type IMPORTANT
  Uint32 Time=0;
  int PlayerPosIndice = 0;
  int ShootCooldown = 63;
  int BulletSpeed = 5;

  windLi_c * tete = initListBullet();
  
  while (program_on){
    Time = SDL_GetTicks();
    if (SDL_PollEvent(&event)){
      switch(event.type){
	
      case SDL_QUIT :
	program_on = SDL_FALSE;
	break;
	
      case SDL_KEYUP:
	switch(event.key.keysym.scancode){
	  
	case SDL_SCANCODE_ESCAPE:
	  program_on = SDL_FALSE;
	  break;
	  
	case SDL_SCANCODE_DOWN:
	  if(PlayerPosIndice < nombreLignes-1){
	    PlayerPosIndice ++;
	    SDL_SetWindowPosition(player, 0, PosPlayerY[PlayerPosIndice]);
	  }
	  break;

	case SDL_SCANCODE_UP:
	  if(PlayerPosIndice > 0){
	    PlayerPosIndice --;
	    SDL_SetWindowPosition(player, 0, PosPlayerY[PlayerPosIndice]);
	  }
	  break;

	case SDL_SCANCODE_SPACE:
	  if(ShootCooldown <= 0){
	    createBullet(&tete, PosPlayerY[PlayerPosIndice], playerHeight,BulletSpeed);
	    ShootCooldown = 63;
	    gunAnimOn = 1;
	  }
	  break;
      
	default:
	  break;
	}
	
      default:
	break;
      } 
    }

    ShootCooldown--;
    
    windLi_c ** pprec = &tete;
    while(*pprec != NULL){
      if((*pprec)->nombreTour <= 0){
	bulletDraw(*pprec);
	(*pprec)->posX += 20;
	if((*pprec)->posX + (*pprec)->height < MaxWidth){
	  SDL_SetWindowPosition((*pprec)->win, (*pprec)->posX, (*pprec)->posY);
	  (*pprec)->nombreTour = BulletSpeed;
	}else{
	  delListBullet(pprec);
	}
      }else{
	(*pprec)->nombreTour--;
      }
      if(*pprec != NULL){
	pprec = &(*pprec)->next;
      }
    }

    if(gunAnimOn && gunAnimWait <= 0){
      if(gunAnmI < 14){
	gunIn.x = GunWidth - gunIn.w * (gunAnmI+1);
	SDL_SetRenderDrawColor(playerRend, 0, 0, 150, 255);
	SDL_RenderClear(playerRend);
	SDL_RenderCopy(playerRend, gun, &gunIn, &gunOut);
	SDL_RenderPresent(playerRend);
	gunAnmI++;
      }else{
	gunAnmI = 0;
	gunIn.x = GunWidth - gunIn.w * (gunAnmI+1);
	SDL_SetRenderDrawColor(playerRend, 0, 0, 150, 255);
	SDL_RenderClear(playerRend);
	SDL_RenderCopy(playerRend, gun, &gunIn, &gunOut);
	SDL_RenderPresent(playerRend);
	gunAnimOn = 0;
      }
      gunAnimWait = gunAnimSpeed;
    }

    gunAnimWait--;
    
    SDL_Delay(timeLeft(Time));
  }

  SDL_DestroyTexture(gun);
  freeListBullet(&tete);
  free(PosPlayerY);
  SDL_DestroyRenderer(playerRend);
  SDL_DestroyWindow(player);
  IMG_Quit();
  
  return 0;
}
