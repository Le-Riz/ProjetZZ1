#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "game.h"
#include "SDL.h"
#include "texture.h"
#include "Menu.h"
#include "collision.h"
#include "background.h"

void initGameTexture(Window * win){
    int limuleWidth, limuleHeight;
    SDL_Texture * limuleTexture = IMG_LoadTexture(win->renderer,SRC_SPRITE"slimebleuprojet.png");
    SDL_QueryTexture(limuleTexture,NULL,NULL,&limuleWidth,&limuleHeight);
    Texture * limule = createTexture(limuleTexture,rectC(0,0,limuleWidth/11,limuleHeight),limuleWidth,limuleHeight,1,11,1,5,5);

    abrDraw * GameScreen = createAbrDraw(NULL,NULL,NULL,TEXTURE,NULL,propRectC(0,0,100,100,0,0,1),1,0,NULL);
    GameScreen->object = createPereTexture(win,GameScreen,-1,-1);
    addAbrDraw(&win->abr, GameScreen);
    colLi * col = win->collision[0];
    while(col && col->col->action != &StartPlay){
        col = col->next;
    }
    if(col){
        col->col->abr[3] = GameScreen;
    }

    abrDraw * Gameplay = createAbrDraw(GameScreen,NULL,NULL,TEXTURE,NULL,propRectC(0,0,100,100,0,0,1),1,1,NULL);
    Gameplay->object = createPereTexture(win,Gameplay,800,800);
    addAbrDraw(&GameScreen->lv, Gameplay);
    printf("Gameplay %p %p\n",Gameplay, ((Texture *)Gameplay->object)->texture->texture);

    int bgWidth, bgHeight;
    SDL_Texture * BgTexture = IMG_LoadTexture(win->renderer,SRC_SPRITE"mapprojet.png");
    SDL_QueryTexture(BgTexture,NULL,NULL,&bgWidth,&bgHeight);
    BackgroundTexture * BgGame = createBackground(BgTexture,rectC(0,0,12,12),24,24,0,28,28,4,1,0);
    abrDraw * bgDraw = createAbrDraw(Gameplay,NULL,NULL,BACKGROUND,BgGame,propRectC(0,0,100,100,0,0,1),1,1,NULL);
    addAbrDraw(&Gameplay->lv,bgDraw);

    /*abrDraw * limuleDraw = createAbrDraw(Gameplay,NULL,NULL,TEXTURE,limule,propRectC(25,25,25,25,0,0,1),1,0,NULL);
    addAbrDraw(&Gameplay->lv,limuleDraw);*/

    Increment * limuleAnim = createIncrement(&limule->currentAnim,limule->waitTime,1,limule->nbFramePerLine-1,0,1,NULL,0);
    placeSpriteOnGrid(win,2,2,bgDraw,limule,limuleAnim);
    addIncrement(&win->inc,limuleAnim);

    Texture ** walls = calloc(3,sizeof(Texture));
    SDL_Texture * bushWall = IMG_LoadTexture(win->renderer,SRC_SPRITE"bushprojet.png");
    int widthBush, heightBush;
    SDL_QueryTexture(bushWall,NULL,NULL,&widthBush,&heightBush);
    SDL_Texture * rockWall = IMG_LoadTexture(win->renderer,SRC_SPRITE"rocherprojet.png");
    int widthRock, heightRock;
    SDL_QueryTexture(rockWall,NULL,NULL,&widthRock,&heightRock);
    SDL_Texture * iceWall = IMG_LoadTexture(win->renderer,SRC_SPRITE"glaceprojet.png");
    int widthIce, heightIce;
    SDL_QueryTexture(iceWall,NULL,NULL,&widthIce,&heightIce);
    walls[0] = createTexture(bushWall,rectC(0,0,widthBush,heightBush),widthBush,heightBush,0,1,1,-1,-1);
    walls[1] = createTexture(rockWall,rectC(0,0,widthRock,heightRock),widthRock,heightRock,0,1,1,-1,-1);
    walls[2] = createTexture(iceWall,rectC(0,0,widthIce,heightIce),widthIce,heightIce,0,1,1,-1,-1);
    Texture * Walls = makeSprite(win,walls,3,-1);
    printf("Sprite Dim %d %d, bush %d %d, rock %d %d, ice %d %d\n",Walls->width,Walls->height,widthBush,heightBush,widthRock,heightRock,widthIce,heightIce);
    SDL_DestroyTexture(bushWall);
    SDL_DestroyTexture(rockWall);
    SDL_DestroyTexture(iceWall);
    free(walls);
    printf("save into %d ",saveTexture(win,Walls));
    printf("Texture Save %p\n",Walls->texture->texture);

    createWall(win,0,0,bgDraw);
    
}

void placeSpriteOnGrid(Window * win, int x, int y, abrDraw * Grid, Texture * sprite, Increment * spriteInc){
    int width, height;
    getSizeAbr(win,Grid,&width,&height);
    BackgroundTexture * bgText = (BackgroundTexture *) Grid->object;
    SDL_Rect In = inputRect(*bgText);
    SDL_Rect Pos = propRectToRect(propRectC(0,0,100,100,0,0,1),width,height,In.w,In.h);
    SDL_FRect PosRect;
    PosRect.x = Pos.w*(x * bgText->caseWidth + (x-1) * bgText->caseSep)/In.w;
    PosRect.y = Pos.h*(y * bgText->caseHeight + (y-1) * bgText->caseSep)/In.h;
    PosRect.w = Pos.w*(bgText->caseWidth)/In.w;
    PosRect.h = Pos.h*(bgText->caseHeight)/In.h;
    propRect Rect = propRectC((float)PosRect.x/(float)width*100.,(float)PosRect.y/(float)height*100.,(float)PosRect.w/(float)width*100.,(float)PosRect.h/(float)height*100.,0,0,0);
    abrDraw * spriteAbr = createAbrDraw(Grid->pere,NULL,NULL,TEXTURE,sprite,Rect,1,1,NULL);
    if(spriteInc){
        spriteInc->abr = spriteAbr;
        spriteAbr->increment = spriteInc;
    }
    addAbrDraw(&(Grid->pere->lv),spriteAbr);

}

void createWall(Window * win, int x, int y, abrDraw * bg){
    Texture * Walls = loadTexture(win,0);
    placeSpriteOnGrid(win,x,y,bg,Walls,NULL);
}