#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char **argv){
  //Fenêtre principale
  SDL_Window * main_win = NULL;

  //Initialisation de la SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    SDL_Log("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  //Dimensions de la fenêtre initiale
  int SCREEN_WIDTH = 1200;
  int SCREEN_HEIGHT = 800;

  //On capture les dimensions de l'écran du PC
  SDL_DisplayMode displayMode;
  SDL_GetDesktopDisplayMode(0, &displayMode);

  //Coordonnées nous permettant d'afficher la fenêtre au milieu de l'écran du PC
  int displayX = (displayMode.w - SCREEN_WIDTH) / 2;
  int displayY = (displayMode.h - SCREEN_HEIGHT) / 2;

  //Création et Paramétrage de la fenêtre principale
  main_win = SDL_CreateWindow("Fenêtre Principale", displayX, displayY, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);


  //Vérification de la création de cette fenêtre
  if(main_win == NULL){
    SDL_Log("Erreur : échec de la création de la fenêtre : %s\n", SDL_GetError());
    SDL_DestroyWindow(main_win);
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  //Création d'un renderer pour mettre à jour notre fenêtre
  SDL_Renderer* renderer = SDL_CreateRenderer(main_win, -1, SDL_RENDERER_ACCELERATED);

  //Création d'une surface qui contiendra notre sprite sheet
  SDL_Surface* spriteSheet = IMG_Load("PngItem_2501894.png");

  //Vérification du chargement de l'image 
  if(spriteSheet == NULL) exit(EXIT_FAILURE);

  //Création de notre texture du sprite sheet
  SDL_Texture* spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSheet);
  SDL_FreeSurface(spriteSheet);

  //Vérification du chargement de la texture
  if(spriteTexture == NULL) exit(EXIT_FAILURE);

  //Variables définissant les dimensions de chaque frame du sprite
  int SPRITE_WIDTH = 200;
  int SPRITE_HEIGHT = 234;

  //Création d'un tableau stockant tous les rectangles contenant les coord de toutes nos frames
  SDL_Rect sprite[8];
  for(int i = 0; i < 8; i++){
    sprite[i].x = i*SPRITE_WIDTH;
    sprite[i].y = 0;
    sprite[i].w = SPRITE_WIDTH;
    sprite[i].h = SPRITE_HEIGHT;
  }

  //Création des variables
  SDL_Color white = { 255, 255, 255, 255 };
  SDL_bool en_route = SDL_TRUE;
  SDL_bool drawW = SDL_FALSE;
  SDL_Event event;
  SDL_Rect Wrect;
  SDL_Rect Grect;
  SDL_bool rabit = SDL_FALSE;
  SDL_RendererFlip flip = SDL_FLIP_NONE;
  int current_frame = 0;
  int FPS = 15;
  int FRAME_DELAY = 1000/FPS;
  int x,y,largeur,hauteur;
  int vitesseX = 4;
  int vitesseY = 4;
  int frameTime,frameStart;
  while(en_route){
    frameStart = SDL_GetTicks();
    while (SDL_PollEvent(&event))
    {
      SDL_GetWindowSize(main_win, &largeur, &hauteur);
      switch (event.type)
      {
      case SDL_QUIT:
        en_route = SDL_FALSE;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_b:
          if(drawW == SDL_FALSE){
            drawW = SDL_TRUE;
          }
          break;
        case SDLK_r:
          if(rabit == SDL_FALSE && drawW == SDL_TRUE){
            rabit = SDL_TRUE;
            /*Coordonnées de là où apparaît notre sprite en tout premier lieu*/
            Grect.x = largeur/4;
            Grect.y = hauteur/4;
            Grect.w = SPRITE_WIDTH;
            Grect.h = SPRITE_HEIGHT;
          }
          break;
        case SDLK_UP:
          SDL_GetWindowPosition(main_win, &x, &y);
          if(y>=10){
            y -= 10;
          }
          else{
            y = 0;
          }
          SDL_SetWindowPosition(main_win, x, y);
          break;
        case SDLK_DOWN:
          SDL_GetWindowPosition(main_win, &x, &y);
          if(y<=displayMode.h-10-hauteur){
            y += 10;
          }
          else{
            y = displayMode.h-hauteur;
          }
          SDL_SetWindowPosition(main_win, x, y);
          break;
        case SDLK_LEFT:
          SDL_GetWindowPosition(main_win, &x, &y);
          if(x >=10){
            x -= 10;
          }
          else{
            x = 0;
          }
          SDL_SetWindowPosition(main_win, x, y);
          break;
        case SDLK_RIGHT:
          SDL_GetWindowPosition(main_win, &x, &y);
          if(x<=displayMode.w-10-largeur){
            x += 10;
          }
          else{
            x = displayMode.w-largeur;
          }
          SDL_SetWindowPosition(main_win, x, y);
          break;
        default:
          break;
        }
      case SDL_WINDOWEVENT: 
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_CLOSE: //Termine le programme lorsqu'on clique sur la croix pour fermer la fenêtre
          en_route = SDL_FALSE;
          break;
        default:
          break;
        }
      default:
        break;
      }
    }

    if (drawW == SDL_TRUE ||  rabit == SDL_TRUE){
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);
    }

    if (drawW == SDL_TRUE){
      
      Wrect.x = largeur/4;
      Wrect.y = hauteur/4;
      Wrect.w = largeur/2;
      Wrect.h = hauteur/2;
      SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, white.a);
      SDL_RenderDrawRect(renderer, &Wrect);
    }

    if (rabit == SDL_TRUE){
      if(Grect.x <= Wrect.x || Grect.x >= (Wrect.w+Wrect.x-Grect.w)){

        if(Grect.x < Wrect.x){
          Grect.x = Wrect.x;
        }
        if(Grect.x > (Wrect.w+Wrect.x-Grect.w)){
          Grect.x = (Wrect.w+Wrect.x-Grect.w);
        }

        if(flip == SDL_FLIP_NONE){
          flip = SDL_FLIP_HORIZONTAL;
        }
        else{
          flip = SDL_FLIP_NONE;
        }

        vitesseX = - vitesseX;
      }
      if(Grect.y <= Wrect.y || Grect.y >= (Wrect.h+Wrect.y-Grect.h)){
        if (Grect.y < Wrect.y){
          Grect.y = Wrect.y;
        }
        if (Grect.y > (Wrect.h+Wrect.y-Grect.h)){
          Grect.y = (Wrect.h+Wrect.y-Grect.h);
        }
        vitesseY = - vitesseY;
      }
      Grect.x = Grect.x + vitesseX;
      Grect.y = Grect.y + vitesseY;
      SDL_RenderCopyEx(renderer, spriteTexture, &sprite[current_frame], &Grect, 0, NULL, flip);
      current_frame = (current_frame+1)%8;
      
    }
    SDL_RenderPresent(renderer);
    
    frameTime = SDL_GetTicks() - frameStart;

    if (frameTime < FRAME_DELAY){
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  //Termine le programme
  SDL_Delay(200); 
  SDL_DestroyTexture(spriteTexture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(main_win);
  SDL_Quit();

}