#ifndef __MENU_H__
#define __MENU_H__

#define SPRITESRC "art&sprite/"
#include "SDL.h"

void initStartingScreen(Window * win);

void enableOptionStartScreen(int x,int y, collision * c);

void disableOptionStartScreen(int x,int y, collision * c);

void quitGame(int x,int y, collision * c);

void enablePlayMenu(int x,int y, collision * c);

void disablePlayMenu(int x,int y, collision * c);

void StartPlay(int x,int y, collision * c);

void StartSimu(int x,int y, collision * c);

#endif