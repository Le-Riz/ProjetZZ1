#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "arcade.h"

void initMatTfromFile(FILE * f, int ** M, int n, int m, int * mc, int * nbE, SDL_Point * P){
    if(f&&M){
        int inserer;
        for(int i=1; i<=n; i++){
            for(int j=1; j<=m; j++){
                if((j==m-1)&&(i==n-1)){
                    fscanf(f, "%d", &inserer);
                }
                else if ((j==m-1)&&(i<n-1)){
                    fscanf(f, "%d\n", &inserer);
                }
                else{
                    fscanf(f, "%d ", &inserer);
                }
                M[i][j] = inserer;
                if(inserer == 2){
                    (*mc)++;
                }
                else if (inserer == 4){
                    (*nbE)++;
                }
                else if (inserer == 1){
                    P->x = i;
                    P->y = j;
                }
            }
        }
    }
}

void SetNumbersText(SDL_Texture ** T, TTF_Font * font, SDL_Color color, SDL_Renderer * renderer){ //On sait d'office que le tableau a 10 cases
    char c[2];
    for(int i = 0; i < 10; i++){
        sprintf(c, "%d", i);
        SDL_Surface * S = TTF_RenderText_Solid(font, c, color);
        T[i] = SDL_CreateTextureFromSurface(renderer, S);
        SDL_FreeSurface(S);
    }
}


void drawNumber(int nb, SDL_Texture ** T, SDL_Renderer * renderer, int baseX, int baseY){
    int width,height;
    char str[10];
    SDL_QueryTexture(T[0], NULL, NULL, &width, &height);
    sprintf(str, "%d", nb);
    int i = 0;
    //int baseX = 0 + (width+60);
    while(str[i] != '\0'){
        SDL_Rect R = {baseX + (i*width), baseY, width, height};
        SDL_RenderCopy(renderer, T[str[i] - '0'], NULL, &R);
        i++;
    }

}

int voisinnageCollis(int ** M, SDL_Point P, char c){ //Cette fonction regarde le type de case de la destination d'un mouvement afin de savoir s'il y a collision ou non.
    int voisin;
    int result = -1;
    switch (c)
    {
        case 'h':
            voisin = M[P.x-1][P.y];
            break;
        case 'b':
            voisin = M[P.x+1][P.y];
            break;
        case 'g':
            voisin = M[P.x][P.y-1];
            break;
        case 'd':
            voisin = M[P.x][P.y+1];
            break;
        default:
            break;
    }
    if(voisin == 0){
        result = 1; //Collision, on ne se déplace pas
    }
    else if (voisin == 4){
        result = 2; //Contacte avec un ennemi
    }
    else{
        result = 0; //On se déplace
    }
    return result;
}


void drawTexture(int ** M, int state, SDL_Renderer* renderer, SDL_Texture* bush, SDL_Texture* rock, SDL_Texture* coin, SDL_Texture* ennemyJ, SDL_Rect* KirbyRect, int marge, int caseW, int caseH, int currentFrame, int ligne, int colonne){
    int x = 1;
    if(x!=-1){
        for(int i = 1; i <= ligne; i++){
            for(int j = 1 ; j <= colonne; j++){
                if(M[i][j] == 0){
                    SDL_Rect where = {marge + (((j-1)) * caseW) , ((i-1)) * caseH, caseW, caseH};
                    SDL_RenderCopy(renderer, bush, NULL, &where);
                }
                else if (M[i][j] == 2){
                    SDL_Rect where = {marge + (((j-1)) * caseW) , ((i-1)) * caseH, caseW, caseH};
                    SDL_RenderCopy(renderer, coin, NULL, &where);
                }
                else if (M[i][j] == 4){
                    SDL_Rect whereisEnnemy = {marge + (((j-1)) * caseW), ((i-1)) * caseH, caseW, caseH};
                    SDL_RenderCopyEx(renderer, ennemyJ, &KirbyRect[currentFrame], &whereisEnnemy, 0, NULL, SDL_FLIP_NONE);
                }
            }
        }
    }

}



