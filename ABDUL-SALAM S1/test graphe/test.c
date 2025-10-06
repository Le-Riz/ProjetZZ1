#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include "test.h"

double alea(){
  return (double)rand() / ((double)RAND_MAX + 1);
}

double distanceEuclidienne(Case p1, Case p2){
  double deltaX = p2.x - p1.x;
  double deltaY = p2.y - p1.y;
  return sqrt((deltaX * deltaX) + (deltaY * deltaY));
}

void libereMatrice(void * M, int type, int n){
  switch (type)
  {
  case 0:
    int ** mat = (int **) M;
    for(int i = 0; i < n; i++){
      free(mat[i]);
    }
    free(mat);
    break;
  
  case 1:
    double ** dat = (double **) M;
    for(int i = 0; i < n; i++){
      free(dat[i]);
    }
    free(dat);
    break;
  case 2:
    Case * pat = (Case *) M;
    free(pat);
    break;
  case 3:
    int * iat = (int *) M;
    free(iat);
    break;
  default:
    break;
  }
}

void genere(int ** matrice, int bas, int haut){
  if(bas < haut){
    int range = haut-(bas+1) + 1;
    int k = (bas+1) + rand() % range;
    matrice[bas][bas+1] = 1;
    matrice[bas+1][bas] = 1;
    if (k+1 <= haut){
        matrice[bas][k+1] = 1;
        matrice[k+1][bas] = 1;
    }
    genere(matrice, bas+1, k);
    genere(matrice, k+1, haut);
  }
}

void genere_Graph(int ** matrice, double p, int n){ //p = 0 -> arbre sans altération | p=1 -> graphe complet | n nombre de points
    for(int i = 0; i < n; i++){
        for(int j = i+1; j < n; j++){
            if (alea()<p){
                matrice[i][j] = 1; 
            }
        }
    }
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int r){
    int x = r - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (r << 1);
        }
    }
}

float convertAngleToRad(float angle){
  return (angle*M_PI)/180;
}

void defineDots(float centerX, float centerY, Points P, int nb, float radius){
  float startAngle = 0;
  float a = convertAngleToRad(startAngle);
  int x,y;
  for(int i = 0; i < nb; i++){
    a+=convertAngleToRad(360.0f/nb);
    x = cosf(a)*radius;
    y = sinf(a)*radius;
    P.coord[i].x = centerX + x;
    P.coord[i].y = centerY + y;  
  }
}

