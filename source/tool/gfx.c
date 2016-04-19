/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tool/gfx.h"

WINDOW *create_newgrid(int starty, int startx, int nbRow, int nbCol, int spaceBetween, bool gameOverRh)
{
    WINDOW *local_win;

    local_win = newwin(nbRow * spaceBetween + 1, nbCol * (spaceBetween * 2) + 1, starty, startx);
    box(local_win, 0, 0); /* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
    for (int i = 1; i < spaceBetween * nbRow; i++) {
        wmove(local_win, spaceBetween*i, 1);
        whline(local_win, 0, (nbRow * spaceBetween)*2 - 2);
    }
    for (int i = 1; i < spaceBetween * nbCol; i++) {
        wmove(local_win, 1, (spaceBetween * i)*2);
        wvline(local_win, 0, nbRow * spaceBetween - 1);
    }
    wmove(local_win, 0, 0);
    if (gameOverRh) {
        wmove(local_win, (spaceBetween)*2, (spaceBetween * 2) * nbCol);
        wattron(local_win, COLOR_PAIR(1)); //COLOR init with init_pair
        wattron(local_win, A_BOLD); //Bold char
        wvline(local_win, '#', spaceBetween + 1);
        wattroff(local_win, A_BOLD);
        wattroff(local_win, COLOR_PAIR(1));
    } else {
        wmove(local_win, spaceBetween * nbRow, (spaceBetween * 2));
        wattron(local_win, COLOR_PAIR(1)); //COLOR init with init_pair
        wattron(local_win, A_BOLD); //Bold char
        whline(local_win, '#', (spaceBetween * 2)*2 + 1);
        wattroff(local_win, A_BOLD);
        wattroff(local_win, COLOR_PAIR(1));
    }

    wrefresh(local_win); /* Show that box 		*/

    return local_win;
}

WINDOW *create_newcar(cpiece newPiece, int number, bool selected, int spaceBetween, int MAXROW)
{
    WINDOW *local_win;
    int height = 0;
    int width = 0;
    int starty = get_y(newPiece);
    int startx = get_x(newPiece);
    //Mirror y axis
    starty = MAXROW - starty - get_height(newPiece);
    bool horizontal = is_horizontal(newPiece);
    //Setup size
    if (horizontal) {
        height = spaceBetween * get_height(newPiece);
        width = spaceBetween * get_width(newPiece)*2;
    } else if (!horizontal) {
        height = spaceBetween * get_height(newPiece);
        width = spaceBetween * get_width(newPiece)*2 + 1;
    }
    //End setup
    //New car with border    
    local_win = newwin(height - 1, width - 2, starty * spaceBetween + 1, startx * spaceBetween * 2 + 1);
    if (number == 0) {
        wattron(local_win, COLOR_PAIR(1)); //COLOR init with init_pair
        wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
        wattroff(local_win, COLOR_PAIR(1));
    } else {
        wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+'); /* 0, 0 gives default characters 
                                                                    * for the vertical and horizontal
                                             * lines			*/
    }
    //Print number of the vehicle with color 
    if (selected) {
        wattron(local_win, COLOR_PAIR(2)); //COLOR init with init_pair
        wattron(local_win, A_BOLD); //Bold char
        mvwprintw(local_win, (height - 1) / 2, (width - 2) / 2, "%d", number);
        wattroff(local_win, A_BOLD);
        wattroff(local_win, COLOR_PAIR(2));
    } else {
        wattron(local_win, COLOR_PAIR(1)); //COLOR init with init_pair
        wattron(local_win, A_BOLD); //Bold char
        mvwprintw(local_win, (height - 1) / 2, (width - 2) / 2, "%d", number);
        wattroff(local_win, A_BOLD);
        wattroff(local_win, COLOR_PAIR(1));
    }
    wmove(local_win, 0, 0); //Set cur pos to top left car
    wrefresh(local_win); /* Show that box 		*/
    return local_win;
}
WINDOW *create_newscore(int height, int width, int starty, int startx, int score, char *message)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+'); /* 0, 0 gives default characters 
                                                                    * for the vertical and horizontal
                                             * lines			*/
    char msg[] = "Status : ";
    char msg2[] = "Score : ";
    wmove(local_win, 0, 0); //Set cur pos to top left car
    mvwprintw(local_win, 0, 0, "%s%s", msg, message);
    mvwprintw(local_win, height / 2, (width - (strlen(msg2) + 1)) / 2, "%s%d", msg2, score);
    wrefresh(local_win); /* Show that box 		*/

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    /* box(local_win, ' ', ' '); : This won't produce the desired
     * result of erasing the window. It will leave it's four corners 
     * and so an ugly remnant of window. 
     */
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    /* The parameters taken are 
     * 1. win: the window on which to operate
     * 2. ls: character to be used for the left side of the window 
     * 3. rs: character to be used for the right side of the window 
     * 4. ts: character to be used for the top side of the window 
     * 5. bs: character to be used for the bottom side of the window 
     * 6. tl: character to be used for the top left corner of the window 
     * 7. tr: character to be used for the top right corner of the window 
     * 8. bl: character to be used for the bottom left corner of the window 
     * 9. br: character to be used for the bottom right corner of the window
     */
    wrefresh(local_win);
    delwin(local_win);
}

