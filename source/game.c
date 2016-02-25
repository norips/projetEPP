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
    for(int i=0;i<nb_pieces;i++){
        newGame->arrPieces[i] = new_piece_rh(0, 0,false,false); //Create new piece
        copy_piece(pieces[i],newGame->arrPieces[i]); //Overwrite new piece with the copy
    }
    return newGame;
}

void delete_game (game g){
    for(int i=0;i<g->nbPieces;i++)
        delete_piece(g->arrPieces[i]);
    free(g->arrPieces);
    free(g);
}

void copy_game (cgame src, game dst){
    dst->arrPieces = malloc(sizeof(piece) * src->nbPieces);
    for(int i=0;i<src->nbPieces;i++){
        dst->arrPieces[i] = new_piece_rh(0, 0,false,false); //Create new piece
        copy_piece(src->arrPieces[i],dst->arrPieces[i]); //Overwrite new piece with the copy
    }
    dst->nbPieces = src->nbPieces;
    dst->nbMove = src->nbMove;
}


int game_nb_pieces(cgame g){
    return g->nbPieces;
}

cpiece game_piece(cgame g, int piece_num){
    return g->arrPieces[piece_num];
}

bool game_over_hr(cgame g){
    return get_x(g->arrPieces[0]) == 4 && get_y(g->arrPieces[0]);
}

bool play_move(game g, int piece_num, dir d, int distance){
    piece tmp = new_piece_rh(0, 0,false,false);
    copy_piece(game_piece(g,piece_num),tmp);
    move_piece(tmp,d,distance);
    if(get_x(tmp) == get_x(game_piece(g,piece_num)) && get_y(tmp) == get_y(game_piece(g,piece_num)))
        return false;
    if(d == LEFT || d == RIGHT){
        if((get_x(tmp) + get_width(tmp)) > 6 || get_x(tmp) < 0)
            return false;
    } else {
        if((get_y(tmp) + get_height(tmp)) > 6 || get_y(tmp) < 0)
            return false;
    }
    for(int i=0;i<game_nb_pieces(g);i++){
        if(i==piece_num)
            continue;
        if(intersect(tmp,game_piece(g,i)))
            return false;
    }
    g->arrPieces[piece_num] = tmp;  
    g->nbMove+=distance;
    return true;
}

int game_nb_moves(cgame g){
    return g->nbMove;
}
