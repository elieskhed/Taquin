#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <time.h>
#include "bloc.h"

// Fonction pour libérer l'espace alloué pour un taquin
void freeTaquin(bloc ** taquin){

    for (int i = 0; i < 4; i++){

        for (int j = 0; j < 4; j++){
            SDL_FreeSurface(taquin[i][j].surf);
        }
        free(taquin[i]); 
    }

    free(taquin); 
    taquin = NULL; 
}

// Fonction qui renvoie le taquin identité 4*4

bloc ** taquinIdentite(){

    // On déclare un tableau à 2 dimensions de blocs 4*4
    bloc ** taquin = (bloc **) malloc (4 * sizeof(bloc *)); 

    // Variable représentant le nom de l'image à ouvrir
    char * imgName = (char *) malloc(7 * sizeof(char));

    for (int i = 0; i < 4; i++){

        taquin[i] = (bloc *) malloc (4 * sizeof(bloc)); 

        for (int j = 0; j < 4; j++){

            if ((i + j) == 6){

                taquin[i][j].value = 0; 

            } else {

                taquin[i][j].value = 4 * i + j + 1; 

            }

            //taquin[i][j].posX = 400 + (BLOC_WIDTH+10)*j;
            //taquin[i][j].posY = 200 + (BLOC_HEIGH+10)*i;
            taquin[i][j].posX = 150 + (BLOC_WIDTH+10)*j;
            taquin[i][j].posY = 150 + (BLOC_HEIGH+10)*i;
            snprintf(imgName, 7, "%d.bmp", taquin[i][j].value); 
            taquin[i][j].surf = SDL_LoadBMP(imgName); 
        }
    }

    free(imgName); 
    imgName = NULL; 

    return taquin; 
}

// Fonction pour écrire un taquin dans le fichier
void writeTaquin(bloc ** taquin, FILE * fichier){

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            if (taquin[i][j].value > 9){
               fputc('1', fichier); 
            }
            fputc(taquin[i][j].value % 10 + 48, fichier);
            fputc(' ', fichier);
        }
        fputc('\n', fichier); 
    }
    fputc('\n', fichier); 
}

// Fonction pour obtenir le énième taquin du fichier
bloc ** readTaquin(int numTaquin, FILE * fichier, int * X, int * Y){

    // On crée un taquin identité
    bloc ** taquin = taquinIdentite(); 

    char ligne[14]; 

    for (int i = 0; i < (5* (numTaquin -1)); i++){
        fgets(ligne, 14, fichier); 
    }
    
    // On lit les valeurs du fichier pour les mettre dans les valeurs de chaque bloc
    for (int i = 0; i < 4; i++){

        fscanf(fichier, "%d %d %d %d", &taquin[i][0].value, &taquin[i][1].value, &taquin[i][2].value, &taquin[i][3].value); 
        
        for (int j = 0; j < 4; j++){

            snprintf(ligne, 7, "%d.bmp", taquin[i][j].value); 
            taquin[i][j].surf = SDL_LoadBMP(ligne); 
            // Lorsqu'on trouve la case vide on modifie les coordonées X et Y avec ses coordonnées
            if (taquin[i][j].value == 0){
                *X = i; 
                *Y = j; 
            } 

        }
    }

    return taquin; 
}

// On déplace la case vide de coordonnées (X,Y)
void deplacement(bloc ** taquin, short int direction, int * X, int * Y){

    int tempInt; 
    SDL_Surface * tempSurf; 

    // Haut
    if (direction == 0 && *X != 0){
        tempInt = taquin[*X-1][*Y].value; taquin[*X-1][*Y].value = 0; taquin[*X][*Y].value = tempInt; 
        tempSurf = taquin[*X-1][*Y].surf; taquin[*X-1][*Y].surf = taquin[*X][*Y].surf; taquin[*X][*Y].surf = tempSurf; 
        *X -= 1; 
    }

    // Droite
    else if (direction == 1 && *Y != 3){
        tempInt = taquin[*X][*Y+1].value; taquin[*X][*Y+1].value = 0; taquin[*X][*Y].value = tempInt; 
        tempSurf = taquin[*X][*Y+1].surf; taquin[*X][*Y+1].surf = taquin[*X][*Y].surf; taquin[*X][*Y].surf = tempSurf; 
        *Y += 1; 
    }

    // Bas
    else if (direction == 2 && *X != 3){
        tempInt = taquin[*X+1][*Y].value; taquin[*X+1][*Y].value = 0; taquin[*X][*Y].value = tempInt; 
        tempSurf = taquin[*X+1][*Y].surf; taquin[*X+1][*Y].surf = taquin[*X][*Y].surf; taquin[*X][*Y].surf = tempSurf;  
        *X += 1; 
    }

    // Gauche
    else if (direction == 3 && *Y != 0){
        tempInt = taquin[*X][*Y-1].value; taquin[*X][*Y-1].value = 0; taquin[*X][*Y].value = tempInt; 
        tempSurf = taquin[*X][*Y-1].surf; taquin[*X][*Y-1].surf = taquin[*X][*Y].surf; taquin[*X][*Y].surf = tempSurf;
        *Y -= 1; 
    }
}
// Fonction qui crée un rectangle sur la fenêtre
void printBloc(SDL_Surface * window, bloc blocActuel){ 
    SDL_Rect position;
    position.x = blocActuel.posX;
    position.y = blocActuel.posY;

    SDL_BlitSurface(blocActuel.surf, NULL, window, &position);
}
// Fonction qui renvoie un taquin identité modifié de nbDeplacements déplacements
bloc ** melange(int nbDeplacements, int * X, int * Y){
    bloc ** taquin = taquinIdentite(); 
    // Après ces lignes le tableau est initialisé comme le taquin IDENTITÉ 
    // Maintenant on fait des déplacements aléatoires      
    for (short int direction = rand()%4, i = 0; i < nbDeplacements; i++, direction = rand()%4){
        // On regarde si on peut faire un déplacement
        if ((!direction && *X)||(direction == 1 && *Y != 3)||(direction == 2 && *X != 3)||(direction == 3 && *Y)){
            deplacement(taquin, direction, X, Y);
        }
        // Sinon on augmente nbDeplacements car il n'y a pas eu de déplacement à ce tour de boucle
        else {
            nbDeplacements ++; 
        }
    }
    return taquin; 
}

short int resolu(bloc ** taquin){
    short int valide = 1; 

    for (int i = 0; valide && (i < 4); i++){
        for (int j = 0; valide && (j < 4); j++){
            // On regarde si chaque case a bien la bonne valeur 
            // Sinon on change la valeur de la variable valide à 0
            if ((i+j != 6) && (taquin[i][j].value != 4 * i + j + 1)){
                valide = 0; 
            }
        }
    }
    return valide; 
}

