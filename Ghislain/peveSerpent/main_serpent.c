#include <SDL2/SDL.h>
#include <stdio.h>


/*
faire un trait multicolor qui passe sur la fenetre
*/

void dessin(SDL_Renderer* rend, int x, int y,int r, int g,int b)
{
    SDL_FRect rect1 = {x,y,5,5};
    SDL_SetRenderDrawColor(rend,r,g,b,255);
    SDL_RenderDrawRectF(rend,&rect1);
    SDL_RenderPresent(rend);
    SDL_FRect rect2 = {x+1,y+1,3,3};
    SDL_SetRenderDrawColor(rend,r,b,b,255);
    SDL_RenderDrawRectF(rend,&rect2);
    SDL_RenderPresent(rend);
    SDL_FRect rect3 = {x+2,y+2,1,1};
    SDL_SetRenderDrawColor(rend,r,g,b,255);
    SDL_RenderDrawRectF(rend,&rect3);
    SDL_RenderPresent(rend);
}

void HsvToRgb(int h,int s,int v,int* r, int* g, int* b)
{
    int region;
    int remainder;
    int p;
    int q;
    int t;

    if (s == 0)
    {
        *r = v;
        *g = v;
        *b = v;
    }
    else
    {
        region = h / 43;
        remainder = (h - (region * 43)) * 6; 
        
        p = (v * (255 - s)) >> 8;
        q = (v * (255 - ((s * remainder) >> 8))) >> 8;
        t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
        
        switch (region)
        {
            case 0:
                *r = v; *g = t; *b = p;
                break;
            case 1:
                *r = q; *g = v; *b = p;
                break;
            case 2:
                *r = p; *g = v; *b = t;
                break;
            case 3:
                *r = p; *g = q; *b = v;
                break;
            case 4:
                *r = t; *g = p; *b = v;
                break;
            default:
                *r = v; *g = p; *b = q;
                break;
        }
    }
}


int main(int argc, char ** argv)
{
  (void) argc;
  (void) argv;

  int posx=0;
  int posy=0;

  int r,g,b;
  r = 255;
  g = 0;
  b = 0;
  int h;
  h = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) // Initialisation SDL
    {
    SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
    }

    int haut = 300;
    int larg = 500;

    SDL_Window * window = NULL;
    window = SDL_CreateWindow("Serpentin",400,300,larg,haut,0); // Creation fenetre

    if (window == NULL)
    {
    SDL_Log("Error : SDL window creation - %s\n",SDL_GetError());
    SDL_Quit();      
    exit(EXIT_FAILURE);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_bool program_on = SDL_TRUE;       // Gestion des evenments
    SDL_Event event;
    
    dessin(renderer,posx,posy,r,g,b);
    int depx;
    int depy;
    depx = 1;
    depy = 1;


    while (program_on)
    {
        if (SDL_PollEvent(&event))
        {
            switch(event.type)
            {                
            case SDL_QUIT :                           
                program_on = SDL_FALSE;     
                break;
       
            case SDL_KEYDOWN :
                case SDL_SCANCODE_ESCAPE :
                    program_on = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }

        // faire déplacement : voir rgb -> hsv sur internet

        h += 1;
        HsvToRgb(h%255,255,255,&r,&g,&b);

        posx += depx;
        posy += depy;
        dessin(renderer,posx,posy,r,g,b);
        

        if(posy+5 == haut) depy = -depy;
        if(posx+5 == larg) depx = -depx;
        if(posx == 0) depx = -depx;
        if(posy == 0) depy = -depy;

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    SDL_Quit();

    return 0;
}