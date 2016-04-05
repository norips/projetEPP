#ifndef GAME_CLI_H
#define GAME_CLI_H
#include <ncurses.h>
#include "game.h"
#define SIZE 4

WINDOW *create_newgrid(int starty, int startx, int nbRow, int nbCol, int spaceBetween, bool gameOverRh);
WINDOW *create_newcar(cpiece newPiece, int number, bool selected, int spaceBetween, int MAXROW);
WINDOW *create_newscore(int height, int width, int starty, int startx, int score, char *message);
void destroy_win(WINDOW *local_win);
void draw_game(game newGame, int starty, int startx, int MAXROW, int MAXCOL, WINDOW **grid, WINDOW **car, WINDOW **score, int choosenCar, char *message, bool gameOverRh);
void erase_game(game newGame, WINDOW *my_win, WINDOW **car, WINDOW *score);
void play_input(game newGame, int ch, int *choosenCar);
int get_car_with_mouse(int y, int x, WINDOW** winCar, int nbPieces);
void display_score(cgame newGame);
void setup();
void wait_for_size(int MINH, int MINW);
void show_instruction(int MINH, int MINW);
#endif 

