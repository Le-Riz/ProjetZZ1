#ifndef __GRAPH_H__
#define __GRAPH_H__

#define T 9


struct Case {
    int x;
    int y;
};
typedef struct Case Case;

struct Points{
    Case * coord;
};
typedef struct Points Points;

void genere(int**, int, int);

void genere_Graph(int**, double, int);

double alea();

double distanceEuclidienne(Case, Case);

float convertAngleToRad(float);

void defineDots(float, float, Points, int, float);

void creationGprime(int**,int**,int,/*int***/);

int distancetot(int**,int*,int);

void min_recuisimu(int**,int*,int**,int);

void parcourgraphe(int**,int*,float*,float**,int);

int sommetsuivant(float**,int*,int,int,int);

void ajoutpheromone(float**,int*,float,int);

void modifpheromone(float**,float**,int,int);

void min_fourmi(int** graph,float* distmin,int* cheminopti,int nbVague, int nbFourmis, int taille);
/*
int pluscourtchemin(int** graph,int** matdistance,int deb,int fin,int* chemin,int placement);

int cheminopti(int** graph,int* cheminfin,int* chemindeb,int** graphdist);
*/
#endif
