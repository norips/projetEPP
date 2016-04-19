#ifndef GAME_CLI_H
#define GAME_CLI_H
#include <ncurses.h>
#include "game.h"
#define SIZE 4

/**
 * @file gfx.h
 *
 * @brief This file describes gfx part, using ncurses.
 **/

/**
 * @brief Create a new grid
 * @param[in] height the height of the grid
 * @param[in] width the width of the grid
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @return Created window
 */
WINDOW *create_newgrid(int starty, int startx, int nbRow, int nbCol, int spaceBetween, bool gameOverRh);

/**
 * @brief Create a new car
 * 
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @param small indicates if the vehicle is of size 2 (small=true) or 3 (small=false)
 * @param horizontal indicates whether the vehicle can move horizontally or vertically
 * @param number indicates the number of the vehicule
 * @return Created window
 */
WINDOW *create_newcar(cpiece newPiece, int number, bool selected, int spaceBetween, int MAXROW);

/**
 * @brief Create a new score
 * @param[in] height the height of the grid
 * @param[in] width the width of the grid
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @param[in] score the current score to be drawing on screen
 * @param[in] over wether the game is over or not
 * @return Created window
 */
WINDOW *create_newscore(int height, int width, int starty, int startx, int score, char *message);

/**
 * @brief Destroy a window created by ncurses
 * @param[in] Windows to destroy
 * @return Nothing
 */
void destroy_win(WINDOW *local_win);

/**
 * @brief Draw a new game
 * 
 * @param[in] The game to draw
 * @param startx the x pos to start drawing the grid
 * @param starty the y pos to start drawing the grid
 * @param[out] grid Address of pointer to your grid's window
 * @param[out] car Your array of car
 * @param[out] score Address of pointer to your score's window
 * @param choosenCar indice of the choosen car
 * @return Nothing
 */
void draw_game(game newGame, int starty, int startx, int MAXROW, int MAXCOL, WINDOW **grid, WINDOW **car, WINDOW **score, int choosenCar, char *message, bool gameOverRh);

/**
 * @brief Erase a game
 * 
 * @param[in] The game to erase
 * @param[in] grid Address of pointer to your grid's window
 * @param[in] car Your array of car
 * @param[in] score Address of pointer to your score's window
 * @return Nothing
 */
void erase_game(game newGame, WINDOW *my_win, WINDOW **car, WINDOW *score);

/**
 * @brief Play move using input
 * 
 * @param[in] The game to play on
 * @param ch Input
 * @param[out] car The choosen car
 * @return Nothing
 */
void play_input(game newGame, int ch, int *choosenCar);

/**
 * @brief Get number of the click selected by mouse
 * 
 * @param y Y pos of the mouse
 * @param x X pos of the mouse
 * @param[in] winCar Address of pointer to your cars' window
 * @param nbPieces Number of cars
 * @return Number of the selected car otherwise -1
 */
int get_car_with_mouse(int y, int x, WINDOW** winCar, int nbPieces);

/**
 * @brief Display score on finish
 * @param newGame Finished game
 * @return Nothing
 */
void display_score(cgame newGame);

/**
 * @brief Init ncurses
 * @return Nothing
 */
void setup();

/**
 * @brief Wait for good terminal size
 * @return Nothing
 */
void wait_for_size(int MINH, int MINW);

/**
 * @brief Show instruction and wait for key to continue
 * @return Nothing
 */
void show_instruction(int MINH, int MINW);
#endif 

