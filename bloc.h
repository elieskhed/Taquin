#ifndef BLOC_C
#define BlOC_C

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>

#define BLOC_WIDTH 150
#define BLOC_HEIGH 150

/*
 * Entite bloc:
 * Chaques bloc est identifie par une valeur value,
 * Se placent aux coordonnes (posX, posY),
 * Contient l'image contenue dans surf.
*/
struct bloc{
    SDL_Surface * surf;
    int posX;
    int posY;
    int value;
};
typedef struct bloc bloc;
//Fonction qui renvoie le taquin identité 4*4
bloc ** taquinIdentite();
//libere le taquin 
void freeTaquin(bloc ** taquin);
//Fonction pour écrire un taquin dans le fichier
void writeTaquin(bloc ** taquin, FILE * fichier);
//Fonction pour obtenir le énième taquin du fichier
bloc ** readTaquin(int numTaquin, FILE * fichier, int * X, int * Y);
//Verifie la validite de taquin
short int resolu(bloc ** taquin);
//Fonction de creation de bloc.
void printBloc(SDL_Surface * window, bloc blocActuel);
//Fonction qui melange un ensemble de blocs.
bloc ** melange(int nbDeplacements, int * X, int * Y);
// On déplace la case vide de coordonnées (X,Y).
void deplacement(bloc ** taquin, short int direction, int * X, int * Y);

#endif
