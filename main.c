#include <stdio.h>
#include <stdlib.h>
#include "bloc.h"
#include <time.h>
#include <SDL/SDL.h>

/* JEU DE TAQUIN: 
 * 2 options:
 * f -> écrire/lire un taquin dans un fichier
 * r -> génère un taquin aléatoirement
*/

void mode1(char choice, FILE* file, int optn);


int main(int argc, char ** argv){

  char choice;
  
  printf("Bienvenue, sur le jeu Taquin.\n\n");

  printf("Appuyez sur 'f' si vous comptez utiliser un fichier\n");  
  printf("Appuyez sur 'r' pour une partie rapide, 'q' pour quitter.\n");
  scanf("%c", &choice);

  if (choice == 'r'){

    mode1(choice, NULL, 0);
  }

  else if (choice =='f'){

    char file_name[10];
    int opt;

    printf("1: Lecture \n 2: Ecriture \n");
    scanf("%d", &opt);
    printf("\n");
    printf("Veuillez entrer le nom du fichier (Veuillez mettre l'extension .txt à la fin): \n");
    scanf("%s", file_name);
    printf("\n");
    FILE * file = NULL;
    
    if (opt == 1){

      printf("Mode lecture: \n");
      file = fopen(file_name, "r");
      mode1(choice, file, 1);
      fclose(file);
    }

    else if (opt == 2){
      printf("Mode Ecriture: \n");
      file = fopen(file_name, "a+");
      mode1(choice, file, 2);
      fclose(file);
    }

    else{
      printf("Aucun choix ne correspond, veuillez réessayer.\n");
    }
  }

  else{
    printf("Aurevoir!\n");
    return EXIT_SUCCESS;
  }

  return EXIT_SUCCESS;
}


void mode1(char choice, FILE* file, int optn){

  srand(time(NULL));

  int coordX = 3; // Coordonnée X de la case vide 
  int coordY = 3; // Coordonnée Y de la case vide

  bloc** taquin = NULL;
  //Creation des blocs
  if (choice == 'r' || (choice == 'f' && optn == 2)) {

      taquin = melange(4, &coordX, &coordY);
      if (choice == 'f' && optn == 2){
	        writeTaquin(taquin, file);
          printf("Copie dans effectue dans le fichier.\n");
      }
  }

  else{

    int line;
    printf("Quelle est l'occurence du tableau que vous choisissez de copier ?\n");
    scanf("%d", &line);
    printf("\n");
    taquin = readTaquin(line, file, &coordX, &coordY);
  }

  //////////////////////////////////////////////////////////////////////////////////
  
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface * window = NULL;
  SDL_Surface * fond = NULL;

  SDL_Rect pos_fond; // image de fond afin de decorer le taquin

  pos_fond.x = 0; 
  pos_fond.y = 0;
   
  fond = SDL_LoadBMP("fond.bmp");
  window =  SDL_SetVideoMode(900, 900, 32, SDL_HWSURFACE);

  
  SDL_BlitSurface(fond, NULL, window, &pos_fond);


  SDL_WM_SetCaption("Taquin", NULL);
   

  ///////////////////////////////////////////////////////////////////////////////

  SDL_Event event;
  int run = 1;
   
  //EVENEMENTS
  while(!resolu(taquin) && run){

      SDL_WaitEvent(&event);

      //Placement des blocs

      for (int i = 0; i < 4; i++){
          for (int j = 0; j < 4; j++){
	            printBloc(window, taquin[i][j]);
          }
      }

      switch(event.type){
       
          case SDL_QUIT:

              run = 0;
              break;

          case SDL_KEYDOWN:

              switch(event.key.keysym.sym){

                  case SDLK_UP: // Mouvement de la case vide vers le haut
            	     
                      deplacement(taquin, 0, &coordX, &coordY); 
                      break;

                  case SDLK_RIGHT: // Mouvement de la case vide vers la droite

                      deplacement(taquin, 1, &coordX, &coordY); 
                      break; 

                  case SDLK_DOWN: // Mouvement de la case vide vers le bas

                      deplacement(taquin, 2, &coordX, &coordY); 
                      break;

                  case SDLK_LEFT: // Mouvement de la case vide vers la gauche

                      deplacement(taquin, 3, &coordX, &coordY); 
                      break; 
	
                  }
        
          break; 
      }
     
      SDL_Flip(window); // Mise à jour de l'écran 
   }

  if (resolu(taquin)){
    printf("Bravo! Vous avez réussi!\n");
    SDL_Delay(2000);
  }
  else{
    printf("Une prochaine fois peut être...\n");
    SDL_Delay(2000);
  }

    SDL_FreeSurface(fond); //On libere l'image de fond de la memoire.
    freeTaquin(taquin); 
    SDL_Quit();
}

