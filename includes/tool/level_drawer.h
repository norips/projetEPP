/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   level_drawer.h
 * Author: norips
 *
 * Created on 3 avril 2016, 17:43
 */

#ifndef LEVEL_DRAWER_H
#define LEVEL_DRAWER_H
#include "game.h"
#ifdef __cplusplus
extern "C" {
#endif

    bool draw_level(int choosen, char **name, int n, int* mini);
    bool handle_level(game *newLevel);



#ifdef __cplusplus
}
#endif

#endif /* LEVEL_DRAWER_H */