int get_car_with_mouse(int y, int x, WINDOW** winCar, int nbPieces)
{
    for (int i = 0; i < nbPieces; i++) {
        int miny, minx, maxx, maxy;
        getbegyx(winCar[i], miny, minx); //Don't need to pass address because it's a macro
        getmaxyx(winCar[i], maxy, maxx); //Don't need to pass address because it's a macro
        if (x >= minx && x <= minx + maxx &&
                y >= miny && y <= miny + maxy) {
            return i;
        }
    }
    return -1;
}

void display_score(cgame newGame)
{
    clear();
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2, col / 2 - 20, "You win with : %d moves !", game_nb_moves(newGame));
    mvprintw(row / 2 + 1, col / 2 - 20, "Press any key to continue");
    refresh();
    getch();
}

void setup()
{
    //INIT
    initscr(); /* Start curses mode 		*/
    cbreak(); /* Line buffering disabled, Pass on everything to me 		*/
    noecho();
    start_color(); /* Init color */
    use_default_colors(); /* Use default color, prevent screen to use default color aka black bg */
    init_pair(1, COLOR_RED, -1); /* Init color pair with red foreground and no color background aka transparent */
    init_pair(2, COLOR_GREEN, -1); /* Init color pair with red foreground and no color background aka transparent */
    keypad(stdscr, TRUE); /* I need that for mouse events 	*/
    mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */
}

void wait_for_size(int MINH, int MINW)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    while ((row < MINH) || (col < MINW)) {
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2, (col - 37) / 2, "Please resize to %dx%d actual = %dx%d", MINW, MINH, col, row);
        refresh();
        //Slow down cpu
        napms(100);
        clear();
    }
}

void show_instruction(int MINH, int MINW)
{
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 1, col / 2 - 20, "Please use your mouse to select car or use it number");
    mvprintw(row / 2, col / 2 - 20, "Use arrow key to move car, selected car is in green");
    mvprintw(row / 2 + 1, col / 2 - 20, "Press 'q' to exit");
    mvprintw(row / 2 + 2, col / 2 - 20, "Press 's' to solve current game");
    mvprintw(row / 2 + 3, col / 2 - 20, "Press any key to continue");
    //Show
    refresh();
    //Wait to enter
    while (getch() == KEY_RESIZE) {
        wait_for_size(MINH, MINW);
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 1, col / 2 - 20, "Please use your mouse to select car or use it number");
        mvprintw(row / 2, col / 2 - 20, "Use arrow key to move car, selected car is in green");
        mvprintw(row / 2 + 1, col / 2 - 20, "Press 'q' to exit");
        mvprintw(row / 2 + 2, col / 2 - 20, "Press 's' to solve current game");
        mvprintw(row / 2 + 3, col / 2 - 20, "Press any key to continue");
        //Show
        refresh();

    }
    mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */
    clear();
    refresh();
}

void draw_game(game newGame, int starty, int startx, int MAXROW, int MAXCOL, WINDOW **grid, WINDOW **car, WINDOW **score, int choosenCar, char *message, bool gameOverRh)
{
    *grid = create_newgrid(starty, startx, MAXROW, MAXCOL, SIZE, gameOverRh); // Create new grid
    wmove(*grid, 0, 0);
    //Create each car and highlight the selected one
    for (int i = 0; i < game_nb_pieces(newGame); i++) {
        if (choosenCar == i) {
            car[i] = create_newcar(game_piece(newGame, i), i, true, SIZE, MAXROW); // Create new car and highlight it
            continue;
        }
        car[i] = create_newcar(game_piece(newGame, i), i, false, SIZE, MAXROW); // Create new car
    }
    wmove(*grid, 0, 0);
    //Create score on right of the grid
    *score = create_newscore(SIZE * MAXROW + 1, SIZE*MAXCOL, starty, startx + SIZE * MAXCOL * 2 + 1, game_nb_moves(newGame), message);
    wmove(*grid, 0, 0);
}

void erase_game(game newGame, WINDOW *my_win, WINDOW **car, WINDOW *score)
{
    destroy_win(my_win); /*Destroy window    */
    for (int i = 0; i < game_nb_pieces(newGame); i++) /*to create new one */
        destroy_win(car[i]); /*                  */
    destroy_win(score); /*                  */
}

void play_input(game newGame, int ch, int *choosenCar)
{
    if (ch >= '0' && ch <= '9') {
        *choosenCar = ch - '0';
    }
    if (*choosenCar != -1 && *choosenCar < game_nb_pieces(newGame)) {
        switch (ch) {
        case KEY_LEFT:
            play_move(newGame, *choosenCar, LEFT, 1);
            break;
        case KEY_RIGHT:
            play_move(newGame, *choosenCar, RIGHT, 1);
            break;
        case KEY_DOWN:
            play_move(newGame, *choosenCar, DOWN, 1);
            break;
        case KEY_UP:
            play_move(newGame, *choosenCar, UP, 1);
            break;
        }
    }
}