#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

/*
gcc test.c -g -lSDL2 -lSDL2_image -lSDL2_ttf -o prog
*/

int* regleenemie;

struct Entite {
    SDL_Point coord;
    int HP;
};typedef struct Entite Entite;


/// @brief Fonction donnant le déplacement à effectuer dans une situation et avec un set de règles possibles donné
/// @param loi set de règles possibles que peut suivre l'entité
/// @param tailleloi taille du set complet de règles
/// @param tailleregle taille d'une seule règle
/// @return la direction qu'il faut suivre
int action(int* loi,int tailleloi,int tailleregle)
{
    int res;
    int nbregle = tailleloi/tailleregle;
    float total = 0.0;
    for(int i =0;i<nbregle;i++)
    {
        total += loi[(i+1)*tailleregle-1]*loi[(i+1)*tailleregle-1];
    } 
    float tirage = (rand()%100) / 100.0;
    float proba = 0.0;
    for(int i =0;i<nbregle;i++)
    {
        proba += loi[(i+1)*tailleregle-1]*loi[(i+1)*tailleregle-1] / total;
        if (proba > tirage)
        {
            res = loi[(i+1)*tailleregle-2];
            proba -= 1.0;
        }
    }
    //printf("%f, %f, %d\n",tirage,proba,res);
    return res;
}


/// @brief Fonction donnant le déplacement à effectuer dans une situation et avec un set de règles donné,
/// cette fonction sert en particulier d'intermédiaire, calculant uniquement les règles pouvant être appliquées dans cette situation
/// @param situation situation de l'entité souhaitant se déplacer
/// @param loi set de règle que peut suivre l'entité
/// @param tailleloi taille du set complet de règles
/// @param tailleregle taille d'une seule règle
/// @return la direction qu'il faut suivre
int mouvement(int* situation, int* loi,int tailleloi,int tailleregle)
{
    //printf("Situation :\n");
    //for(int i=0;i<tailleregle-2;i++) printf("%d, ",situation[i]);
    //printf("\n");
    //printf("Loi :\n");
    /*
    for(int i=0;i<14;i++)
    {
        for(int j=0;j<tailleregle;j++) printf("%d, ",loi[i*tailleregle+j]);
        printf("\n");
    }
    */

    int* reglesutilisables = calloc(tailleloi,sizeof(int));
    int taille = 0;
    
    int nbregle = tailleloi/tailleregle;
    for(int i=0;i<nbregle;i++)
    {
        int possible = 1;
        for(int j=0;j<tailleregle-2;j++)
        {
            if(loi[i*tailleregle+j] != situation[j] && loi[i*tailleregle+j] != -1)
            {
                possible = 0;
            }
        }
        if(possible)
        {
            for(int j=0;j<tailleregle;j++) reglesutilisables[taille+j] = loi[i*tailleregle+j];
            taille += tailleregle;
        }
    }
    int res = action(reglesutilisables,taille,tailleregle);
    /*
    for(int i=0;i<taille;i++) 
    {
        printf("%d, ",reglesutilisables[i]);
    }
    printf("\n");
    printf("mouvement : %d  _________________________________________\n",res);
    */
    
    free(reglesutilisables);

    return res;
}


/// @brief Fonction permetant d'obtenir un ordre aléatoire des premiers entiers
/// @param taille valeur représentant la taille du tableau à renvoyer et donc aussi des combiens premiers entierx mélanger
/// @return un tableau avec les taille premiers entier dans un ordre aléatoire
int * melange(int taille) // penser a free res apres utilisation
{
    int * res = calloc(taille,sizeof(int));
    int i =1;
    while(i < taille)
    {
        int indice = rand()%taille;
        if(res[indice] == 0) 
        {
            res[indice] = i;
            i++;
        }
    }
    return res;
}




/*______________________________________




























_______________________________________*/


/// @brief Fonction initialisant la matrice des tuiles et les différentes variables associées par lecture de fichier
/// @param f Pointeur de fichier
/// @param M Matrice des tuiles
/// @param n Indice des lignes nous permettant de parcourir la matrice des tuiles
/// @param m Indice des colonne nous permettant de parcourir la matrice des tuiles
/// @param mc Pointeur d'entier auquel on y insére dans son contenu le nombre de pièce du niveau
/// @param nbE Pointeur d'entier auquel on y insére dans son contenu le nombre d'ennemi du niveau
/// @param P Pointeur d'un point SDL auquel on y insère les coordonnées initiales du joueur
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

/// @brief Fonction permettant d'initialiser le tableau des textures qui contiendra les textures des chiffres de 1 à 9
/// @param T Tableau des textures vide
/// @param font font contenant la police d'écriture souhaitée
/// @param color La couleur du texte
/// @param renderer rendu SDL
void SetNumbersText(SDL_Texture ** T, TTF_Font * font, SDL_Color color, SDL_Renderer * renderer){ //On sait d'office que le tableau a 10 cases
    char c[2];
    for(int i = 0; i < 10; i++){
        sprintf(c, "%d", i);
        SDL_Surface * S = TTF_RenderText_Solid(font, c, color);
        T[i] = SDL_CreateTextureFromSurface(renderer, S);
        SDL_FreeSurface(S);
    }
}

/// @brief Fonction permettant de dessiner à l'écran un nombre donné en paramètre
/// @param nb Le nombre qu'on souhaite dessiner
/// @param T Tableau de texture contenant les textures des chiffres de 1 à 9
/// @param renderer rendu SDL
/// @param baseX Coordonné en x où apparaît le premier chiffre (on commence par le chiffre le plus à gauche et on ajoute vers la droite)
/// @param baseY Coordonné en y où apparaît le premier chiffre
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

