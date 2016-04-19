#ifndef LEVELDRAWERMLV_H
#define LEVELDRAWERMLV_H
#include <stdbool.h>
#include <game.h>
#ifdef __cplusplus
extern "C" {
#endif
    
/**
* @file level_drawerMLV.h
*
* @brief This file describes gfx part to handle level, using mlv.
**/
    
/**
 * @brief Handle level, draw level list and init the parameter with the selected level
 * 
 * @param[out] newLevel Game that will be initialized by the function
 * @return Nothing
 */
void handle_level(game *newLevel);

#ifdef __cplusplus
}
#endif

#endif /* LEVELHANDLER_H */

