#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include "game.h"

struct game_s{
    int nbMove;
    int nbPieces;
    piece *arrPieces;
};

game new_game_hr (int nb_pieces, piece *pieces){
    game newGame = malloc(sizeof(struct game_s));
    newGame->nbMove=0;
    newGame->nbPieces = nb_pieces;
    newGame->arrPieces = malloc(sizeof(piece) * nb_pieces);
    for(int i=0;i<nb_pieces;i++)
        newGame->arrPieces[i] = pieces[i];
    return newGame;
}

void delete_game (game g){
    for(int i=0;i<g->nbPieces;i++)
        delete_piece(g->arrPieces[i]);
    free(g->arrPieces);
    free(g);
}

void copy_game (cgame src, game dst){
    
}


int game_nb_pieces(cgame g){
    return 0;
}

cpiece game_piece(cgame g, int piece_num){
    return NULL;
}

bool game_over_hr(cgame g){
    return true;
}

bool play_move(game g, int piece_num, dir d, int distance){
    return true;
}

int game_nb_moves(cgame g){
    return 0;
}