int main(int argc, char **argv){
  srand(time(NULL));
  
  //Fenêtre principale
  SDL_Window * main_win = NULL;

  //Initialisation de la SDL
  if(SDL_Init(SDL_INIT_VIDEO) != 0){
    SDL_Log("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  //Dimension de la fenêtre initiale.
  int SCREEN_WIDTH = 1200;
  int SCREEN_HEIGHT = 800;

  SDL_DisplayMode displayMode;
  SDL_GetDesktopDisplayMode(0, &displayMode);

  /*Coordonnées nous permettant d'afficher la fenêtre au centre de l'écran du PC*/
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

  //Création des variables
  int NOMBRE_POINT = 8;
  SDL_Color color = { 255, 255, 255, 255 }; //Couleur des sommets (des cercles)
  SDL_bool MDone = SDL_FALSE; //Booléen permettant de calculer la distance des points FIXE dans les dimensions initiales de la fenêtre.
  SDL_bool en_route = SDL_TRUE;
  SDL_bool draw = SDL_FALSE; //Booléen permettant de déterminer quand on fait apparaître le graphe
  SDL_Event event;
  int nbIN, nbARC; //nbIN compte le nombre de point étant impliqué dans un arc entre deux sommets (cas où notre graphe n'est pas complet)
  int FPS = 15; 
  int FRAME_DELAY = 1000/FPS;
  int largeur,hauteur; //largeur et la hauteur de la fenêtre qui se met à jour lorsqu'on redimensionne la fenêtre
  int rayon = 30; //Rayon de chaque sommet
  int frameTime,frameStart;
  Points P; //Les points statiques, sont qui sont omni-présents dans la mémoire (notre banque de points)
  P.coord = (Case *) malloc(sizeof(Case)*NOMBRE_POINT);
  Points PIn; //Les points qui seronts affichés (ceux qui sont impliqués dans un arc).
  PIn.coord = (Case *) malloc(sizeof(Case)*NOMBRE_POINT);
  int * In = (int *) malloc(sizeof(int) * NOMBRE_POINT); //Les indices des concernés par un arc (ceux qui seront affichés dans le cas d'un graphe incomplet)
  Case * arcs = (Case *) malloc(sizeof(Case)*(NOMBRE_POINT*NOMBRE_POINT));
  
  //matrice des arêtes
  int ** matrice = (int **) malloc(sizeof(int *)*NOMBRE_POINT);
  //matrice des distances euclidiennes
  double ** Dmatrice = (double **) malloc(sizeof(double *)*NOMBRE_POINT);
  //Initialisation des deux matrices
  for(int i=0; i < NOMBRE_POINT; i++){
    matrice[i] = (int *) malloc(sizeof(int) * NOMBRE_POINT);
    Dmatrice[i] = (double *) malloc(sizeof(double) * NOMBRE_POINT);
  }
  for(int i = 0; i<NOMBRE_POINT; i++){
    for(int j = 0; j<NOMBRE_POINT; j++){
        matrice[i][j]=0;
        if(j>=i){
          Dmatrice[i][j]=0;
        }
    }
  }
  while(en_route){
    frameStart = SDL_GetTicks();
    SDL_GetWindowSize(main_win, &largeur, &hauteur); //on recalcule à chaque tour de boucle la largeur et la hauteur de la fenêtre.
    while (SDL_PollEvent(&event))
    {

      switch (event.type)
      {
      case SDL_QUIT:
        en_route = SDL_FALSE;
        break;
      case SDL_WINDOWEVENT: 
        switch (event.window.event)
        {
        case SDL_WINDOWEVENT_CLOSE: //Termine le programme lorsqu'on clique sur la croix pour fermer la fenêtre
          en_route = SDL_FALSE;
          break;
        default:
          break;
        }
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_SPACE: //Là on affiche/change le graphe
          draw = SDL_TRUE;
          for(int i = 0; i < NOMBRE_POINT; i++){
            In[i] = -1; //Réinitialise le tableau.
            for(int j = 0; j < NOMBRE_POINT; j++){
              matrice[i][j] = 0; //Réinitialise la matrice
            }
          }
          genere(matrice, 0, NOMBRE_POINT-1);
          genere_Graph(matrice, 1, NOMBRE_POINT);
          int nbArcs = 0; 
          int nbIn = 0; //Nombre de points qui seront présents (ceux impliqués à un arc dans le cas d'un graphe incomplet).
          for(int i = 0; i < NOMBRE_POINT; i++){
            for(int j = 0; j < NOMBRE_POINT; j++){
              if(matrice[i][j] == 1){
                arcs[nbArcs].x = i;
                arcs[nbArcs].y = j;
                nbArcs++;
                /*On test si les points à ajouter n'existent pas déjà dans le tab In*/
                if(nbIn > 0){
                  int k = 0;
                  while(k < nbIn && In[k] != j){
                    k++;
                  }
                  if (k >= nbIn){
                    In[nbIn] = j;
                    nbIn++;
                  }
                  k= 0;
                  while(k < nbIn && In[k] != i){
                    k++;
                  }
                  if (k >= nbIn){
                    In[nbIn] = i;
                    nbIn++;
                  }
                }
                else{
                  In[nbIn] = i;
                  nbIn++;
                  In[nbIn] = j;
                  nbIn++;
                }
              }
            }
          }
          nbIN = nbIn; //On enregistre les nouvelles valeurs obtenues dans une variable pouvant être utiliser en dehors de cette seconde boucle 
          nbARC = nbArcs;
          break;
        
        default:
          break;
        }
      default:
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if(draw == SDL_TRUE){

      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);

      /*Création des n points -> Détermination de leurs coordonnées*/
      defineDots(largeur/2, hauteur/2, P, NOMBRE_POINT, (largeur/4)-20); //le rayon est réduit de 20 pixel afin d'avoir un bon rendu en plein écran

      /*Cette étape auquelle on calcule notre matrice des distances ne se réalise qu'une fois dans tout le processus afin de ne pas biaiser les distances en redimensionnant la fenêtre*/
      if(MDone == SDL_FALSE){
        /*Une fois les points définits, on détermine la distance entre les points, on précise que cette distance est fixé pour ne pas avoir des distances biaisés lors de la redimension de la fenêtre*/
        for(int i = 0; i < NOMBRE_POINT; i++){
          for(int j = i; j < NOMBRE_POINT; j++){
            if(i == j){
              Dmatrice[i][j] = 0;
            }
            else{
              Dmatrice[i][j] = distanceEuclidienne(P.coord[i], P.coord[j]);
            }
          }
        }
        MDone = SDL_TRUE;
      }


      /*On place les points qui doivent être afficher dans PIn*/
      for(int i = 0; i < nbIN; i++){
        PIn.coord[i] = P.coord[In[i]];
      }

      /*Dessin des cercles autour des points concernés*/
      for(int i = 0; i < nbIN; i++){
        drawCircle(renderer, PIn.coord[i].x, PIn.coord[i].y, rayon);
      }

      /*Dessin des arcs*/
      for(int i = 0; i < nbARC; i++){
        SDL_RenderDrawLine(renderer, P.coord[arcs[i].x].x, P.coord[arcs[i].x].y, P.coord[arcs[i].y].x, P.coord[arcs[i].y].y);
      }

    }

    SDL_RenderPresent(renderer);
    
    frameTime = SDL_GetTicks() - frameStart;

    if (frameTime < FRAME_DELAY){
      SDL_Delay(FRAME_DELAY - frameTime);
    }
  }

  //Termine le programme
  libereMatrice(matrice, 0, NOMBRE_POINT);
  libereMatrice(Dmatrice, 1, NOMBRE_POINT);
  libereMatrice(P.coord, 2, NOMBRE_POINT);
  libereMatrice(PIn.coord, 2, NOMBRE_POINT);
  libereMatrice(arcs, 2, NOMBRE_POINT);
  libereMatrice(In, 3, NOMBRE_POINT);

  SDL_Delay(200); 
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(main_win);
  SDL_Quit();
}