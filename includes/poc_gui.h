#ifndef POC_GUI_H
#define POC_GUI_H

#define SIZE 4
#define MINH 27
#define MINW 73
#define MAXCOL 6
#define MAXROW 6

WINDOW *create_newgrid(int starty, int startx,int nbRow,int nbCol,int spaceBetween);
WINDOW *create_newcar(cpiece newPiece,int number,bool selected,int spaceBetween);
WINDOW *create_newscore(int height,int width,int starty, int startx,int score,bool playing);
void destroy_win(WINDOW *local_win);
void draw_game(game newGame,int starty,int startx,WINDOW **my_win,WINDOW **car,WINDOW **score,int choosenCar);
void erase_game(game newGame,WINDOW *my_win,WINDOW **car,WINDOW *score);
void play_input(game newGame,int ch,int *choosenCar);

#endif 

