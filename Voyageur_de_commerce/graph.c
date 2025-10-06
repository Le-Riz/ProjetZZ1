#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <limits.h>
#include <math.h>

#include "graph.h"



void printTab(int * tab,int taille){
    for(int i=0;i<taille;i++){
        printf("%d ",tab[i]);
    }
    printf("\n");
}

double alea(){
  return (double)rand() / ((double)RAND_MAX + 1);
}

double distanceEuclidienne(Case p1, Case p2){
  double deltaX = p2.x - p1.x;
  double deltaY = p2.y - p1.y;
  return sqrt((deltaX * deltaX) + (deltaY * deltaY));
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

int** creationGprime(int** g,int** res,int taille)
{
    int i,j,k;
    for(i=0;i<taille;i++)
    {
        for(j=0;j<i+1;j++)
        {
            if(i==j) g[i][j] = 0;
            else
            {
                if(g[i][j] || g[j][i])
                {
                    res[i][j] = 1;
                    res[j][i] = 1;
                    prec[i][j] = i;
                    prec[j][i] = j;
                }
                else
                {
                    res[i][j] = taille+1;
                    res[j][i] = taille+1;

                }
            }
        }
    }

    for(k=1;k<=taille;k++)
    {
       for(i=0;i<taille;i++)
        {
            for(j=0;j<i;j++)
            {
               if(res[i][j] > res[i][k-1]+res[k-1][j])
               {
                    res[i][j] = res[i][k-1]+res[k-1][j];
                    res[j][i] = res[i][k-1]+res[k-1][j];
                    //prec[i][j] = prec[k][j];
                    //prec[j][i] = prec[k][i];
               }
            }
        }
        
    }

    return res;
}

int distancetot(int** graph,int* list,int l)
{
    printf("\n");
    int res=0;
    for(int i=0;i<l-1;i++) {
        res += graph[list[i]][list[i+1]];
        //printf("%d %d %d %d\n",i,i+1,graph[list[i]][list[i+1]],res);
    }
    res += graph[list[l-1]][list[0]];
    return res;
}

void min_recuisimu(int** graph,int* distopti,int**listopti, int taille)
{
    srand(time(NULL));
    int tempinit =0;
    int* list = calloc(taille,sizeof(int));
    for(int i=0;i<taille;i++) list[i] = i;

    for(int i=0;i<40;i++)
    {
        int x = rand()%taille;
        int y = rand()%taille;
        int sauv = list[x];
        list[x] = list[y];
        list[y] = sauv;
        int nvtemp = distancetot(graph,list,taille);
        if(nvtemp > tempinit) tempinit = nvtemp; 
    }

    
    int dist;
    float temp;temp = (float)tempinit;
    
    while(temp > 0.0001)
    {
        dist = distancetot(graph,list,taille);
        int x = rand()%taille;
        int y = rand()%taille;
        int sauv = list[x];
        list[x] = list[y];
        list[y] = sauv;

        int nvdist = distancetot(graph,list,taille);

        if(nvdist > dist)
        {
            int diff = nvdist - dist;
            int p = (int)(10000*exp(-diff/taille));

            int test = rand()%p;

            if(test > p)
            {
                int sauv = list[x];
                list[x] = list[y];
                list[y] = sauv;
            }
        }
        temp *= 0.999;
    }
    *distopti = dist;
    *listopti = list;
}



void parcourgraphe(int** graph,int* chemin,float* dist,float** pheromone,int taille)
{
    int distinter = 0;

    int dep = rand()%taille; // sommet de depart aléatoire
    chemin[0] = dep;
    
    int suiv;suiv = dep;

    for(int i=1;i<taille;i++)
    {
      suiv = sommetsuivant(pheromone,chemin,i,suiv,taille); // rechercher sommet suivant
        chemin[i] = suiv;
        distinter += graph[chemin[i-1]][chemin[i]];
    }
    *dist = distinter + graph[chemin[T-1]][chemin[0]];
}

int sommetsuivant(float** pheromone,int* debchemin,int iter,int pos, int taille)
{
    int* sommetpossible = calloc(taille-iter,sizeof(int));
    int k=0;
    for(int i=0;i<taille;i++) // recherche des sommets pas encore visites
    {
        int j=0;
        while (j<iter && i!=debchemin[j])
        {
            j++;
        }
        if(j >= iter){
            sommetpossible[k] = i;
            k++;
        }
        
    }
    float totpheromone = 0.;
    for(int i=0;i<k;i++)
    {
        totpheromone += pheromone[pos][sommetpossible[i]];
    }
    float p=0.;
    for(int i=0;i<k;i++)
    {
        p += pheromone[pos][sommetpossible[i]] / totpheromone;
    }
    int y=sommetpossible[k-1];
    float next = (rand()%100)/100.;

    
    for(int i=0;i<k;i++)
    {
      p -= pheromone[pos][sommetpossible[k-i-1]] / totpheromone;
      if(next > p)
      {
	y = sommetpossible[k-i-1];
        
	break;
      }

    }
    free(sommetpossible);
    return y;
}

void ajoutpheromone(float** nvpheromone,int* chemin,float dist,int taille)
{
    for(int i=0;i<taille-1;i++)
    {
        nvpheromone[chemin[i]][chemin[i+1]] = 1/dist;
    }
    nvpheromone[chemin[taille-1]][chemin[0]] = 1/dist;
}

void modifpheromone(float** pheromone,float** nvpheromone,int diffusion,int taille)
{
    for(int i=0;i<taille;i++)
    {
        for(int j=0;j<taille;j++)
        {
            pheromone[i][j] /= diffusion;
            pheromone[i][j] += nvpheromone[i][j]; 
        }
    }
}

void min_fourmi(int** graph,float* distmin,int* cheminopti,int nbVague, int nbFourmis, int taille)
{
  *distmin = INT_MAX;
    float distfait;
    int * cheminfait = calloc(taille,sizeof(int));
    
    float** pheromone = calloc(taille,sizeof(float*)); // Init pheromone et nvpheromone
    float** nvpheromone = calloc(taille,sizeof(float*));
    for(int i=0;i<taille;i++)
    {
        pheromone[i] = calloc(taille,sizeof(float));
        for(int j=0;j<taille;j++) pheromone[i][j] = 1.0;
        nvpheromone[i] = calloc(taille,sizeof(float));
    }

    for(int vague=0;vague<nbVague;vague++) // nb de vagues
    {
        for(int fourmi=0;fourmi<nbFourmis;fourmi++) // voyage des fourmis par vagues
        {
	  parcourgraphe(graph,cheminfait,&distfait,pheromone,taille); // recherche chemin aleatoire biaise par les pheromones
            if(distfait<*distmin) // amelioration chemin opti
            {
                *distmin = distfait;
                for(int i=0;i<taille;i++)
                {
                    cheminopti[i] = cheminfait[i];
                }
            }
            ajoutpheromone(nvpheromone,cheminfait,distfait,taille); // repartition des pheromones
        }
        modifpheromone(pheromone,nvpheromone,2,taille); // ajout du changment de pheromones
    }


    free(cheminfait);
    for(int i=0;i<taille;i++)
    {
        free(pheromone[i]);
        free(nvpheromone[i]);
    }
    free(pheromone);
    free(nvpheromone);
}
/*
int pluscourtchemin(int** graph,int** matdistance,int deb,int fin,int* chemin,int placement)
{
    int sommet = deb;
    int k=0;
    int sommetpotentiel=-1;
    while(sommet != fin)
    {
        int dist = INT_MAX;
        for(int i=0;i<T;i++)
        {
            //printf("sommet:%d i:%d sol:%d,%d\n",sommet,i,graph[sommet][i],graph[i][sommet]);
            if(graph[i][sommet] == 1 || graph[sommet][i] == 1)
            {
                if(matdistance[i][fin]<dist)
                {
                    dist = matdistance[i][fin];
                    sommetpotentiel = i;
                }
                //printf("i:%d fin:%d dist:%d\n",i,fin,matdistance[i][fin]);
            }
        }
        //printf("%d %d\n",k,sommetpotentiel);
        chemin[placement+k] = sommetpotentiel;
        k++;
        sommet = sommetpotentiel;
    }
    return k;
}

int cheminopti(int** graph,int* cheminfin,int* chemindeb,int** graphdist)
{
    cheminfin[0] = chemindeb[0];
    int j=1;
    for(int i=1;i<T;i++)
    {
        if(graph[chemindeb[i-1]][chemindeb[i]] == 1)
        {
            cheminfin[j] = chemindeb[i];
            j++;
        }
        else
        {
            j += pluscourtchemin(graph,graphdist,chemindeb[i-1],chemindeb[i],cheminfin,j);
        }
    }
    cheminfin[j] = cheminfin[0];
    j++;
    return j;
}
*/