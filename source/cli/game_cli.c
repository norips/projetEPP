
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game.h"
#include "tool/gfx.h"
#include "tool/level_handler.h"
#include "tool/level_drawer.h"
#include "solv.h"
int MINH = 23;
int MINW = 79;

int MAXCOL = 6;
int MAXROW = 6;

typedef bool(*game_over_func)(cgame); /*Pointer to function for game over*/
game_over_func game_over;
bool gameOverRh;

static bool game_over_an(cgame newGame)
{
    return get_x(game_piece(newGame, 0)) == 1 && get_y(game_piece(newGame, 0)) == 0;
}

/*
 * @brief Show different game and wait for user to choose
 * @return game The new game created
 */
static game select_game()
{
    int row, col;
    int pos = 0;
    int ch = 0;
    game newGame = NULL;
    while (ch != '\n') {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2, col / 2 - 20, "Use arrow key to select your game");
        if (pos == 0) {
            mvprintw(row / 2 + 1, col / 2 - 20, "-> Rush Hour");
            mvprintw(row / 2 + 2, col / 2 - 20, "   Ane rouge");
        } else {
            mvprintw(row / 2 + 1, col / 2 - 20, "   Rush Hour");
            mvprintw(row / 2 + 2, col / 2 - 20, "-> Ane rouge");
        }
        ch = getch();
        if (ch == KEY_UP) {
            if (pos > 0)
                pos--;
        } else if (ch == KEY_DOWN) {
            if (pos < 1)
                pos++;
        }
        refresh();
    }
    if (pos == 0) {
        //Rush hour
        piece pieces[4];
        pieces[0] = new_piece_rh(3, 3, true, true);
        pieces[1] = new_piece_rh(3, 0, true, false);
        pieces[2] = new_piece_rh(4, 1, true, true);
        pieces[3] = new_piece_rh(5, 3, false, false);
        newGame = new_game(6, 6, 4, pieces);
        MAXCOL = game_width(newGame);
        MAXROW = game_height(newGame);
        MINH = MAXROW * SIZE + 2;
        MINW = MAXCOL * SIZE;
        game_over = game_over_hr;
        gameOverRh = true;
        for (int i = 0; i < game_nb_pieces(newGame); i++) {
            delete_piece(pieces[i]);
        }
        clear();
        refresh();
        return newGame;
    } else {
        //Ane rouge
        piece pieces[10];
        pieces[0] = new_piece(1, 3, 2, 2, true, true); //Rouge
        pieces[1] = new_piece(3, 3, 1, 2, true, true); // 2
        pieces[2] = new_piece(3, 1, 1, 2, true, true); // 3
        pieces[3] = new_piece(3, 0, 1, 1, true, true); // 4
        pieces[4] = new_piece(1, 2, 2, 1, true, true); // 5
        pieces[5] = new_piece(2, 1, 1, 1, true, true); // 6
        pieces[6] = new_piece(1, 1, 1, 1, true, true); // 7
        pieces[7] = new_piece(0, 0, 1, 1, true, true); // 8
        pieces[8] = new_piece(0, 1, 1, 2, true, true); // 9
        pieces[9] = new_piece(0, 3, 1, 2, true, true); // 10
        newGame = new_game(4, 5, 10, pieces);
        MAXCOL = game_width(newGame);
        MAXROW = game_height(newGame);
        MINH = MAXROW * SIZE + 2;
        MINW = MAXCOL * SIZE;
        game_over = game_over_an;
        gameOverRh = false;
        for (int i = 0; i < game_nb_pieces(newGame); i++) {
            delete_piece(pieces[i]);
        }
        clear();
        refresh();
        return newGame;
    }
}

game play_solution(game newGame, gameStruct *result, int n)
{
    char *pch, *sch;
    char *save_move = malloc(strlen(result->move) + 1);
    strcpy(save_move, result->move);
    pch = strtok(save_move, "\n");
    for (int i = 0; i < n && pch != NULL; i++) {
        pch = strtok(NULL, "\n");
    }
    sch = strtok(pch, " ");
    int piece = atoi(sch);
    sch = strtok(NULL, " ");
    if (strcmp(sch, "UP") == 0) {
        play_move(newGame, piece, UP, 1);
    } else if (strcmp(sch, "DOWN") == 0) {
        play_move(newGame, piece, DOWN, 1);
    } else if (strcmp(sch, "LEFT") == 0) {
        play_move(newGame, piece, LEFT, 1);
    } else if (strcmp(sch, "RIGHT") == 0) {
        play_move(newGame, piece, RIGHT, 1);
    }
    free(save_move);
    return newGame;
}

int main(int argc, char *argv[])
{
    WINDOW *my_win, *score;
    WINDOW **car;
    game newGame, tmpGame;
    MEVENT event;

    int ch = 0, choosenCar = -1, soluce_move = 0;
    char message[1024];
    bool quit = false;
    bool show_solution = false;
    gameStruct *resultSolv = NULL;
    //INIT
    setup();
    //END INIT
    //Wait for good size
    wait_for_size(MINH, MINW);
    //Instruction
    show_instruction(MINH, MINW);
    while (!quit) {
        quit = false;
        show_solution = false;
        soluce_move = 0;
        strcpy(message, "Playing");
        //Select game
        newGame = select_game();
        //Check for level file
        if (handle_level(&tmpGame)) {
            delete_game(newGame);
            newGame = tmpGame;
            MAXCOL = game_width(newGame);
            MAXROW = game_height(newGame);
            MINH = MAXROW * SIZE + 2;
            MINW = MAXCOL * SIZE;
        }
        car = malloc(sizeof (WINDOW*) * game_nb_pieces(newGame));
        //First draw
        draw_game(newGame, 0, 0, MAXROW, MAXCOL, &my_win, car, &score, choosenCar, message, gameOverRh);
        //Loop while the game is not finished
        while (!game_over(newGame)) {
            //Print on bottom of grid
            mvprintw(MAXROW * SIZE + 1, 0, "Please choose car :");
            ch = getch();
            if (ch == 's' && !show_solution) {
                show_solution = true;
                strcpy(message, "Solution");
                resultSolv = solv(newGame, gameOverRh,true);
                if(!resultSolv){
                    strcpy(message, "No solution");
                    show_solution=false;
                }
            }
            if (show_solution) {
                newGame = play_solution(newGame, resultSolv, soluce_move++);
            } else {
                if (KEY_MOUSE == ch) {
                    /* Mouse event. */
                    if (OK == getmouse(&event)) {
                        choosenCar = get_car_with_mouse(event.y, event.x, car, game_nb_pieces(newGame));
                    }
                } else {
                    if (ch == 'q') {
                        quit = true;
                        break;
                    }
                    play_input(newGame, ch, &choosenCar);
                }
            }
            wait_for_size(MINH, MINW);
            erase_game(newGame, my_win, car, score);
            draw_game(newGame, 0, 0, MAXROW, MAXCOL, &my_win, car, &score, choosenCar, message, gameOverRh);
        }
        if (!quit) {
            display_score(newGame);
        }
        for (int i = 0; i < game_nb_pieces(newGame); i++) {
            destroy_win(car[i]);
        }
        destroy_win(my_win);
        destroy_win(score);
        free(car);
        delete_game(newGame);
        if (resultSolv != NULL) {
            delete_game(resultSolv->current);
            free(resultSolv->move);
            free(resultSolv);
            resultSolv = NULL;
        }
    }
    endwin(); /* End curses mode		  */
    return 0;
}

