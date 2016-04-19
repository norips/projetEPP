#ifndef SOLV_H
#define SOLV_H
#include "uthash.h"
#include "utlist.h"
#include "game.h"
#ifdef __cplusplus
extern "C" {
#endif
    
/**
* @file solv.h
*
* @brief This file describes the solver.
**/
    
#define RUSH 1
#define ANE 0
//Hash table

typedef struct hashTableChrInt {
    char* key; /* key (string is WITHIN the structure) */
    UT_hash_handle hh; /* makes this structure hashable */
} hashTableChrInt;

//List

typedef struct gameStruct {
    game current;
    char *move;
    struct gameStruct *prev; /* needed for a doubly-linked list only */
    struct gameStruct *next; /* needed for singly- or doubly-linked lists */
} gameStruct;

/**
 * @brief Main function, solv the puzzle
 * 
 * @param[in] newGame game to solv
 * @param[in] gameType gameType
 * @param[in] _showPath Whether show path to win or not
 * @return Winning game
 * @retval NULL no solution was found
 */
gameStruct *solv(cgame newGame, int game,bool showPath);

#ifdef __cplusplus
}
#endif

#endif /* SOLV_H */