void makeMove(int ** M, SDL_Point Player, Entite E, char * roll){ //On commence par qq chose de simple.
    int cmt = 0;
    if(Player.x < E.coord.x){
        /*Se déplace vers le haut*/
        roll[cmt] = 'N';
        cmt++;
    }
    else if (Player.x > E.coord.x){
        /*Se déplace vers le bas*/
        roll[cmt] = 'S';
        cmt++;
    }
    else{
        /*Ne se déplace que sur le côté*/
    }

    if(Player.y < E.coord.y){
        /*Se déplace à gauche*/
        roll[cmt] = 'O';
        cmt++;
    }
    else if (Player.y > E.coord.y){
        /*Se déplace à droite*/
        roll[cmt] = 'E';
        cmt++;
    }
    else{
        /*Ne se déplace que à la verticale*/
    }
    roll[cmt] = '\0';
    
}

void EnnemyMakesMove(int ** M, Entite * E, SDL_Point Player, int nbE, int * HP){
    for(int i = 0; i < nbE; i++){
        char roll[4];
        makeMove(M, Player, E[i], roll);
        int j = 0;
        while(roll[j] != '\0'){
            switch (roll[j])
            {
            case 'N':
                if(M[E[i].coord.x - 1][E[i].coord.y] == 3){
                    M[E[i].coord.x][E[i].coord.y] = 3;
                    E[i].coord.x--;
                    M[E[i].coord.x][E[i].coord.y] = 4;
                }
                else if(M[E[i].coord.x - 1][E[i].coord.y] == 1){
                    (*HP)--;
                    /*Animation de dégât subis*/
                }
                break;
            case 'S':
                if(M[E[i].coord.x + 1][E[i].coord.y] == 3){
                    M[E[i].coord.x][E[i].coord.y] = 3;
                    E[i].coord.x++;
                    M[E[i].coord.x][E[i].coord.y] = 4;
                }
                else if(M[E[i].coord.x + 1][E[i].coord.y] == 1){
                    (*HP)--;
                    /*Animation de dégât subis*/
                }
                break;
            case 'O':
                if(M[E[i].coord.x][E[i].coord.y-1] == 3){
                    M[E[i].coord.x][E[i].coord.y] = 3;
                    E[i].coord.y--;
                    M[E[i].coord.x][E[i].coord.y] = 4;
                }
                else if(M[E[i].coord.x][E[i].coord.y-1] == 1){
                    (*HP)--;
                    /*Animation de dégât subis*/
                }
                break;
            case 'E':
                if(M[E[i].coord.x][E[i].coord.y+1] == 3){
                    M[E[i].coord.x][E[i].coord.y] = 3;
                    E[i].coord.y++;
                    M[E[i].coord.x][E[i].coord.y] = 4;
                }
                else if(M[E[i].coord.x][E[i].coord.y+1] == 1){
                    (*HP)--;
                    /*Animation de dégât subis*/
                }
                break;
            default:
                break;
            }
            j++;
        }
    }
}

void PlayerMakeMove(int ** M, SDL_Point * P, Entite * E, int nbE, int ligne, int colonne){
    int S[3]; //[où est preda | où est piece | distance preda]
    int min = 10000; //arbitraire 
    int nearest;
    /*On cherche le prédateur le plus proche*/
    for(int i = 0; i < nbE; i++){
        if(abs(P->x - E[i].coord.x) + abs(P->y - E[i].coord.y) < min){
            min = abs(P->x - E[i].coord.x) + abs(P->y - E[i].coord.y);
            nearest = i;
        }
    }
    /*On détermine la distance, loin, pas loin etc*/
    if(abs(P->x - E[nearest].coord.x) + abs(P->y - E[nearest].coord.y) <= 4){
        S[1] = PROCHE;
    }
    else if((abs(P->x - E[nearest].coord.x) + abs(P->y - E[nearest].coord.y) > 4)&&(abs(P->x - E[nearest].coord.x) + abs(P->y - E[nearest].coord.y) <= 8)){
        S[1] = MOYEN;
    }
    else{
        S[1] = LOIN;
    }

    /*Position du prédateur par rapport au joueur*/
    if(E[nearest].coord.x - P->x < 0){ //au-dessus
        S[0] = NORD;
    }
    else if (E[nearest].coord.x - P->x > 0){ //En dessous
        S[0] = SUD;
    }
    else{
        if(E[nearest].coord.y - P->y < 0){ //gauche
            S[0] = OUEST;
        }
        else if(E[nearest].coord.y - P->y > 0){ //droite
            S[0] = EST;
        }
    }

    /*IDEM avec Piece*/

    /*Recherche de la pièce la plus proche*/
    SDL_Point nearestCoin;
    min = 10000;
    for(int i = 0; i <= ligne; i++){
        for(int j = 0; j <= colonne; j++){
            if(M[i][j] == 2){
                if(abs(P->x - i) + abs(P->y - j) < min){
                    min = abs(P->x - i) + abs(P->y - j);
                    nearestCoin.x = i;
                    nearestCoin.y = j;
                }
            }
        }
    }

    /*Position de la pièce par rapport au joueur*/
    if(nearestCoin.x - P->x < 0){ //au-dessus
        S[2] = NORD;
    }
    else if (nearestCoin.x - P->x > 0){ //En dessous
        S[2] = SUD;
    }
    else{
        if(nearestCoin.y - P->y < 0){ //gauche
            S[2] = OUEST;
        }
        else if(nearestCoin.y - P->y > 0){ //droite
            S[2] = EST;
        }
    }


}

