#ifndef __GEOMETRIE_H__
#define __GEOMETRIE_H__

#include <SDL2/SDL.h>
#include "SDL.h"

typedef enum {
    POINT,
    LINE,
    RECTANGLE,
    RECTANGLEFILL,
    ROUNDRECTANGLE,
    ROUNDRECTANGLEFILL,
    CIRCLE,
    CIRCLEFILL,
    ARC
} GeomType;

typedef struct geometrie
{
    void * data;
    GeomType type;
    SDL_Color color;
    int thick;
} geometrie;

typedef struct line{
    SDL_Point p1;
    SDL_Point p2;
} line;

typedef struct rectangle{
    SDL_Point p1;
    int w;
    int h;
    int angle;
} rectangle;

typedef struct roundRectangle{
    SDL_Point p;
    int w;
    int h;
    float radius;
} roundRectangle;

typedef struct circle{
    SDL_Point p;
    float radius;
} circle;

typedef struct arc{
    SDL_Point p;
    float radius;
    float start;
    float end;
} arc;

line * createLine(SDL_Point p1, SDL_Point p2);

rectangle * createRectangle(SDL_Point p,int w, int h, int angle);

roundRectangle * createRoundRectangle(SDL_Point p,int w, int h, float radius);

circle * createCircle(SDL_Point p, float radius);

arc * createArc(SDL_Point p, float radius, float start, float end);

geometrie * createGeometrie(void * data, GeomType type, SDL_Color color, int thickness);

void drawGeom(Window * win, geometrie geo, SDL_Rect dstRect,int InWidth, int InHeight);

#endif