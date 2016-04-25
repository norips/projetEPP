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
/**
 * @file gfxMLV.h
 *
 * @brief This file describes gfx part, using MLV.
 **/

/**
 * @brief Play events
 * 
 * @param[in] newGame The game to play on
 * @param[out] click Selected car
 * @return Nothing
 */
void play_events(game newGame, int *click, bool *gameOver);

/**
 * @brief Draw score
 * 
 * @param[in] startx Top left corner x
 * @param[in] starty Top left corner y
 * @param[in] height Height of the score
 * @param[in] width Width of the score
 * @param[in] spaceBetweenRow Nb of pixel between row
 * @param[in] score Score to draw
 * @return Nothing
 */
void draw_score(int startx, int starty, int height, int width, int spaceBetweenRow, int score);

/**
 * @brief Show game type
 * 
 * @param[in] startx Top left corner x
 * @param[in] starty Top left corner y
 * @return Selected game type
 * @retval 1 Rush hour game type
 * @retval 0 Ane rouge game type
 */
int choose_game_type(int startx, int starty);

/**
 * @brief Draw finish line
 * 
 * @param[in] x1 Top left corner x
 * @param[in] y1 Top left corner y
 * @param[in] x2 Bottom right corner x
 * @param[in] y2 Bottom right corner y
 * @param[in] spaceBetweenRow Nb of pixel between row
 * @param[in] color Color of the finish line
 * @return Nothing
 */
void draw_finish_line(int x1, int y1, int x2, int y2, int spaceBetweenRow, MLV_Color color);

/**
 * @brief Draw grid
 * 
 * @param[in] startx Top left corner x
 * @param[in] starty Top left corner y
 * @param[in] nbCol Nb of column
 * @param[in] nbRow Nb of row
 * @param[in] spaceBetweenRow Nb of pixel between row
 * @param[in] type Game type
 * @return Nothing
 */
void draw_grid(int startx, int starty, int nbCol, int nbRow, int spaceBetweenRow, int type);

/**
 * @brief Load car with corresponding asset
 * 
 * @param[in] newGame Game
 * @param[in] indice_piece Nb of the piece you want to get
 * @return MLV_Image of the car
 */
MLV_Image* init_car(game newGame, int indice_piece);

/**
 * @brief Draw car from init_car
 * 
 * @param[in] input Car from init_car
 * @param[in] col x pos of the car
 * @param[in] row y pos of the car
 * @param[in] spaceBetweenRow Nb of pixel between row
 * @return Nothing
 */
void draw_car(MLV_Image *input, int col, int row, int spaceBetweenRow);

/**
 * @brief Get car's number from mouse
 * 
 * @param[in] x x pos of the mouse
 * @param[in] y y pos of the mouse
 * @param[in] Game to get piece on
 * @return Number of the selected piece otherwise -1
 */
int get_car_with_mouse(int x, int y, game tmpGame);

#ifdef __cplusplus
}
#endif

#endif /* GFXMLV_H */

