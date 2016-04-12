/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   levelHandler.h
 * Author: silvermoon
 *
 * Created on 3 mars 2016, 11:38
 */


#ifndef LEVELDRAWERMLV_H
#define LEVELDRAWERMLV_H
#include <stdbool.h>
#include <game.h>
#ifdef __cplusplus
extern "C" {
#endif
    void draw_level(int startx, int starty, int height, int width, int choosen, char **name, int n, int *);
    void get_input(int *choosen, int nblevel, bool *enter, int mini);
    void handle_level(game *newLevel);

#ifdef __cplusplus
}
#endif

#endif /* LEVELHANDLER_H */

