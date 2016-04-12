/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gfxMLV.h
 * Author: norips
 *
 * Created on 11 avril 2016, 16:29
 */

#ifndef GFXMLV_H
#define GFXMLV_H

#ifdef __cplusplus
extern "C" {
#endif

#define RUSH 1
#define ANE 0
typedef bool(*game_over_func)(cgame); /*Pointer to function for game over*/
game_over_func game_over;
#define RATIO 70
//
// Attention ! 
// Pour pouvoir compiler ce programme sous windows et sous macintosh,
// il faut, pour la déclaration du main, respecter strictement la syntaxe
// suivante :
//
void play_events(game newGame, int *click);
void draw_score(int startx, int starty, int height, int width, int spaceBetweenRow, int score);
int choose_game_type(int startx, int starty);
void draw_finish_line(int x1, int y1, int x2, int y2, int spaceBetweenRow, MLV_Color color);
void draw_grid(int startx, int starty, int nbCol, int nbRow, int spaceBetweenRow, int type);
MLV_Image* init_car(game newGame, int indice_piece, int ratio);
void draw_car(MLV_Image *input, int col, int row, int spaceBetweenRow);
int getCarWithMouse(int x, int y, game tmpGame);

#ifdef __cplusplus
}
#endif

#endif /* GFXMLV_H */

