#ifndef LEVELHANDLER_H
#define LEVELHANDLER_H
#include <stdbool.h>
#include <game.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
* @file level_handler.h
*
* @brief This file describes level handler.
**/
    
/**
 * @brief Parse level and load it
 * 
 * @param File path of the level
 * @return Loaded game
 */
game parse_level(char *levelFound);

/**
 * @brief Parse level and load it
 * 
 * @param path Dir path
 * @param nblevel Number of level found
 * @return Array of file path of levels
 */
char** find_levels(char *path, int *nblevel);
#ifdef __cplusplus
}
#endif

#endif /* LEVELHANDLER_H */