/// @brief Fonction retournant la conséquence d'un mouvement et de savoir s'il y a collision ou non
/// @param M Matrice des tuiles
/// @param P Point SDL nous indiquant la position actuelle du joueur
/// @param c Caractère décrivant le mouvement effectué : 'h' pour haut , 'b' pour bas, 'g' pour gauche et 'd' pour droite
/// @return Retourne un entier décrivant s'il y a collision ou non (0 pour non). Et s'il y a collision, 1 pour contre un mur et 2 contre un ennemi
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


/// @brief Fonction parcourant la matrice des tuiles et dessinant les textures à chaque cases associées
/// @param M Matrice des tuiles
/// @param renderer rendu SDL
/// @param bush Texture du buisson
/// @param rock Texture du rocher
/// @param coin Texture d'une piece
/// @param ennemyJ Texture du slime ennemi
/// @param Tglace Texture du rocher de glace
/// @param KirbyRect Texture du slime joueur
/// @param marge La marge en pixel du premier bord noir
/// @param caseW Taille en largeur d'une case affiché à l'écran
/// @param caseH Taille en hauteur d'une case affiché à l'écran
/// @param currentFrame Frame courante de l'animation des sprites des slimes
/// @param ligne Indice en ligne nous permettant de parcourir la matrice
/// @param colonne Indice en colonne nous permettant de parcourir la matrice
/// @param currLevel Entier nous indiquant quel texture de mur générer
void drawTexture(int ** M, SDL_Renderer* renderer, SDL_Texture* bush, SDL_Texture* rock, SDL_Texture* coin, SDL_Texture* ennemyJ, SDL_Texture* Tglace, SDL_Rect* KirbyRect, float marge, float caseW, float caseH, int currentFrame, int ligne, int colonne, int currLevel){

    SDL_Texture * wall;

    if(currLevel < 3){
        wall = bush;
    }
    else if((currLevel>=3)&&(currLevel<5)){
        wall = rock;
    }
    else{
        wall = Tglace;
    }
    for(int i = 1; i <= ligne; i++){
        for(int j = 1 ; j <= colonne; j++){
            if(M[i][j] == 0){
                SDL_Rect where = {marge + (((j-1)) * caseW) , ((i-1)) * caseH, caseW, caseH};
                SDL_RenderCopy(renderer, wall, NULL, &where);
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

/// @brief Fonction permettant de dessiner à l'écran l'issus de la partie en cas de victoire ou de défaite
/// @param renderer rendu SDL
/// @param GO Texture de "GAME OVER!"
/// @param WIN Texture de "YOU WIN!"
/// @param done Pointeur de booléen SDL auquel on change la valeur de son contenu afin de mettre un terme au jeu
/// @param gw Largeur de la texture "GAME OVER!"
/// @param gh Hauteur de la texture "GAME OVER!"
/// @param ww Largeur de la texture "YOU WIN!"
/// @param wh Hauteur de la texture "YOU WIN!"
/// @param dx Largeur de l'écran
/// @param dy Hauteur de l'écran
/// @param c Caractère nous donnant l'issus de la partie : 'L' pour Lose et 'W' pour Win
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



/*____________________________________































____________________________*/

/// @brief Procédure faisant bouger l'énemie selon un set de règles défini
/// @param M matrice du terrin
/// @param E tableau des énemies
/// @param player position du joueur
/// @param nbE nombre d'énemies
/// @param HP points de vie du joueur
void EnnemyMakesMove(int** M, Entite* E, SDL_Point player, int nbE, int* HP)
{
    for(int i = 0; i < nbE; i++)
    {
        int* situation = calloc(6,sizeof(int));

        // gestion des murs
        if(M[E[i].coord.x-1][E[i].coord.y] != 0) situation[0] = 1;
        if(M[E[i].coord.x+1][E[i].coord.y] != 0) situation[1] = 1;
        if(M[E[i].coord.x][E[i].coord.y+1] != 0) situation[2] = 1;
        if(M[E[i].coord.x][E[i].coord.y-1] != 0) situation[3] = 1;

        //gestion direction joueur
        int difx = E[i].coord.x - player.x;
        int dify = E[i].coord.y - player.y;
        if(difx*difx > dify*dify)
        {
            if(difx > 0) situation[4] = 0;
            else situation[4] = 1;
        }
        else
        {
            if(dify > 0) situation[4] = 3;
            else situation[4] = 2;
        }

        //gestion distance joueur
        if(difx*difx+dify*dify < 16) situation[5] = 1;

        int deplacement = mouvement(situation,regleenemie,64,8);
        switch(deplacement)
        {
        case 0:
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

        case 1:
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

        case 2:
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

        case 3:
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
        
        default:
            break;
        }
        free(situation);
    }
}

/// @brief Fonction donnant la direction vers où est la pièce la plus proche
/// @param Px coordonée en x du joueur
/// @param Py coordonée en y du joueur
/// @param map matrice du terrin de jeu
/// @param ligne nombre de lignes du terrin
/// @param colonne nombre de colonnes du terrin
/// @return entier représentant la direction de la pièce la plus proche
int distancepiece(int Px,int Py,int** map,int ligne,int colonne)
{
    int dist = INT_MAX;
    int res = 0;
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            if(map[i][j] == 2)
            {
                int difx = Px - i;
                int dify = Py - j;
                if(difx*difx + dify*dify < dist)
                {
                    if(difx*difx > dify*dify)
                    {
                        if(difx > 0) res = 0;
                        else res = 1;
                    }
                    else
                    {
                        if(dify > 0) res = 3;
                        else res = 2;
                    }
                    dist = difx*difx + dify*dify;
                }
            }
        }
    }
    return res;
}


/// @brief Procedure permettant d'obtenir la différence de coordonées entre le joueur et l'énemie le plus proche
/// @param map matrice du terrin de jeu
/// @param Px coordonée en x du joueur
/// @param Py coordonée en y du joueur
/// @param resx différence de position en x
/// @param resy différence de position en y
/// @param ligne nombre de lignes du terrin
/// @param colonne nombre de colonnes du terrin
void coordenemieplusproche(int** map,int Px,int Py,int* resx,int* resy,int ligne,int colonne)
{
    int dist = INT_MAX;
    *resx = 200;
    *resy = 200; 
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            if(map[i][j] == 4)
            {
                int difx = Px - i;
                int dify = Py - j;
                if(difx*difx + dify*dify < dist)
                {
                    *resx = difx;
                    *resy = dify;
                    dist = difx*difx + dify*dify;
                }
            }
        }
    }
}



