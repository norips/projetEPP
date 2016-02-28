#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "piece.h"

#define NB_PIECES 8
#define NB 6


void initialiser_grille (char t[NB][NB]){
  for (int i=0; i<NB; i++){
    for (int j=0; j<NB; j++){
      t[i][j]='\0';
    }
  }
}

void completer_grille (char t[NB][NB], game jeu){
  for (int i=0; i<game_nb_pieces(jeu); ++i){
    int abs = get_x(game_piece(jeu,i));
    int ord = get_y(game_piece(jeu,i));
    t[ord][abs]= '0'+i;

    if (is_horizontal(game_piece(jeu,i))){
      int taille=get_width(game_piece(jeu,i));
      for (int z=abs; z < abs+taille; ++z)
	t[ord][z]= '0'+i;
      
    }else{
      int taille=get_height(game_piece(jeu,i));
      for (int j=ord; j < ord+taille; ++j)
	t[j][abs]= '0'+i;
    }
  }
}

void afficher_grille(char t[NB][NB]){
  for(int i=0;i<NB;i++){
    for(int j=0;j<NB;j++){
      if (t[i][j]=='\0'){
	printf("|  %-6c ",t[i][j]); /*%6c pour les aligner à 5 chiffres.*/
      }else{
	printf("|   %-4c ",t[i][j]);
      }
    }
    putchar('\n');
    for(int j=0;j<NB;j++){
      printf("+--------");
    }
    putchar('\n');
  }
}



void afficher_jeu(char grille [NB][NB], game tmpGame){
  initialiser_grille(grille);
  completer_grille(grille, tmpGame);
  afficher_grille(grille);
  printf("\n");
}


int main (){
    piece tmp[8];
    tmp[0] = new_piece_rh(0, 2, true, true);
    tmp[1] = new_piece_rh(0, 0, false, true);
    tmp[2] = new_piece_rh(0, 3, true, false);
    tmp[3] = new_piece_rh(0, 5, false, true);
    tmp[4] = new_piece_rh(2, 1, false, false);
    tmp[5] = new_piece_rh(4, 4, true, false);
    tmp[6] = new_piece_rh(4, 3, true, true);
    tmp[7] = new_piece_rh(5, 0, false, false);
    game tmpGame;
    tmpGame = new_game_hr(NB_PIECES,tmp);

    char grille[NB][NB]={};

    afficher_jeu(grille, tmpGame);
    
    int nombre = 0;
    int dep = 0;
    int dist = 0;

    while (get_x(game_piece(tmpGame,0)) !=4){
      printf("Entrez numero de la voiture: ");
      scanf("%d", &nombre);
      
      printf("Entrez la direction: (up = 0 / left = 1 / down = 2 / right = 3): ");
      scanf("%d",&dep);
      
      printf("Entrez la distance du mouvement: ");
      scanf("%d", &dist);
      
      
      printf("\n");
      play_move(tmpGame,nombre,dep,dist);
      afficher_jeu(grille, tmpGame);
    }

    printf("GAGNE !!!!!!!!!!\n");
}