void drawGameOver(SDL_Renderer* renderer, SDL_Texture* GO, SDL_Texture* WIN, SDL_bool * done, int gw, int gh, int ww, int wh, int dx, int dy, char c){
    if(c == 'L'){
        SDL_Rect whereisGO = {(dx/2) - (gw/2), (dy/2) - (gh/2), gw, gh};
        SDL_RenderCopy(renderer, GO, NULL, &whereisGO);
    }
    else{
        SDL_Rect whereisWIN = {(dx/2) - (ww/2), (dy/2) - (wh/2), ww, wh};
        SDL_RenderCopy(renderer, WIN, NULL, &whereisWIN);
    }
    (*done) = SDL_TRUE;
}

int main(int argc, char * argv[]){
    if(argc != 2){
        printf("Erreur, pas le bon nombre d'argument\n");
        exit(EXIT_FAILURE);
    }
    FILE * file = fopen(argv[1], "r");
    if(!file){
        printf("Erreur, lecture du fichier impossible !\n");
        exit(EXIT_FAILURE);
    }
    /*Fichier sous la forme de [nombre de ligne] [nombre de colonne]\n
                                [matrice de collision]*/
    int ligne,colonne;
    if(fscanf(file, "%d %d\n", &ligne, &colonne) == 0){
        printf("Erreur lors de la lecture du fichier !\n");
        exit(EXIT_FAILURE);
    }
    


    //Fenêtre principale
    SDL_Window * main_win = NULL;

    //Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Initialisation TTF pour générer un texte
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("RetroGaming.ttf", 32);
   
    SDL_Color color = {255, 255, 255, 255}; // Couleur du texte (noir)

    //On capture les dimensions de l'écran du PC
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);


    //Création et Paramétrage de la fenêtre principale
    main_win = SDL_CreateWindow("Fenêtre Principale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, displayMode.w, displayMode.h, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);


    //Vérification de la création de cette fenêtre
    if(main_win == NULL){
        SDL_Log("Erreur : échec de la création de la fenêtre : %s\n", SDL_GetError());
        SDL_DestroyWindow(main_win);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    //Création d'un renderer pour mettre à jour notre fenêtre
    SDL_Renderer* renderer = SDL_CreateRenderer(main_win, -1, SDL_RENDERER_ACCELERATED);

    //Création de la texture du fond, on commence par la création d'une surface puis on réalise la texture
    SDL_Surface* backgroundSurf = IMG_Load("../chef_doeuvre/sprites/maparcadeprojet.png");

    //Création de la texture du sprite
    SDL_Surface* KirbySurf = IMG_Load("../chef_doeuvre/sprites/slimebleuprojet.png");
    SDL_Surface* EnnemyJ = IMG_Load("../chef_doeuvre/sprites/slimeprojet.png");

    //Création texture buisson, pièce et rocher
    SDL_Surface* Sbuisson = IMG_Load("../chef_doeuvre/sprites/bushprojet.png");
    SDL_Surface* Srocher = IMG_Load("../chef_doeuvre/sprites/rocherprojet.png");
    SDL_Surface* Scoin = IMG_Load("../chef_doeuvre/sprites/pieceprojet.png");

    SDL_Texture* Tbuisson = SDL_CreateTextureFromSurface(renderer, Sbuisson);
    SDL_Texture* Trocher = SDL_CreateTextureFromSurface(renderer, Srocher);
    SDL_Texture* Tcoin = SDL_CreateTextureFromSurface(renderer, Scoin);
    SDL_Texture* TEnnemyJ = SDL_CreateTextureFromSurface(renderer, EnnemyJ);

    SDL_FreeSurface(Sbuisson);
    SDL_FreeSurface(Srocher);
    SDL_FreeSurface(Scoin);
    SDL_FreeSurface(EnnemyJ);

    //Création d'un caractère 'x'
    SDL_Surface * S = TTF_RenderText_Solid(font, "x", color);
    SDL_Texture* tx = SDL_CreateTextureFromSurface(renderer, S);
    SDL_FreeSurface(S);
    int xw,xh;
    SDL_QueryTexture(tx, NULL, NULL, &xw, &xh);

    //Création du mot "HP" et de la barre associé
    SDL_Surface * SHP = TTF_RenderText_Solid(font, "HP", color);
    SDL_Texture* THP = SDL_CreateTextureFromSurface(renderer, SHP);
    SDL_FreeSurface(SHP);
    int hpw,hph;
    SDL_QueryTexture(THP, NULL, NULL, &hpw, &hph);
    SDL_Rect HPRect;

    //Création du mot "Game Over!"
    SDL_Surface * Sover = TTF_RenderText_Solid(font, "GAME OVER!", color);
    SDL_Texture* Tover = SDL_CreateTextureFromSurface(renderer, Sover);
    SDL_FreeSurface(Sover);
    int overw,overh;
    SDL_QueryTexture(Tover, NULL, NULL, &overw, &overh);

    //Création du mot "YOU WIN!"
    SDL_Surface * Swin = TTF_RenderText_Solid(font, "YOU WIN!", color);
    SDL_Texture* Twin = SDL_CreateTextureFromSurface(renderer, Swin);
    SDL_FreeSurface(Swin);
    int winw,winh;
    SDL_QueryTexture(Tover, NULL, NULL, &winw, &winh);

    //Vérification du chargement de l'image 
    if(backgroundSurf == NULL || KirbySurf == NULL) exit(EXIT_FAILURE);

    SDL_Texture* backgroundText = SDL_CreateTextureFromSurface(renderer, backgroundSurf);
    SDL_Texture* KirbyText = SDL_CreateTextureFromSurface(renderer, KirbySurf);
    SDL_FreeSurface(backgroundSurf);
    SDL_FreeSurface(KirbySurf);

    //Vérification du chargement de la texture
    if((backgroundText == NULL)||(KirbyText == NULL)) exit(EXIT_FAILURE);

    //Variables définissant les dimensions de chaque frame du sprite
    int nbFrame = 11;
    int bkgW,bkgH,KirW,KirH;
    SDL_QueryTexture(backgroundText, NULL, NULL, &bkgW, &bkgH);
    SDL_QueryTexture(KirbyText, NULL, NULL, &KirW, &KirH);
    int BACKG_WIDTH = bkgW/2;
    int BACKG_HEIGHT = bkgH;
    int spriteW = (KirW/nbFrame);
    int spriteH = KirH;
    /*La matrice des tuiles auquelle on rajoute les bourdures de la cartes (ici des 0)*/
    ligne += 2;
    colonne += 2;
    int ** MTile = (int **) calloc(sizeof(int*), ligne);
    for(int i = 0; i < ligne; i++){
        MTile[i] = (int*) calloc(sizeof(int), colonne);
    }
    /*Fonction qui remplit la matrice avec les informations du fichier texte donné en argument de l'exécution du programme*/
    int maxCoins = 0; 
    int nbEnnemy = 0;
    SDL_Point currPos;
    initMatTfromFile(file, MTile, ligne-2, colonne-2, &maxCoins, &nbEnnemy, &currPos);
    
    srand(time(NULL));

    //Création des variables
    SDL_bool en_route = SDL_TRUE;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    SDL_Rect * KirbyRect = (SDL_Rect *) malloc(sizeof(SDL_Rect) * nbFrame);
    for(int i = 0; i < nbFrame; i++){
        KirbyRect[i].x = i * spriteW;
        KirbyRect[i].y = 0;
        KirbyRect[i].w = spriteW;
        KirbyRect[i].h = spriteH;

    }
    //Coordonnées des Enemies
    Entite * enemies = (Entite *) malloc(sizeof(Entite)*nbEnnemy);
    int compteur = 0;
    for(int i = 1; i <= ligne-2; i++){
        for(int j = 1; j <=colonne-2; j++){
            if(MTile[i][j] == 4){
                enemies[compteur].coord.x = i;
                enemies[compteur].coord.y = j;
                enemies[compteur].HP = 1;
                compteur++;
            }
        }
    }
    SDL_Event event;
    int current_frame = 0;
    int FPS = 60;
    int FRAME_DELAY = 1000/FPS;
    int largeur,hauteur;
    SDL_GetWindowSize(main_win, &largeur, &hauteur);
    int frameTime,frameStart;

    int caseW = BACKG_WIDTH/24;
    int caseH = BACKG_HEIGHT/24;
    //int marge = (displayMode.w - displayMode.h)/2;
    int marge = 0;

    SDL_Texture * numbers[10];

    SetNumbersText(numbers, font, color, renderer); 

    int ln,hn;
    SDL_QueryTexture(numbers[0], NULL, NULL, &ln, &hn);
    SDL_Rect backgRect;
    int startTime = SDL_GetTicks();
    int moveTime = SDL_GetTicks();
    int currTime;
    int coins = 0;
    int offsetX=0;
    int offsetY=0;
    int state=1; //Dans quel fragment de la carte on se situe
    int PlayerHP = 10;
    int maxHealth = PlayerHP;
    SDL_bool done = SDL_FALSE;
    
    while(en_route){
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    en_route = SDL_FALSE;
                    break;
                default:
                    break;
            }
        }

        currTime = SDL_GetTicks();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

        
        //Move Ennemie
        if(currTime-startTime >= 1000){
            EnnemyMakesMove(MTile, enemies, currPos, nbEnnemy, &PlayerHP);
            startTime = currTime;
        }

        /*Scroll vers la droite*/
        if((keyboardState[SDL_SCANCODE_RIGHT]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'd');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x][currPos.y+1] == 2){
                    coins++;
                }
                MTile[currPos.x][currPos.y+1] = 1;
                currPos.y++;
                if(currPos.y == 9 || currPos.y == 17){
                    offsetX += BACKG_WIDTH;
                }
            }
            else if(res == 2){
                PlayerHP--;
                /*Anim*/
            }
            current_frame = (current_frame + 1) % nbFrame;
            moveTime = currTime;
            flip = SDL_FLIP_NONE;
        }

        /*Scroll vers la gauche*/
        if((keyboardState[SDL_SCANCODE_LEFT]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'g');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x][currPos.y-1] == 2){
                    coins++;
                }
                MTile[currPos.x][currPos.y-1] = 1;
                currPos.y--;
                if(currPos.y == 8 || currPos.y == 16){
                    offsetX -= BACKG_WIDTH;
                }
            }
            else if(res == 2){
                PlayerHP--;
                /*Anim*/
            }
            moveTime = currTime;
            flip = SDL_FLIP_HORIZONTAL;
        }

        /*Scroll vers le haut*/
        if((keyboardState[SDL_SCANCODE_UP]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'h');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x-1][currPos.y] == 2){
                    coins++;
                }
                MTile[currPos.x-1][currPos.y] = 1;
                currPos.x--;
                if(currPos.x == 8 || currPos.x == 16){
                    offsetY -= BACKG_HEIGHT;
                }
            }
            else if(res == 2){
                PlayerHP--;
                /*Anim*/
            }
            moveTime = currTime;
        }

        /*Scroll vers le bas*/
        if((keyboardState[SDL_SCANCODE_DOWN]) && (currTime - moveTime >= 100)){
            
            int res = voisinnageCollis(MTile, currPos, 'b');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x+1][currPos.y] == 2){
                    coins++;
                }
                MTile[currPos.x+1][currPos.y] = 1;
                currPos.x++;
                if(currPos.x == 9 || currPos.x == 17){
                    offsetY += BACKG_HEIGHT;
                }
            }
            else if(res == 2){
                PlayerHP--;
                /*Anim*/
            }
            moveTime = currTime;
        }

        //PlayerMakeMove(MTile, &currPos, enemies, nbEnnemy, ligne-2, colonne-2);


        frameTime = SDL_GetTicks() - frameStart;
        

        backgRect.x = 0;
        backgRect.y = 0;
        backgRect.w = caseW * (colonne-2);
        backgRect.h = caseH * (ligne-2);
        SDL_Rect whereisBackg = {(displayMode.w/2)-(displayMode.h/2),0, displayMode.h, displayMode.h};
        SDL_RenderCopyEx(renderer, backgroundText, &backgRect, NULL, 0, NULL, SDL_FLIP_NONE); //Background

        /*Dessin des texture*/
        int newCaseW = displayMode.w / (colonne-2);
        int newCaseH = displayMode.h / (ligne-2);
        drawTexture(MTile, state, renderer, Tbuisson, Trocher, Tcoin, TEnnemyJ, KirbyRect, marge, newCaseW, newCaseH, current_frame, ligne-2, colonne-2);
        
        /*Kirby*/
        SDL_Rect whereisKirby = {marge + (((currPos.y-1) ) * newCaseW), ((currPos.x-1)) * newCaseH, newCaseW, newCaseH};
        SDL_RenderCopyEx(renderer, KirbyText, &KirbyRect[current_frame], &whereisKirby, 0, NULL, flip);



        current_frame = (current_frame + 1) % nbFrame;

        /*Pièces*/
        drawNumber(coins, numbers, renderer, caseW+xw, 0);
        SDL_Rect RNbCoin = {0, 0, caseW, caseH};
        SDL_Rect rx = {caseW, caseH-xh, xw, xh};
        SDL_RenderCopy(renderer, Tcoin, NULL, &RNbCoin);
        SDL_RenderCopy(renderer, tx, NULL, &rx);

        /*Barre d'HP*/
        SDL_Rect whereHP = {0, caseH, caseW, caseH};
        HPRect.x = hpw + caseW/4;
        HPRect.y = caseH + (hph/2) - (caseH/2);
        if(PlayerHP >= 0){
            HPRect.w = (displayMode.w/8)*PlayerHP/maxHealth;
        }
        else{
            HPRect.w = 0;
        }
        HPRect.h = newCaseH/2;
        SDL_RenderCopy(renderer, THP, NULL, &whereHP);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &HPRect);

        /*Win ? Game Over?*/
        if(PlayerHP <= 0){
            drawGameOver(renderer, Tover, Twin, &done, overw, overh, winw, winh, displayMode.w, displayMode.h, 'L');
        }
        else if(coins == maxCoins){
            drawGameOver(renderer, Tover, Twin, &done, overw, overh, winw, winh, displayMode.w, displayMode.h, 'W');
        }


        if (frameTime < FRAME_DELAY){
        SDL_Delay(FRAME_DELAY - frameTime);
        }

        SDL_RenderPresent(renderer);

        if(keyboardState[SDL_SCANCODE_ESCAPE]){
            en_route = SDL_FALSE;
        }

        if(done == SDL_TRUE){
            en_route = SDL_FALSE;
            SDL_Delay(2000);
        }

    }

    //Termine le programme
    SDL_Delay(100); 
    for(int i = 0; i < ligne; i++){
        free(MTile[i]);
    }
    free(MTile);
    SDL_DestroyTexture(Tbuisson);
    SDL_DestroyTexture(Trocher);
    SDL_DestroyTexture(Tcoin);
    SDL_DestroyTexture(tx);
    SDL_DestroyTexture(backgroundText);
    SDL_DestroyTexture(KirbyText);
    for(int i = 0; i < 10; i++){
        SDL_DestroyTexture(numbers[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_win);
    SDL_Quit();

    return 0;
}