/*_______________________________





































______________________________*/


/// @brief Fonction qui lance une simulation du Jeu en enregistrant le score réalisé par l'IA
/// @param regleJ Tableau contenant les règles
/// @param deplacementmax Nombre de déplacement maximal par simulation
/// @param main_win fenêtre principale SDL
/// @param font font nous permettant d'avoir une police d'écriture RetroGaming
/// @param renderer rendu SDL
/// @param Tbuisson Texture du buisson
/// @param Trocher Texture du rocher
/// @param Tcoin Texture d'une piece
/// @param TEnnemyJ Texture du slime ennemi
/// @param tx Texture du caractère 'x'
/// @param THP Texture du mot "HP"
/// @param Tover Texture de "GAME OVER!"
/// @param Twin Texture de "YOU WIN!"
/// @param backgroundText Texture du fond de la carte
/// @param KirbyText Texture du slime joueur
/// @param Tglace Texture du rocher de glace
/// @return Retourne un flottant nous donnant le score réalisé par l'IA 
float simulationJeu(int* regleJ,int deplacementmax,SDL_Window * main_win, TTF_Font * font, SDL_Renderer* renderer, SDL_Texture* Tbuisson, SDL_Texture* Trocher, SDL_Texture* Tcoin, SDL_Texture* TEnnemyJ, SDL_Texture* tx, SDL_Texture* THP, SDL_Texture* Tover, SDL_Texture* Twin, SDL_Texture* backgroundText, SDL_Texture* KirbyText,SDL_Texture* Tglace){
    int nbdeplacement = 0;
    float nbdeplacementdernierepiece = 1.0;
    
    FILE * file = fopen("../../chef_doeuvre/maps/maptext.txt", "r");
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

    int currLevel = 1 ;//+ (rand() % 6);

   
    SDL_Color color = {255, 255, 255, 255}; // Couleur du texte (noir)

    //On capture les dimensions de l'écran du PC
    SDL_DisplayMode displayMode;
    SDL_GetDesktopDisplayMode(0, &displayMode);


    int xw,xh;
    SDL_QueryTexture(tx, NULL, NULL, &xw, &xh);

    int hpw,hph;
    SDL_QueryTexture(THP, NULL, NULL, &hpw, &hph);
    SDL_Rect HPRect;

    int overw,overh;
    SDL_QueryTexture(Tover, NULL, NULL, &overw, &overh);

    int winw,winh;
    SDL_QueryTexture(Tover, NULL, NULL, &winw, &winh);


    //Variables définissant les dimensions de chaque frame du sprite
    int nbFrame = 11;
    int bkgW,bkgH,KirW,KirH;
    SDL_QueryTexture(backgroundText, NULL, NULL, &bkgW, &bkgH);
    SDL_QueryTexture(KirbyText, NULL, NULL, &KirW, &KirH);
    int BACKG_WIDTH = bkgW/4;
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
    fclose(file);
    

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
    Entite * enemies;
    if(nbEnnemy > 0){
        enemies = (Entite *) malloc(sizeof(Entite)*nbEnnemy);
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
    }    
    SDL_Event event;
    int current_frame = 0;
    int FPS = 60;
    int FRAME_DELAY = 1000/FPS;
    int largeur,hauteur;
    SDL_GetWindowSize(main_win, &largeur, &hauteur);
    int frameTime,frameStart;

    int caseW = BACKG_WIDTH/28;
    int caseH = BACKG_HEIGHT/28;

    float marge = (float)(displayMode.w - displayMode.h)/2.0;

    SDL_Texture * numbers[10];

    SetNumbersText(numbers, font, color, renderer); 

    int ln,hn;
    SDL_QueryTexture(numbers[0], NULL, NULL, &ln, &hn);
    SDL_Rect backgRect;
    int startTime = SDL_GetTicks();
    int moveTime = SDL_GetTicks();
    int currTime;
    int coins = 0;
    //int offsetX=0;
    //int offsetY=0;
    //int state=1; //Dans quel fragment de la carte on se situe
    int PlayerHP = 10;
    int maxHealth = PlayerHP;
    SDL_bool done = SDL_FALSE;
    int offsetBKG = 0;
    SDL_bool changeLvl = SDL_TRUE;
    
    while(en_route && nbdeplacement < deplacementmax){
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

        if(coins == maxCoins && currLevel < 6){
            currLevel++;
            changeLvl = SDL_TRUE;
        }
        
        if(changeLvl == SDL_TRUE){
            changeLvl = SDL_FALSE;
            //printf("currlevel : %d\n", currLevel);
            FILE * f;
            switch (currLevel)
            {
            case 1:
                f = fopen("../../chef_doeuvre/maps/mapM1N1.txt", "r");
                break;
            case 2:
                f = fopen("../../chef_doeuvre/maps/mapM1N2.txt", "r");
                break;
            case 3:
                f = fopen("../../chef_doeuvre/maps/mapM2N1.txt", "r");
                offsetBKG = bkgW/4;
                break;
            case 4:
                f = fopen("../../chef_doeuvre/maps/mapM2N2.txt", "r");
                offsetBKG = bkgW/4;
                break;
            case 5:
                f = fopen("../../chef_doeuvre/maps/mapM3N1.txt", "r");
                offsetBKG = bkgW/2;
                break;
            case 6:
                f = fopen("../../chef_doeuvre/maps/mapM3N2.txt", "r");
                offsetBKG = bkgW/2;
                break;
            default:
                break;
            }
            //Libère la matrice avant de la réinitialiser
            for(int i = 0; i < ligne; i++){
                free(MTile[i]);
            }
            free(MTile);
            MTile = NULL;
            //Réinitialisation de la matrice
            if(fscanf(f, "%d %d\n", &ligne, &colonne) == 0){
                printf("Erreur lors de la lecture du fichier !\n");
                exit(EXIT_FAILURE);
            }
            ligne += 2;
            colonne += 2;
            MTile = (int **) calloc(sizeof(int*), ligne);
            for(int i = 0; i < ligne; i++){
                MTile[i] = (int*) calloc(sizeof(int), colonne);
            }
            maxCoins = 0;
            coins = 0;
            PlayerHP = 10;
            //On libère les ennemies s'il y en avait avant
            if(nbEnnemy > 0){
                free(enemies);
                enemies = NULL;
                nbEnnemy = 0;
            }
            initMatTfromFile(f, MTile, ligne-2, colonne-2, &maxCoins, &nbEnnemy, &currPos);
            fclose(f);
            //printf("nbEnnemy = %d\n", nbEnnemy);
            //On réinitialise les enemies s'il y en a
            if(nbEnnemy > 0){
                enemies = (Entite *) malloc(sizeof(Entite)*nbEnnemy);
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
            }
        }

        
        //Move Ennemie
        if(nbEnnemy > 0){
            if(currTime-startTime >= 30){
                EnnemyMakesMove(MTile, enemies, currPos, nbEnnemy, &PlayerHP);
                startTime = currTime;
            }
        }

        // Deplacement joueur
        
        //Scroll vers la droite
        if((keyboardState[SDL_SCANCODE_RIGHT]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'd');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x][currPos.y+1] == 2){
                    coins++;
                }
                MTile[currPos.x][currPos.y+1] = 1;
                currPos.y++;
            }
            else if(res == 2){
                PlayerHP--;
                //Anim
            }
            current_frame = (current_frame + 1) % nbFrame;
            moveTime = currTime;
            flip = SDL_FLIP_NONE;
        }

        //Scroll vers la gauche
        if((keyboardState[SDL_SCANCODE_LEFT]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'g');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x][currPos.y-1] == 2){
                    coins++;
                }
                MTile[currPos.x][currPos.y-1] = 1;
                currPos.y--;
            }
            else if(res == 2){
                PlayerHP--;
                //Anim
            }
            moveTime = currTime;
            flip = SDL_FLIP_HORIZONTAL;
        }

        //Scroll vers le haut
        if((keyboardState[SDL_SCANCODE_UP]) && (currTime - moveTime >= 100)){
            int res = voisinnageCollis(MTile, currPos, 'h');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x-1][currPos.y] == 2){
                    coins++;
                }
                MTile[currPos.x-1][currPos.y] = 1;
                currPos.x--;
            }
            else if(res == 2){
                PlayerHP--;
                //Anim
            }
            moveTime = currTime;
        }

        //Scroll vers le bas
        if((keyboardState[SDL_SCANCODE_DOWN]) && (currTime - moveTime >= 100)){
            
            int res = voisinnageCollis(MTile, currPos, 'b');
            if(res == 0){
                MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                if(MTile[currPos.x+1][currPos.y] == 2){
                    coins++;
                }
                MTile[currPos.x+1][currPos.y] = 1;
                currPos.x++;
            }
            else if(res == 2){
                PlayerHP--;
                //Anim
            }
            moveTime = currTime;
        }
        
        
        int* situation = calloc(7,sizeof(int));

        // gestion des murs
        if(MTile[currPos.x-1][currPos.y] != 0) situation[0] = 1;
        if(MTile[currPos.x+1][currPos.y] != 0) situation[1] = 1;
        if(MTile[currPos.x][currPos.y+1] != 0) situation[2] = 1;
        if(MTile[currPos.x][currPos.y-1] != 0) situation[3] = 1;

        //gestion distance piece
        situation[4] = distancepiece(currPos.x,currPos.y,MTile,ligne,colonne);

        //gestion distance enemi le plus proche
        int difx;
        int dify;
        coordenemieplusproche(MTile,currPos.x,currPos.y,&difx,&dify,ligne,colonne);

        if(difx*difx+dify*dify < 16) situation[5] = 1;
        
        //gestion direction enemi
        if(difx*difx > dify*dify)
        {
            if(difx > 0) situation[6] = 0;
            else situation[6] = 1;
        }
        else
        {
            if(dify > 0) situation[6] = 3;
            else situation[6] = 2;
        }
        
        //choix mouvement
        int deplacement = mouvement(situation,regleJ,90+36,9);
        free(situation);

        nbdeplacement++;
        int rescol;

        if(currTime - moveTime >= 20)
        {
            moveTime = currTime;
            switch(deplacement)
            {
            case 0: // aller en haut
                rescol = voisinnageCollis(MTile, currPos, 'h');
                if(rescol == 0){
                    MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                    if(MTile[currPos.x-1][currPos.y] == 2){
                        coins++;
                        nbdeplacementdernierepiece = nbdeplacement;
                    }
                    MTile[currPos.x-1][currPos.y] = 1;
                    currPos.x--;
                }
                else if(rescol == 2){
                    PlayerHP--;
                    //Anim
                }
                moveTime = currTime;
                break;

            case 1: // aller en bas
                rescol = voisinnageCollis(MTile, currPos, 'b');
                if(rescol == 0){
                    MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                    if(MTile[currPos.x+1][currPos.y] == 2){
                        coins++;
                        nbdeplacementdernierepiece = nbdeplacement;
                    }
                    MTile[currPos.x+1][currPos.y] = 1;
                    currPos.x++;
                }
                else if(rescol == 2){
                    PlayerHP--;
                    //Anim
                }
                moveTime = currTime;
                break;

            case 2: // aller a droite
            rescol = voisinnageCollis(MTile, currPos, 'd');
                if(rescol == 0){
                    MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                    if(MTile[currPos.x][currPos.y+1] == 2){
                        coins++;
                        nbdeplacementdernierepiece = nbdeplacement;
                    }
                    MTile[currPos.x][currPos.y+1] = 1;
                    currPos.y++;
                }
                else if(rescol == 2){
                    PlayerHP--;
                    //Anim
                }
                current_frame = (current_frame + 1) % nbFrame;
                moveTime = currTime;
                flip = SDL_FLIP_NONE;
                break;

            case 3: // aller a gauche
                rescol = voisinnageCollis(MTile, currPos, 'g');
                if(rescol == 0){
                    MTile[currPos.x][currPos.y] = 3; //case auquelle le joueur se situait avant son déplancement => libre
                    if(MTile[currPos.x][currPos.y-1] == 2){
                        coins++;
                        nbdeplacementdernierepiece = nbdeplacement;
                    }
                    MTile[currPos.x][currPos.y-1] = 1;
                    currPos.y--;
                }
                else if(rescol == 2){
                    PlayerHP--;
                    //Anim
                }
                moveTime = currTime;
                flip = SDL_FLIP_HORIZONTAL;
                break;
            
            default:
                break;
            }

        }
        

        frameTime = SDL_GetTicks() - frameStart;
        

        backgRect.x = offsetBKG;
        backgRect.y = 0;
        backgRect.w = caseW * (colonne-2);
        backgRect.h = caseH * (ligne-2);
        SDL_Rect whereisBackg = {(displayMode.w/2)-(displayMode.h/2),0, displayMode.h, displayMode.h};
        SDL_RenderCopyEx(renderer, backgroundText, &backgRect, &whereisBackg, 0, NULL, SDL_FLIP_NONE); //Background

        float newCaseW = (float)displayMode.h / (float)(colonne-2);
        float newCaseH = (float)displayMode.h / (float)(ligne-2);
       
        /*Kirby*/
        SDL_Rect whereisKirby = {marge + (((currPos.y-1) ) * newCaseW), ((currPos.x-1)) * newCaseH, newCaseW, newCaseH};
        SDL_RenderCopyEx(renderer, KirbyText, &KirbyRect[current_frame], &whereisKirby, 0, NULL, flip);

        /*Dessin des texture*/
        drawTexture(MTile, renderer, Tbuisson, Trocher, Tcoin, TEnnemyJ, Tglace, KirbyRect, marge, newCaseW, newCaseH, current_frame, ligne-2, colonne-2, currLevel);


        current_frame = (current_frame + 1) % nbFrame;

       /*Pièces*/
        SDL_Rect RNbCoin = {0, 0, displayMode.w/24, displayMode.h/16};
        SDL_Rect rx = {displayMode.w/24, (displayMode.h/16)-xh, xw, xh};
        SDL_RenderCopy(renderer, Tcoin, NULL, &RNbCoin);
        SDL_RenderCopy(renderer, tx, NULL, &rx);
        drawNumber(coins, numbers, renderer, (displayMode.w/24)+xw+10, displayMode.h/64);

        
        //Barre d'HP
        SDL_Rect whereHP = {0, displayMode.h/16, displayMode.w/24, displayMode.h/16};
        HPRect.x = 10 + displayMode.w/24;
        HPRect.y = displayMode.h/16;
        if(PlayerHP >= 0){
            HPRect.w = (displayMode.w/8)*PlayerHP/maxHealth;
        }
        else{
            HPRect.w = 0;
        }
        HPRect.h = displayMode.h/16;
        SDL_RenderCopy(renderer, THP, NULL, &whereHP);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &HPRect);

        /*Win ? Game Over?*/
        if(PlayerHP <= 0){
            drawGameOver(renderer, Tover, Twin, &done, overw, overh, winw, winh, displayMode.w, displayMode.h, 'L');
        }
        else if((coins == maxCoins) && (currLevel == 6)){
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
    if(nbEnnemy > 0)
    {
        free(enemies);
    }
    for(int i = 0; i < ligne; i++){
        free(MTile[i]);
    }
    free(MTile);
    for(int i = 0; i < 10; i++){
        SDL_DestroyTexture(numbers[i]);
    }
  
    return 1/nbdeplacementdernierepiece + PlayerHP;
}



