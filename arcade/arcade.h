#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

struct Entite {
    SDL_Point coord;
    int HP;
};
typedef struct Entite Entite;

enum Position{
    NORD,
    SUD,
    EST,
    OUEST
};

enum Distance{ //-1, 0 et 1
    PROCHE=-1,
    MOYEN,
    LOIN
};