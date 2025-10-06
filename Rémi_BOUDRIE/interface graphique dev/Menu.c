#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "SDL.h"
#include "texture.h"
#include "Menu.h"
#include "draw.h"
#include "collision.h"
#include "background.h"
#include "geometrie.h"

void initStartingScreen(Window * win){
    abrDraw * StartScreen = createAbrDraw(NULL,NULL,NULL,TEXTURE,NULL,propRectC(0,0,100,100,0,0,0),1,1,NULL);
    StartScreen->object = createPereTexture(win,StartScreen,-1,-1);
    addAbrDraw(&win->abr, StartScreen);
    printf("Start screen Texture %p\n",((Texture *)StartScreen->object)->texture->texture);
    
    SDL_Texture * bgImgText = IMG_LoadTexture(win->renderer,SPRITESRC"menuwallpaper.png");
    int bgImgW, bgImgH;
    SDL_QueryTexture(bgImgText,NULL,NULL,&bgImgW,&bgImgH);
    BackgroundTexture * bgImg = createBackground(bgImgText,rectC(0,0,bgImgW,bgImgH),1,1,0,bgImgW,bgImgH,1,1,0);
    abrDraw * StartScreenBg = createAbrDraw(StartScreen,NULL,NULL,BACKGROUND,bgImg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&StartScreen->lv,StartScreenBg);
    //addAbrDraw(&win->abr,StartScreenBg);
    printf("Background image Texture %p\n",bgImg->texture);

    /*SDL_Texture * titleImgText = IMG_LoadTexture(win->renderer,SPRITESRC"projettitre.png");
    int titleImgW, titleImgH;
    SDL_QueryTexture(titleImgText,NULL,NULL,&titleImgW,&titleImgH);
    //printf("Title dim w:%d h:%d\n",titleImgW, titleImgH);
    Texture * titleText = createTexture(titleImgText,rectC(0,0,titleImgW,titleImgH),titleImgW,titleImgH,0,1,1,-1,-1);
    abrDraw * titleStart = createAbrDraw(StartScreen,NULL,NULL,TEXTURE,titleText,propRectC(10,0,80,0,0,0,1),0,1,NULL);
    addAbrDraw(&StartScreen->lv,titleStart);
    //addAbrDraw(&win->abr,titleStart);
    printf("Titre Texture %p\n",titleText->texture->texture);*/


    abrDraw * Buttons = createAbrDraw(StartScreen,NULL,NULL,TEXTURE,NULL,propRectC(37,40,25,50,0,0,0),1,1,NULL);
    Buttons->object = createPereTexture(win,Buttons,400,800);
    addAbrDraw(&StartScreen->lv,Buttons);

    abrDraw * PlayButton = createAbrDraw(Buttons,NULL,NULL,TEXTURE,NULL,propRectC(0,0,100,25,0,0,0),1,1,NULL);
    PlayButton->object = createPereTexture(win,PlayButton,400,200);
    addAbrDraw(&Buttons->lv,PlayButton);
    printf("Boutton Start Texture %p, %d,%d\n",((Texture *)PlayButton->object)->texture->texture,((Texture *)PlayButton->object)->width,((Texture *)PlayButton->object)->height);

    geometrie * geoButtonBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * StartButtonBg = createAbrDraw(PlayButton,NULL,NULL,GEOMETRIE,geoButtonBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&PlayButton->lv,StartButtonBg);

    geometrie * geoButtonArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * StartButtonArround = createAbrDraw(PlayButton,NULL,NULL,GEOMETRIE,geoButtonArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&PlayButton->lv,StartButtonArround);

    strDraw(win, PlayButton,rectC(20,25,350,175), colorC(0,0,0,255), "Play");

    abrDraw * OptionButton = createAbrDraw(Buttons,NULL,NULL,TEXTURE,NULL,propRectC(0,30,100,25,0,0,0),1,1,NULL);
    OptionButton->object = createPereTexture(win,OptionButton,400,200);
    addAbrDraw(&Buttons->lv,OptionButton);

    geometrie * OptiongeoButtonBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * OptionButtonBg = createAbrDraw(OptionButton,NULL,NULL,GEOMETRIE,OptiongeoButtonBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionButton->lv,OptionButtonBg);

    geometrie * OptiongeoButtonArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * OptionButtonArround = createAbrDraw(OptionButton,NULL,NULL,GEOMETRIE,OptiongeoButtonArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionButton->lv,OptionButtonArround);

    strDraw(win, OptionButton,rectC(25,50,350,175), colorC(0,0,0,255), "Option");

    abrDraw * QuitButton = createAbrDraw(Buttons,NULL,NULL,TEXTURE,NULL,propRectC(0,60,100,25,0,0,0),1,1,NULL);
    QuitButton->object = createPereTexture(win,QuitButton,400,200);
    addAbrDraw(&Buttons->lv,QuitButton);

    geometrie * QuitgeoButtonBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * QuitButtonBg = createAbrDraw(QuitButton,NULL,NULL,GEOMETRIE,QuitgeoButtonBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&QuitButton->lv,QuitButtonBg);

    geometrie * QuitgeoButtonArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * QuitButtonArround = createAbrDraw(QuitButton,NULL,NULL,GEOMETRIE,QuitgeoButtonArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&QuitButton->lv,QuitButtonArround);

    strDraw(win, QuitButton,rectC(25,50,350,175), colorC(0,0,0,255), "Quitter");
    abrDraw ** collisionQuitAbr = calloc(1,sizeof(abrDraw *));
    collision ** collisionQuitCol = calloc(1,sizeof(collision *));
    collisionQuitAbr[0] = QuitButton;
    collision collisionQuit = generCol(win,collisionQuitAbr,1,boxColC(37.22,70.37,23.61,11.67,0),1,collisionQuitCol,1,Box,&quitGame);
    collisionQuitCol[0] = addColLi(&win->collision[0],collisionQuit);

    abrDraw * OptionMenu = createAbrDraw(StartScreen,NULL,NULL,TEXTURE,NULL,propRectC(10,30,80,60,0,0,0),1,0,NULL);
    OptionMenu->object = createPereTexture(win,OptionMenu,800,600);
    addAbrDraw(&StartScreen->lv,OptionMenu);

    geometrie * OptionMenuBggeo = createGeometrie(createRoundRectangle(pointC(0,0),750,550,25),ROUNDRECTANGLEFILL,colorC(125,125,125,175),4);
    abrDraw * OptionMenuBg = createAbrDraw(OptionMenu,NULL,NULL,GEOMETRIE,OptionMenuBggeo,propRectC(1,1,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionMenu->lv,OptionMenuBg);

    geometrie * OptionMenuArroundgeo = createGeometrie(createRoundRectangle(pointC(0,0),750,550,25),ROUNDRECTANGLE,colorC(75,75,75,255),10);
    abrDraw * OptionMenuArround = createAbrDraw(OptionMenu,NULL,NULL,GEOMETRIE,OptionMenuArroundgeo,propRectC(1,1,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionMenu->lv,OptionMenuArround);

    abrDraw * OptionMenuQuit = createAbrDraw(OptionMenu,NULL,NULL,TEXTURE,NULL,propRectC(37,70,25,20,0,0,0),1,1,NULL);
    OptionMenuQuit->object = createPereTexture(win,OptionMenuQuit,400,200);
    addAbrDraw(&OptionMenu->lv,OptionMenuQuit);

    geometrie * OptionMenuQuitBggeo = createGeometrie(createRoundRectangle(pointC(0,0),350,150,25),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * OptionMenuQuitBg = createAbrDraw(OptionMenuQuit,NULL,NULL,GEOMETRIE,OptionMenuQuitBggeo,propRectC(1,1,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionMenuQuit->lv,OptionMenuQuitBg);

    geometrie * OptionMenuQuitArroundgeo = createGeometrie(createRoundRectangle(pointC(0,0),350,150,25),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * OptionMenuQuitArround = createAbrDraw(OptionMenuQuit,NULL,NULL,GEOMETRIE,OptionMenuQuitArroundgeo,propRectC(1,1,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&OptionMenuQuit->lv,OptionMenuQuitArround);

    strDraw(win, OptionMenuQuit,rectC(10,30,350,175), colorC(0,0,0,255), "Retour");

    abrDraw ** collisionOptionAbr = calloc(2,sizeof(abrDraw *));
    collision ** collisionOptionCol = calloc(4,sizeof(collision *));
    collisionOptionCol[2] = collisionQuitCol[0];
    collisionOptionAbr[0] = Buttons;
    collisionOptionAbr[1] = OptionMenu;
    collision collisionOption = generCol(win,collisionOptionAbr,2,boxColC(37.22,55.37,23.61,11.67,0),1,collisionOptionCol,2,Box,&enableOptionStartScreen);
    collisionOptionCol[0] = addColLi(&win->collision[0],collisionOption);
    collision collisionOptionBack = generCol(win,collisionOptionAbr,2,boxColC(39.58,72.04,17.92,9.26,0),0,collisionOptionCol,2,Box,&disableOptionStartScreen);
    collisionOptionCol[1] = addColLi(&win->collision[0],collisionOptionBack);

    abrDraw * ButtonsPlay = createAbrDraw(StartScreen,NULL,NULL,TEXTURE,NULL,propRectC(37,40,25,50,0,0,0),1,0,NULL);
    ButtonsPlay->object = createPereTexture(win,ButtonsPlay,400,800);
    addAbrDraw(&StartScreen->lv,ButtonsPlay);

    abrDraw * PlayButtonPlayMenu = createAbrDraw(ButtonsPlay,NULL,NULL,TEXTURE,NULL,propRectC(0,0,100,25,0,0,0),1,1,NULL);
    PlayButtonPlayMenu->object = createPereTexture(win,PlayButtonPlayMenu,400,200);
    addAbrDraw(&ButtonsPlay->lv,PlayButtonPlayMenu);

    geometrie * geoButtonPlayMenuBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * StartButtonPlayMenuBg = createAbrDraw(PlayButtonPlayMenu,NULL,NULL,GEOMETRIE,geoButtonPlayMenuBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&PlayButtonPlayMenu->lv,StartButtonPlayMenuBg);

    geometrie * geoButtonPlayMenuArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * StartButtonPlayMenuArround = createAbrDraw(PlayButtonPlayMenu,NULL,NULL,GEOMETRIE,geoButtonPlayMenuArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&PlayButtonPlayMenu->lv,StartButtonPlayMenuArround);

    strDraw(win, PlayButtonPlayMenu,rectC(25,75,350,175), colorC(0,0,0,255), "   Play   ");

    abrDraw * SimuButtonMenu = createAbrDraw(ButtonsPlay,NULL,NULL,TEXTURE,NULL,propRectC(0,30,100,25,0,0,0),1,1,NULL);
    SimuButtonMenu->object = createPereTexture(win,SimuButtonMenu,400,200);
    addAbrDraw(&ButtonsPlay->lv,SimuButtonMenu);

    geometrie * geoButtonSimuMenuBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * ButtonSimuMenuBg = createAbrDraw(SimuButtonMenu,NULL,NULL,GEOMETRIE,geoButtonSimuMenuBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&SimuButtonMenu->lv,ButtonSimuMenuBg);

    geometrie * geoButtonSimuMenuArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * ButtonSimuMenuArround = createAbrDraw(SimuButtonMenu,NULL,NULL,GEOMETRIE,geoButtonSimuMenuArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&SimuButtonMenu->lv,ButtonSimuMenuArround);

    strDraw(win, SimuButtonMenu,rectC(25,75,350,175), colorC(0,0,0,255), "Simulation");

    abrDraw * RetourButtonMenu = createAbrDraw(ButtonsPlay,NULL,NULL,TEXTURE,NULL,propRectC(0,60,100,25,0,0,0),1,1,NULL);
    RetourButtonMenu->object = createPereTexture(win,RetourButtonMenu,400,200);
    addAbrDraw(&ButtonsPlay->lv,RetourButtonMenu);

    geometrie * geoButtonRetourMenuBg = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLEFILL,colorC(0,120,158,255),4);
    abrDraw * ButtonRetourMenuBg = createAbrDraw(RetourButtonMenu,NULL,NULL,GEOMETRIE,geoButtonRetourMenuBg,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&RetourButtonMenu->lv,ButtonRetourMenuBg);

    geometrie * geoButtonRetourMenuArround = createGeometrie(createRoundRectangle(pointC(0,0),400,200,10),ROUNDRECTANGLE,colorC(0,0,0,255),10);
    abrDraw * ButtonRetourMenuArround = createAbrDraw(RetourButtonMenu,NULL,NULL,GEOMETRIE,geoButtonRetourMenuArround,propRectC(0,0,100,100,0,0,0),0,1,NULL);
    addAbrDraw(&RetourButtonMenu->lv,ButtonRetourMenuArround);

    strDraw(win, RetourButtonMenu,rectC(25,75,350,175), colorC(0,0,0,255), "   Retour   ");

    abrDraw ** collisionPlayAbr = calloc(4,sizeof(abrDraw *));
    collision ** collisionPlayCol = calloc(6,sizeof(collision *));
    collisionPlayCol[1] = collisionQuitCol[0];
    collisionPlayCol[0] = collisionOptionCol[0];
    collisionPlayAbr[0] = ButtonsPlay;
    collisionPlayAbr[1] = Buttons;
    collisionPlayAbr[2] = StartScreen;
    collision collisionPlayPlay = generCol(win,collisionPlayAbr,4,boxColC(36.94,40,23.61,11.67,0),0,collisionPlayCol,6,Box,&StartPlay);
    collisionPlayCol[3] = addColLi(&win->collision[0],collisionPlayPlay);
    collision collisionPlaySimu = generCol(win,collisionPlayAbr,4,boxColC(37.22,55.37,23.61,11.67,0),0,collisionPlayCol,6,Box,&StartSimu);
    collisionPlayCol[4] = addColLi(&win->collision[0],collisionPlaySimu);
    collision collisionPlayRet = generCol(win,collisionPlayAbr,4,boxColC(39.58,72.04,17.92,9.26,0),0,collisionPlayCol,6,Box,&disablePlayMenu);
    collisionPlayCol[5] = addColLi(&win->collision[0],collisionPlayRet);
    collision collisionPlay = generCol(win,collisionPlayAbr,4,boxColC(36.94,40,23.61,11.67,0),1,collisionPlayCol,6,Box,&enablePlayMenu);
    collisionPlayCol[2] = addColLi(&win->collision[0],collisionPlay);
    collisionOptionCol[3] = collisionPlayCol[2];

    updateTextAbrDraw(win->abr, win);
    win->reDraw = 0;
}

void enableOptionStartScreen(int x,int y, collision * c){
    (void)x;
    (void)y;
    c->abr[0]->active = 0;
    requestReDraw(c->win,c->abr[0]);
    c->abr[1]->active = 1;
    requestReDraw(c->win,c->abr[1]);
    c->active = 0;
    c->linkedColli[0]->active = 0;
    c->linkedColli[1]->active = 1;
    c->linkedColli[2]->active = 0;
    c->linkedColli[3]->active = 0;
}

void disableOptionStartScreen(int x,int y, collision * c){
    (void)x;
    (void)y;
    c->abr[0]->active = 1;
    requestReDraw(c->win,c->abr[0]);
    c->abr[1]->active = 0;
    requestReDraw(c->win,c->abr[1]);
    c->active = 0;
    c->linkedColli[1]->active = 0;
    c->linkedColli[0]->active = 1;
    c->linkedColli[2]->active = 1;
    c->linkedColli[3]->active = 1;
}

void quitGame(int x,int y, collision * c){
    (void)x;
    (void)y;
    c->win->program_on = 0;
}

void enablePlayMenu(int x,int y, collision * c){
    printf("PLAY MENU\n");
    (void)x;
    (void)y;
    activateAbr(c->abr[0],1);
    requestReDraw(c->win,c->abr[0]);
    activateAbr(c->abr[1],0);
    requestReDraw(c->win,c->abr[1]);
    c->active = 0;
    c->linkedColli[0]->active = 0;
    c->linkedColli[1]->active = 0;
    c->linkedColli[2]->active = 0;
    c->linkedColli[3]->active = 1;
    c->linkedColli[4]->active = 1;
    c->linkedColli[5]->active = 1;
}

void disablePlayMenu(int x,int y, collision * c){
    (void)x;
    (void)y;
    activateAbr(c->abr[0],0);
    requestReDraw(c->win,c->abr[0]);
    activateAbr(c->abr[1],1);
    requestReDraw(c->win,c->abr[1]);
    c->active = 0;
    c->linkedColli[0]->active = 1;
    c->linkedColli[1]->active = 1;
    c->linkedColli[2]->active = 1;
    c->linkedColli[3]->active = 0;
    c->linkedColli[4]->active = 0;
    c->linkedColli[5]->active = 0;
}

void StartPlay(int x,int y, collision * c){
    printf("START PLAY\n");
    (void)x;
    (void)y;
    activateAbr(c->abr[0],1);
    requestReDraw(c->win,c->abr[0]);
    activateAbr(c->abr[1],0);
    requestReDraw(c->win,c->abr[1]);
    activateAbr(c->abr[2],0);
    requestReDraw(c->win,c->abr[2]);
    activateAbr(c->abr[3],1);
    requestReDraw(c->win,c->abr[3]);
    c->active = 0;
    c->linkedColli[0]->active = 0;
    c->linkedColli[1]->active = 0;
    c->linkedColli[2]->active = 0;
    c->linkedColli[3]->active = 0;
    c->linkedColli[4]->active = 0;
    c->linkedColli[5]->active = 0;
}

void StartSimu(int x,int y, collision * c){
    printf("START SIMU");
    (void)x;
    (void)y;
    activateAbr(c->abr[0],1);
    requestReDraw(c->win,c->abr[0]);
    activateAbr(c->abr[1],0);
    requestReDraw(c->win,c->abr[1]);
    activateAbr(c->abr[2],0);
    requestReDraw(c->win,c->abr[2]);
    activateAbr(c->abr[3],1);
    requestReDraw(c->win,c->abr[3]);
    c->active = 0;
    c->linkedColli[0]->active = 0;
    c->linkedColli[1]->active = 0;
    c->linkedColli[2]->active = 0;
    c->linkedColli[3]->active = 0;
    c->linkedColli[4]->active = 0;
    c->linkedColli[5]->active = 0;
}