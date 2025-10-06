#include <SDL2/SDL.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>

Uint32 timeleft(Uint32 time)
{
    Uint32 now = SDL_GetTicks();
    if(now >= time + 16)
    {
        return 0;
    }
    return time + 16 - now;
}

int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) // Initialisation SDL
    {
        SDL_Log("Error : SDL initialisation - %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window * window = NULL;
    window = SDL_CreateWindow("Animation",200,200,600,600,0); // Creation fenetre

    if (window == NULL)
    {
    SDL_Log("Error : SDL window creation - %s\n",SDL_GetError());
    SDL_Quit();      
    exit(EXIT_FAILURE);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    SDL_Texture* background;
    background = IMG_LoadTexture(renderer,"./savane.png");
    int bl,bh;
    SDL_QueryTexture(background,NULL,NULL,&bl, &bh);

    SDL_Rect sourceb = {0,0,600,600};
    SDL_Rect destinationb = {0,0,600,600};

    SDL_Texture* texture;
    texture = IMG_LoadTexture(renderer,"./run.png");

    SDL_bool program_on = SDL_TRUE;   // Gestion des evenments
    SDL_Event event;

    int w,h;
    SDL_QueryTexture(texture,NULL,NULL,&w, &h);
    int offset_x = w / 2,offset_y = h / 4;

    SDL_Rect source = {0,0,offset_x,offset_y};
    SDL_Rect destination = {0,400,offset_x,offset_y};

    int i,j,comp;
    i = 0;
    j = 0;
    comp =0;

    while (program_on)
    {
        Uint32 time = SDL_GetTicks();
        if (SDL_PollEvent(&event))
        {
            switch(event.type)
            {                
            case SDL_QUIT :                           
                program_on = SDL_FALSE;     
                break;
            case SDL_KEYDOWN :
                switch(event.key.keysym.scancode)
                {
                case SDL_SCANCODE_DOWN :
                    destination.y += 10;
                    destination.w *= 1.1;
                    destination.h *= 1.1;
                    break;
                case SDL_SCANCODE_UP :
                    destination.y -= 10;
                    destination.w *= 0.9;
                    destination.h *= 0.9;
                    break;
                
                case SDL_SCANCODE_ESCAPE :
                    program_on = SDL_FALSE;
                    break;
                default:
                    break;
                }
            }
        }

        if(comp == 0)
        { 
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, background,&sourceb,&destinationb);  
            SDL_RenderCopy(renderer, texture,&source,&destination);  
            SDL_RenderPresent(renderer);
            if(i==0)
            {
                    destination.x += 2;
                    source.x += offset_x;
                    i++;
            }
            else
            {
                    destination.x += 2;
                    source.x -= offset_x;
                    source.y += offset_y;
                    i--;
                    j++;
            }

            if((i>=1)&&(j>=3))
            {
                 source.x = 0;
                 source.y = 0;
                  j = 0;
                  i = 0;
            }
            comp = 3;
            sourceb.x += 4;

        }
        comp--;


        Uint32 d = timeleft(time);
        SDL_Delay(d);

        if(destination.x > 580)
        {
            program_on = SDL_FALSE;
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();

    return 0;
}