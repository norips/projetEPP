#ifndef LEVEL_DRAWER_H
#define LEVEL_DRAWER_H
#include "game.h"
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @file level_drawer.h
 *
 * @brief This file describes gfx part to handle level, using ncurses.
 **/
    
/**
 * @brief Handle level, draw level list and init the parameter with the selected level
 * 
 * @param[out] newLevel Game that will be initialized by the function
 * @return Wether level was loaded or not
 * @retval true level has been loaded
 * @retval false level hasn't been loaded
 */
bool handle_level(game *newLevel);

#ifdef __cplusplus
}
#endif

#endif /* LEVEL_DRAWER_H */