/// @brief Procédure appliquant une méthode gloutone d'optimisation sur les règles fournies
/// @param regles lot de règles initiales
/// @param indice indice du coéficient à modifier dans la méthode
/// @param indiceregle indice de la règle où le coéficient vas être modifié
/// @param donneeVecteur tableau donnant les valeurs possibles pour chacun des coéficients d'une règle
/// @param main_win fenêtre principale SDL
/// @param font font nous permettant d'avoir une police d'écriture RetroGaming
/// @param renderer rendu SDL
/// @param Tbuisson Texture du buisson
/// @param Trocher Texture du rocher
/// @param Tcoin Texture d'une piece
/// @param TEnnemyJ Texture du slime ennemi
/// @param tx Texture du caractère 'x'
/// @param THP Texture du mot "HP"
/// @param Tover Texture de "GAME OVER!"
/// @param Twin Texture de "YOU WIN!"
/// @param backgroundText Texture du fond de la carte
/// @param KirbyText Texture du slime joueur
/// @param Tglace Texture du rocher de glace 
void min_glouton(int* regles,int indice,int indiceregle,int ** donneeVecteur,SDL_Window * main_win, TTF_Font * font, SDL_Renderer* renderer, SDL_Texture* Tbuisson, SDL_Texture* Trocher, SDL_Texture* Tcoin, SDL_Texture* TEnnemyJ, SDL_Texture* tx, SDL_Texture* THP, SDL_Texture* Tover, SDL_Texture* Twin, SDL_Texture* backgroundText, SDL_Texture* KirbyText,SDL_Texture* Tglace)
{
    int res,val,test; val = INT_MAX;
    int taille = donneeVecteur[indice][0];
    for(int i=1;i<taille;i++)
    {
        regles[indiceregle+indice] = donneeVecteur[indice][i];
        test = simulationJeu(regles,20000,main_win, font, renderer, Tbuisson, Trocher, Tcoin, TEnnemyJ, tx, THP, Tover, Twin, backgroundText, KirbyText,Tglace);
        if(test < val)
        {
            val = test;
            res = donneeVecteur[indice][i];
        }
    }
    regles[indiceregle+indice] = res;
}




