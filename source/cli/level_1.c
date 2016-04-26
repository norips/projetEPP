#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "game.h"
#include "piece.h"

#define NB_PIECES 8
#define NB 6

void do_move(game tmpGame, int nombre, char* dep, int dist)
{
    for (int i = 0; dep[i] != '\0'; ++i) {
        dep[i] = toupper(dep[i]);
    }

    if (strcmp("UP", dep) == 0)
        play_move(tmpGame, nombre, UP, dist);
    else if (strcmp("LEFT", dep) == 0)
        play_move(tmpGame, nombre, LEFT, dist);
    else if (strcmp("DOWN", dep) == 0)
        play_move(tmpGame, nombre, DOWN, dist);
    else if (strcmp("RIGHT", dep) == 0)
        play_move(tmpGame, nombre, RIGHT, dist);

}

char ** creer_grille(void)
{
    char** grille;
    grille = malloc(NB * sizeof (char*));
    for (int i = 0; i < NB; ++i) {
        grille[i] = malloc(NB * sizeof (char));
    }
    return grille;
}

void delete_grille(char ** grille)
{
    for (int i = 0; i < NB; ++i)
        free(grille[i]);
    free(grille);
}

void initialiser_grille(char ** t)
{
    for (int i = 0; i < NB; i++) {
        for (int j = 0; j < NB; j++) {
            t[i][j] = ' ';
        }
    }
}

void completer_grille(char** t, game jeu)
{
    for(int i=0;i<game_width(jeu);i++)
        for(int j=0;j<game_height(jeu);j++){
            int piece = game_square_piece(jeu,i,j);
            if(piece == -1)
                t[j][i] = ' ';
            else
                t[j][i] = '0' + piece;
        }
}

void afficher_grille(char** t)
{
    for (int j = 0; j < NB; j++) {
        printf("+---------");
    }
    printf("\n");
    for (int i = 0; i < NB; i++) {
        for (int j = 0; j < NB; j++) {
            if (t[i][j] == ' ') {
                printf("|  %-6c ", t[i][j]); /*%6c pour les aligner à 5 chiffres.*/
            } else {
                printf("|    %-4c ", t[i][j]);
            }
            if (i == 2 && j == 5)
                printf("| -> SORTIE DE LA VOITURE [00]");
            else if (j == 5)
                printf("|");
        }
        putchar('\n');
        for (int j = 0; j < NB; j++) {
            printf("+---------");
        }
        putchar('\n');
    }
}

void afficher_jeu(char ** grille, game tmpGame)
{
    initialiser_grille(grille);
    completer_grille(grille, tmpGame);
    afficher_grille(grille);

    printf("\n");
}

int main()
{
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
    tmpGame = new_game_hr(NB_PIECES, tmp);

    char **grille = creer_grille();
    afficher_jeu(grille, tmpGame);

    int nombre = 0;
    char * dep = malloc(10 * sizeof (char));
    int dist = 0;

    while (get_x(game_piece(tmpGame, 0)) != 4) {
        printf("Entrez numero de la voiture: ");
        if (scanf("%d", &nombre) != 1) {
            return EXIT_FAILURE;
        }

        printf("Entrez la direction: (up / left / down / right) ");
        if (scanf("%s", dep) != 1) {
            return EXIT_FAILURE;
        }

        printf("Entrez la distance du mouvement: ");
        if (scanf("%d", &dist) != 1) {
            return EXIT_FAILURE;
        }


        do_move(tmpGame, nombre, dep, dist);
        printf("\n");
        afficher_jeu(grille, tmpGame);
    }

    printf("YOUPI !!!!!!!!!!\n");
    delete_game(tmpGame);
    delete_grille(grille);
}
