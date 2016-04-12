/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: silvermoon
 *
 * Created on 15 mars 2016, 18:10
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "solv.h"
#include "tool/level_handler.h"

/*
 * 
 */
int main(int argc, char** argv)
{
    int gameType = 0;
    //Ane rouge
    if (argc < 3) {
        fprintf(stderr, "%s : a|r Path_to_lvl ShowPath\n Game : \tr Rush Hour\n\t\ta Ane Rouge\n", argv[0]);
        return EXIT_FAILURE;
    }
    int showPath = 0;
    if (argc == 4) {
        showPath = atoi(argv[3]);
    }
    switch (argv[1][0]) {
    case 'a':
        gameType = 0;
        break;
    case 'r':
        gameType = 1;
        break;
    }
    game newGame = NULL;
    newGame = parse_level(argv[2]);
    if (!newGame) {
        fprintf(stderr, "Unable to parse game \n");
        return EXIT_FAILURE;
    }
    gameStruct *result = NULL;
    result = solv(newGame, gameType, showPath);
    if (!result) {
        printf("-1\n");
        delete_game(newGame);
        return EXIT_FAILURE;
    }
    printf("%d\n", game_nb_moves(result->current));
    if (showPath) {
        printf("Move : %s", result->move);
        free(result->move);
    }
    delete_game(result->current);
    free(result);
    return (EXIT_SUCCESS);
}

