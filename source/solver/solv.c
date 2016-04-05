/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: norips
 *
 * Created on 8 mars 2016, 11:33
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "solv.h"
#include "uthash.h"
#include "utlist.h"


hashTableChrInt *s, *tempH, *hTable = NULL;
gameStruct *hList = NULL;
typedef bool(*game_over_func)(cgame); /*Pointer to function for game over*/
game_over_func game_over;
bool showPath = false;

bool game_over_an(cgame newGame)
{
    return get_x(game_piece(newGame, 0)) == 1 && get_y(game_piece(newGame, 0)) == 0;
}

char* serialize(cpiece *arrPieces, int n)
{
    char *buf = malloc(n * 6 + 1);
    buf[0] = 0;
    for (int i = 0, j = 0; i < n; i++, j += 6) {
        buf[j] = get_x(arrPieces[i]) + '0';
        buf[j + 1] = get_y(arrPieces[i]) + '0';
        buf[j + 2] = get_width(arrPieces[i]) + '0';
        buf[j + 3] = get_height(arrPieces[i]) + '0';
        buf[j + 4] = can_move_x(arrPieces[i]) + '0';
        buf[j + 5] = can_move_y(arrPieces[i]) + '0';
        buf[j + 6] = '\0';
    }
    return buf;
}

bool sup(cpiece p1, cpiece p2)
{
    if (get_x(p1) > get_x(p2)) {
        return true;
    }
    if (get_x(p1) == get_x(p2))
        if (get_y(p1) > get_y(p2))
            return true;
    return false;

}

char* convertGame(game newGame)
{
    int n = game_nb_pieces(newGame);
    cpiece *arrPieces;
    arrPieces = malloc(sizeof (piece) * n);
    for (int i = 0; i < n; i++) {
        arrPieces[i] = game_piece(newGame, i);
    }
    //Sort
    for (int i = 0; i < (n - 1); i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sup(arrPieces[j], arrPieces[j + 1])) {
                cpiece swap = arrPieces[j];
                arrPieces[j] = arrPieces[j + 1];
                arrPieces[j + 1] = swap;
            }
        }
    }
    char *parsed = serialize(arrPieces, n);
    free(arrPieces);
    return parsed;
}

bool check_found_else_create(game newGame)
{
    s = NULL;
    char *parsedGame = convertGame(newGame);
    HASH_FIND_STR(hTable, parsedGame, s);
    if (s == NULL) {
        tempH = malloc(sizeof (hashTableChrInt));
        tempH->key = parsedGame;
        HASH_ADD_STR(hTable, key, tempH);
        return false;
    }
    free(parsedGame);
    return true;
}

void save_or_continue(game tmpGame, gameStruct *new, int nbPiece, int nb_move, char* direction)
{
    char *tmp;
    if (check_found_else_create(tmpGame)) {
        return;
    }
    gameStruct *newPath = malloc(sizeof (gameStruct));
    if (showPath) {
        tmp = malloc(strlen(new->move) * sizeof (char) + (strlen(" 00 \n") + strlen(direction)) * sizeof (char) * nb_move + 1 * sizeof (char));
        if (!tmp || !newPath) {
            printf("Memory allocation error");
        }
        sprintf(tmp, "%s", new->move);
        for (int j = 0; j < nb_move; j++) {
            sprintf(tmp, "%s %d %s\n", tmp, nbPiece, direction);
        }
        newPath->move = tmp;
    }
    game tmpGame2 = new_game_hr(0, NULL);
    copy_game(tmpGame, tmpGame2);
    newPath->current = tmpGame2;
    DL_APPEND(hList, newPath);
}

gameStruct *tear_down(gameStruct *new)
{
    hashTableChrInt *current, *tmp;

    HASH_ITER(hh, hTable, current, tmp)
    {
        HASH_DEL(hTable, current); /* delete; users advances to next */
        free(current->key); /* optional- if you want to free  */
        free(current);
    }
    gameStruct *result;
    result = malloc(sizeof (gameStruct));
    game tempGame = new_game_hr(0, NULL);
    copy_game(new->current, tempGame);
    result->current = tempGame;
    if (showPath) {
        int len = strlen(new->move);
        result->move = malloc(sizeof (char)*(len + 1));
        strcpy(result->move, new->move);
    }
    gameStruct *elt, *temp;

    /* now delete each element, use the safe iterator */
    DL_FOREACH_SAFE(hList, elt, temp)
    {
        DL_DELETE(hList, elt);
        if (showPath) {
            free(elt->move);
        }
        delete_game(elt->current);
        free(elt);
    }
    free(hTable);
    free(hList);
    return result;
}

void explore(gameStruct *new)
{
    for (int i = 0; i < game_nb_pieces(new->current); i++) {
        game tmpGame = new_game_hr(0, NULL);
        copy_game(new->current, tmpGame);
        int nb_move = 1;
        while (play_move(tmpGame, i, LEFT, 1)) {
            save_or_continue(tmpGame, new, i, nb_move, "LEFT");
            nb_move++;
        }
        //RESET
        delete_game(tmpGame);
        tmpGame = new_game_hr(0, NULL);
        copy_game(new->current, tmpGame);
        nb_move = 1;
        while (play_move(tmpGame, i, RIGHT, 1)) {
            save_or_continue(tmpGame, new, i, nb_move, "RIGHT");
            nb_move++;
        }
        //RESET
        delete_game(tmpGame);
        tmpGame = new_game_hr(0, NULL);
        copy_game(new->current, tmpGame);
        nb_move = 1;
        while (play_move(tmpGame, i, UP, 1)) {
            save_or_continue(tmpGame, new, i, nb_move, "UP");
            nb_move++;
        }
        //RESET
        delete_game(tmpGame);
        tmpGame = new_game_hr(0, NULL);
        copy_game(new->current, tmpGame);
        nb_move = 1;
        while (play_move(tmpGame, i, DOWN, 1)) {
            save_or_continue(tmpGame, new, i, nb_move, "DOWN");
            nb_move++;
        }
        delete_game(tmpGame);
    }
}

gameStruct *solv(cgame newGame, int gameType, bool _showPath)
{
    showPath = _showPath;
    gameStruct *new = malloc(sizeof (gameStruct));
    game tmp = new_game_hr(0, NULL);
    copy_game(newGame, tmp);
    new->current = tmp;
    if (showPath) {
        new->move = malloc(sizeof (char)*10);
        new->move[0] = 0;
    }
    DL_APPEND(hList, new);
    switch (gameType) {
    case ANE:
        game_over = game_over_an;
        break;
    case RUSH:
        game_over = game_over_hr;
        break;
    default:
        game_over = game_over_hr;
        break;
    }
    do {
        new = hList;
        if (game_over(new->current)) {
            //printf("Found !\n");
            return tear_down(new);
        }
        explore(new);
        DL_DELETE(hList, new);
        if (showPath) {
            free(new->move);
        }
        delete_game(new->current);
        free(new);
    } while (hList);
    gameStruct *s;
    s = tear_down(new);
    free(s->move);
    delete_game(s->current);
    free(s);
    return NULL;
}