int main(int argc, char ** argv)
{
    (void) argc;
    (void) argv;

    SDL_Color color = {255, 255, 255, 255};

    //Fenêtre principale
    SDL_Window * main_win = NULL;

    //Initialisation de la SDL
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDL_Log("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    //Initialisation TTF pour générer un texte
    TTF_Init();

    TTF_Font* font = TTF_OpenFont("../../arcade/RetroGaming.ttf", 32);

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
    SDL_Surface* backgroundSurf = IMG_Load("../../Limule's Adventure/art&sprite/mapprojet.png");

    //Création de la texture du sprite
    SDL_Surface* KirbySurf = IMG_Load("../../Limule's Adventure/art&sprite/slimebleuprojet.png");
    SDL_Surface* EnnemyJ = IMG_Load("../../Limule's Adventure/art&sprite/slimeprojet.png");

    //Création texture buisson, pièce et rocher
    SDL_Surface* Sbuisson = IMG_Load("../../Limule's Adventure/art&sprite/bushprojet.png");
    SDL_Surface* Srocher = IMG_Load("../../Limule's Adventure/art&sprite/rocherprojet.png");
    SDL_Surface* Sglace = IMG_Load("../../Limule's Adventure/art&sprite/glaceprojet.png");
    SDL_Surface* Scoin = IMG_Load("../../Limule's Adventure/art&sprite/pieceprojet.png");


    SDL_Texture* Tbuisson = SDL_CreateTextureFromSurface(renderer, Sbuisson);
    SDL_Texture* Trocher = SDL_CreateTextureFromSurface(renderer, Srocher);
    SDL_Texture* Tglace = SDL_CreateTextureFromSurface(renderer, Sglace);
    SDL_Texture* Tcoin = SDL_CreateTextureFromSurface(renderer, Scoin);
    SDL_Texture* TEnnemyJ = SDL_CreateTextureFromSurface(renderer, EnnemyJ);

    SDL_FreeSurface(Sbuisson);
    SDL_FreeSurface(Srocher);
    SDL_FreeSurface(Sglace);
    SDL_FreeSurface(Scoin);
    SDL_FreeSurface(EnnemyJ);


    //Création d'un caractère 'x'
    SDL_Surface * S = TTF_RenderText_Solid(font, "x", color);
    SDL_Texture* tx = SDL_CreateTextureFromSurface(renderer, S);
    SDL_FreeSurface(S);

    
    //Création du mot "HP" et de la barre associé
    SDL_Surface * SHP = TTF_RenderText_Solid(font, "HP", color);
    SDL_Texture* THP = SDL_CreateTextureFromSurface(renderer, SHP);
    SDL_FreeSurface(SHP);
    

    //Création du mot "Game Over!"
    SDL_Surface * Sover = TTF_RenderText_Solid(font, "GAME OVER!", color);
    SDL_Texture* Tover = SDL_CreateTextureFromSurface(renderer, Sover);
    SDL_FreeSurface(Sover);


    //Création du mot "YOU WIN!"
    SDL_Surface * Swin = TTF_RenderText_Solid(font, "YOU WIN!", color);
    SDL_Texture* Twin = SDL_CreateTextureFromSurface(renderer, Swin);
    SDL_FreeSurface(Swin);



    SDL_Texture* backgroundText = SDL_CreateTextureFromSurface(renderer, backgroundSurf);
    SDL_Texture* KirbyText = SDL_CreateTextureFromSurface(renderer, KirbySurf);
    SDL_FreeSurface(backgroundSurf);
    SDL_FreeSurface(KirbySurf);

    // generation des regles des enemies
    srand(time(NULL));

    regleenemie = calloc(64,sizeof(int));

    regleenemie[0] = 1;regleenemie[1] = -1;regleenemie[2] = -1;regleenemie[3] = -1;regleenemie[4] = 0;regleenemie[5] = 1;regleenemie[6] = 0;regleenemie[7] = 5;
    regleenemie[8] = -1;regleenemie[9] = 1;regleenemie[10] = -1;regleenemie[11] = -1;regleenemie[12] = 1;regleenemie[13] = 1;regleenemie[14] = 1;regleenemie[15] = 5;
    regleenemie[16] = -1;regleenemie[17] = -1;regleenemie[18] = 1;regleenemie[19] = -1;regleenemie[20] = 2;regleenemie[21] = 1;regleenemie[22] = 2;regleenemie[23] = 5;
    regleenemie[24] = -1;regleenemie[25] = -1;regleenemie[26] = -1;regleenemie[27] = 1;regleenemie[28] = 3;regleenemie[29] = -1;regleenemie[30] = 3;regleenemie[31] = 5;
    regleenemie[32] = 1;regleenemie[33] = -1;regleenemie[34] = -1;regleenemie[35] = -1;regleenemie[36] = -1;regleenemie[37] = -1;regleenemie[38] = 0;regleenemie[39] = 1;
    regleenemie[40] = -1;regleenemie[41] = 1;regleenemie[42] = -1;regleenemie[43] = -1;regleenemie[44] = -1;regleenemie[45] = -1;regleenemie[46] = 1;regleenemie[47] = 1;
    regleenemie[48] = -1;regleenemie[49] = -1;regleenemie[50] = 1;regleenemie[51] = -1;regleenemie[52] = -1;regleenemie[53] = -1;regleenemie[54] = 2;regleenemie[55] = 1;
    regleenemie[56] = -1;regleenemie[57] = -1;regleenemie[58] = -1;regleenemie[59] = 1;regleenemie[60] = -1;regleenemie[61] = -1;regleenemie[62] = 3;regleenemie[63] = 1;

    // generation aléatoire des regles du joueur
    int* reglesjoueur = calloc(90+36,sizeof(int));

    int valmur,valpiece,proba,direction,priorite,directenemie,distenemie;
    for(int i=0;i<10;i++)
    {
        for(int j=0;j<4;j++)
        {
            proba = rand()%100 / 100.0;
            if(proba < 0.3)
            {
                valmur = rand()%2;
                reglesjoueur[i*9 + j] = valmur;
            }
            else
            {
                reglesjoueur[i*9 + j] = -1;
            }
        }
        proba = rand()%100 / 100.0;
        if(proba < 0.3)
            {
                valpiece = rand()%4;
                reglesjoueur[i*9 + 4] = valpiece;
            }
            else
            {
                reglesjoueur[i*9 + 4] = -1;
            }
        valpiece = rand()%5 -1;
        direction = rand()%4;
        priorite = rand()%5 +1;
        directenemie = rand()%4;
        distenemie = rand()%2;
        reglesjoueur[i*9 + 5] = distenemie;
        reglesjoueur[i*9 + 6] = directenemie;
        reglesjoueur[i*9 + 7] = direction;
        reglesjoueur[i*9 + 8] = priorite;
    }


    reglesjoueur[18]=-1;reglesjoueur[19]=1;reglesjoueur[20]=-1;reglesjoueur[21]=-1;reglesjoueur[22]=-1;reglesjoueur[23]=1;reglesjoueur[24]=0;reglesjoueur[25]=1;reglesjoueur[26]=5;
    reglesjoueur[27]=1;reglesjoueur[28]=-1;reglesjoueur[29]=-1;reglesjoueur[30]=-1;reglesjoueur[31]=-1;reglesjoueur[32]=1;reglesjoueur[33]=1;reglesjoueur[34]=0;reglesjoueur[35]=5;
    reglesjoueur[36]=-1;reglesjoueur[37]=-1;reglesjoueur[38]=-1;reglesjoueur[39]=1;reglesjoueur[40]=-1;reglesjoueur[41]=1;reglesjoueur[42]=2;reglesjoueur[43]=3;reglesjoueur[44]=5;
    reglesjoueur[45]=-1;reglesjoueur[46]=-1;reglesjoueur[47]=1;reglesjoueur[48]=-1;reglesjoueur[49]=-1;reglesjoueur[50]=1;reglesjoueur[51]=3;reglesjoueur[52]=2;reglesjoueur[53]=5;


    reglesjoueur[54]=1;reglesjoueur[55]=-1;reglesjoueur[56]=-1;reglesjoueur[57]=-1;reglesjoueur[58]=0;reglesjoueur[59]=-1;reglesjoueur[60]=-1;reglesjoueur[61]=0;reglesjoueur[62]=3;
    reglesjoueur[63]=-1;reglesjoueur[64]=1;reglesjoueur[65]=-1;reglesjoueur[66]=-1;reglesjoueur[67]=1;reglesjoueur[68]=-1;reglesjoueur[69]=-1;reglesjoueur[70]=1;reglesjoueur[71]=3;
    reglesjoueur[72]=-1;reglesjoueur[73]=-1;reglesjoueur[74]=1;reglesjoueur[75]=-1;reglesjoueur[76]=2;reglesjoueur[77]=-1;reglesjoueur[78]=-1;reglesjoueur[79]=2;reglesjoueur[80]=3;
    reglesjoueur[81]=-1;reglesjoueur[82]=-1;reglesjoueur[83]=-1;reglesjoueur[84]=1;reglesjoueur[85]=3;reglesjoueur[86]=-1;reglesjoueur[87]=-1;reglesjoueur[88]=3;reglesjoueur[89]=3;


    reglesjoueur[90]=1;reglesjoueur[91]=-1;reglesjoueur[92]=-1;reglesjoueur[93]=-1;reglesjoueur[94]=-1;reglesjoueur[95]=-1;reglesjoueur[96]=-1;reglesjoueur[97]=0;reglesjoueur[98]=1;
    reglesjoueur[99]=-1;reglesjoueur[100]=1;reglesjoueur[101]=-1;reglesjoueur[102]=-1;reglesjoueur[103]=-1;reglesjoueur[104]=-1;reglesjoueur[105]=-1;reglesjoueur[106]=1;reglesjoueur[107]=1;
    reglesjoueur[108]=-1;reglesjoueur[109]=-1;reglesjoueur[110]=1;reglesjoueur[111]=-1;reglesjoueur[112]=-1;reglesjoueur[113]=-1;reglesjoueur[114]=-1;reglesjoueur[115]=2;reglesjoueur[116]=1;
    reglesjoueur[117]=-1;reglesjoueur[118]=-1;reglesjoueur[119]=-1;reglesjoueur[120]=1;reglesjoueur[121]=-1;reglesjoueur[122]=-1;reglesjoueur[123]=-1;reglesjoueur[124]=3;reglesjoueur[125]=1;


    // creation des données
    int** donneesVecteur = calloc(9,sizeof(int*));
    for(int i=0;i<9;i++)
    {
        if(i == 4 || i == 6)
        {
            donneesVecteur[i] = calloc(6,sizeof(int));
            donneesVecteur[i][0] = 5;
            for(int j=1;j<6;j++) donneesVecteur[i][j] = j-2;
        }
        else if(i == 7)
        {
            donneesVecteur[i] = calloc(5,sizeof(int));
            donneesVecteur[i][0] = 4;
            for(int j=1;j<5;j++) donneesVecteur[i][j] = j-1;
        }
        else if(i == 8)
        {
            donneesVecteur[i] = calloc(6,sizeof(int));
            donneesVecteur[i][0] = 5;
            for(int j=1;j<6;j++) donneesVecteur[i][j] = j;
        }
        else
        {
            donneesVecteur[i] = calloc(4,sizeof(int));
            donneesVecteur[i][0] = 3;
            for(int j=1;j<4;j++) donneesVecteur[i][j] = j-2;
        }
    }

    /*
    // Optimisation
    int tourner=0;
    while(tourner<5)
    {
        int* ordreregles = melange(6*9);

        for(int i=0;i<6*9;i++)
        {
            min_glouton(reglesjoueur,ordreregles[i]/9,ordreregles[i]%9,donneesVecteur,main_win, font, renderer, Tbuisson, Trocher, Tcoin, TEnnemyJ, tx, THP, Tover, Twin, backgroundText, KirbyText,Tglace);
        }
        free(ordreregles);
        tourner++;
        for(int i=0;i<14;i++)
        {
            for(int j =0;j<9;j++) printf("%d, ",reglesjoueur[i*9+j]);   
            printf("\n");
        }
        printf("\n");
    }
    */

    int* reglesopti = calloc(90+36,sizeof(int));
    //deplacement opti pour enemie
    
    reglesopti[0]=-1;reglesopti[1]=1;reglesopti[2]=-1;reglesopti[3]=-1;reglesopti[4]=-1;reglesopti[5]=1;reglesopti[6]=0;reglesopti[7]=1;reglesopti[8]=5;
    reglesopti[9]=1;reglesopti[10]=-1;reglesopti[11]=-1;reglesopti[12]=-1;reglesopti[13]=-1;reglesopti[14]=1;reglesopti[15]=1;reglesopti[16]=0;reglesopti[17]=5;
    reglesopti[18]=-1;reglesopti[19]=-1;reglesopti[20]=-1;reglesopti[21]=1;reglesopti[22]=-1;reglesopti[23]=1;reglesopti[24]=2;reglesopti[25]=3;reglesopti[26]=5;
    reglesopti[27]=-1;reglesopti[28]=-1;reglesopti[29]=1;reglesopti[30]=-1;reglesopti[31]=-1;reglesopti[32]=1;reglesopti[33]=3;reglesopti[34]=2;reglesopti[35]=5;

    reglesopti[36]=1;reglesopti[37]=-1;reglesopti[38]=-1;reglesopti[39]=-1;reglesopti[40]=1;reglesopti[41]=0;reglesopti[42]=1;reglesopti[43]=0;reglesopti[44]=2;
    reglesopti[45]=-1;reglesopti[46]=-1;reglesopti[47]=-1;reglesopti[48]=1;reglesopti[49]=2;reglesopti[50]=0;reglesopti[51]=2;reglesopti[52]=3;reglesopti[53]=2;
    

    // deplacemet opti pour piece
    reglesopti[54]=1;reglesopti[55]=-1;reglesopti[56]=-1;reglesopti[57]=-1;reglesopti[58]=0;reglesopti[59]=-1;reglesopti[60]=-1;reglesopti[61]=0;reglesopti[62]=3;
    reglesopti[63]=-1;reglesopti[64]=1;reglesopti[65]=-1;reglesopti[66]=-1;reglesopti[67]=1;reglesopti[68]=-1;reglesopti[69]=-1;reglesopti[70]=1;reglesopti[71]=3;
    reglesopti[72]=-1;reglesopti[73]=-1;reglesopti[74]=1;reglesopti[75]=-1;reglesopti[76]=2;reglesopti[77]=-1;reglesopti[78]=-1;reglesopti[79]=2;reglesopti[80]=3;
    reglesopti[81]=-1;reglesopti[82]=-1;reglesopti[83]=-1;reglesopti[84]=1;reglesopti[85]=3;reglesopti[86]=-1;reglesopti[87]=-1;reglesopti[88]=3;reglesopti[89]=3;

    // deplacements random
    reglesopti[90]=1;reglesopti[91]=-1;reglesopti[92]=-1;reglesopti[93]=-1;reglesopti[94]=-1;reglesopti[95]=-1;reglesopti[96]=-1;reglesopti[97]=0;reglesopti[98]=1;
    reglesopti[99]=-1;reglesopti[100]=1;reglesopti[101]=-1;reglesopti[102]=-1;reglesopti[103]=-1;reglesopti[104]=-1;reglesopti[105]=-1;reglesopti[106]=1;reglesopti[107]=1;
    reglesopti[108]=-1;reglesopti[109]=-1;reglesopti[110]=1;reglesopti[111]=-1;reglesopti[112]=-1;reglesopti[113]=-1;reglesopti[114]=-1;reglesopti[115]=2;reglesopti[116]=1;
    reglesopti[117]=-1;reglesopti[118]=-1;reglesopti[119]=-1;reglesopti[120]=1;reglesopti[121]=-1;reglesopti[122]=-1;reglesopti[123]=-1;reglesopti[124]=3;reglesopti[125]=1;


    // Jeu
    simulationJeu(reglesopti,20000,main_win, font, renderer, Tbuisson, Trocher, Tcoin, TEnnemyJ, tx, THP, Tover, Twin, backgroundText, KirbyText,Tglace);

    

    free(regleenemie);
    free(reglesjoueur);
    free(reglesopti);
    for(int i=0;i<9;i++) free(donneesVecteur[i]);
    free(donneesVecteur);

    
    SDL_DestroyTexture(Tbuisson);
    SDL_DestroyTexture(Trocher);
    SDL_DestroyTexture(Tcoin);
    SDL_DestroyTexture(tx);
    SDL_DestroyTexture(backgroundText);
    SDL_DestroyTexture(KirbyText);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(main_win);
    SDL_Quit();

    return 0;
}