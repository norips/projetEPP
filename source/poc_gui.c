
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game.h"
#include "poc_gui.h"
int MINH = 0;
int MINW = 0;

int MAXCOL = 6;
int MAXROW = 6;

static int get_car_with_mouse(int y, int x, WINDOW** winCar, int nbpieces)
{
    for (int i = 0; i < nbpieces; i++) {
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

static void setup()
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

static void wait_for_size()
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

static void show_instruction()
{
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 1, col / 2 - 20, "Please use your mouse to select car or use it number");
    mvprintw(row / 2, col / 2 - 20, "Use arrow key to move car, selected car is in green");
    mvprintw(row / 2 + 2, col / 2 - 20, "Press any key to continue");
    //Show
    refresh();
    //Wait to enter
    while (getch() == KEY_RESIZE) {
        wait_for_size();
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 1, col / 2 - 20, "Please use your mouse to select car or use it number");
        mvprintw(row / 2, col / 2 - 20, "Use arrow key to move car, selected car is in green");
        mvprintw(row / 2 + 2, col / 2 - 20, "Press any key to continue");
        //Show
        refresh();

    }
    mousemask(ALL_MOUSE_EVENTS, NULL); /* Report all mouse events */
    clear();
    refresh();
}

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
        newGame = new_game(6, 6, 2, pieces);
        MAXCOL = game_width(newGame);
        MAXROW = game_height(newGame);
        MINH = MAXROW * SIZE;
        MINW = MAXCOL * SIZE;
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
        MINH = MAXROW * SIZE;
        MINW = MAXCOL * SIZE;
        for (int i = 0; i < game_nb_pieces(newGame); i++) {
            delete_piece(pieces[i]);
        }
        clear();
        refresh();
        return newGame;
    }




}

int main(int argc, char *argv[])
{
    WINDOW *my_win, *score;
    WINDOW **car;
    game newGame;
    MEVENT event;

    int ch = 0, choosenCar = -1;

    //INIT
    setup();
    //END INIT
    //Wait for good size
    wait_for_size();
    //Instruction
    show_instruction();
    //Select game
    newGame = select_game();
    car = malloc(sizeof (WINDOW*) * game_nb_pieces(newGame));
    //First draw
    draw_game(newGame, 0, 0, &my_win, car, &score, choosenCar);
    //Loop while the game is not finished
    while (!game_over_hr(newGame)) {
        //Print on bottom of grid
        mvprintw(MAXROW * SIZE + 1, 0, "Please choose car :");
        ch = getch();
        if (KEY_MOUSE == ch) {
            /* Mouse event. */
            if (OK == getmouse(&event)) {
                choosenCar = get_car_with_mouse(event.y, event.x, car, game_nb_pieces(newGame));
            }
        } else {
            if (ch == 'q') {
                break;
            }
            play_input(newGame, ch, &choosenCar);
        }
        wait_for_size();
        erase_game(newGame, my_win, car, score);
        draw_game(newGame, 0, 0, &my_win, car, &score, choosenCar);

    }
    for (int i = 0; i < game_nb_pieces(newGame); i++) {
        destroy_win(car[i]);
    }
    destroy_win(my_win);
    destroy_win(score);
    free(car);
    delete_game(newGame);
    endwin(); /* End curses mode		  */
    return 0;
}

/*
 * @brief Draw a new game
 * 
 * @param[in] The game to draw
 * @param startx the x pos to start drawing the grid
 * @param starty the y pos to start drawing the grid
 * @param[out] grid Address of pointer to your grid's window
 * @param[out] car Your array of car
 * @param[out] score Address of pointer to your score's window
 * @param choosenCar indice of the choosen car
 * @return Nothing
 */
void draw_game(game newGame, int starty, int startx, WINDOW **grid, WINDOW **car, WINDOW **score, int choosenCar)
{
    *grid = create_newgrid(starty, startx, MAXROW, MAXCOL, SIZE); // Create new grid
    wmove(*grid, 0, 0);
    //Create each car and highlight the selected one
    for (int i = 0; i < game_nb_pieces(newGame); i++) {
        if (choosenCar == i) {
            car[i] = create_newcar(game_piece(newGame, i), i, true, SIZE); // Create new car and highlight it
            continue;
        }
        car[i] = create_newcar(game_piece(newGame, i), i, false, SIZE); // Create new car
    }
    wmove(*grid, 0, 0);
    //Create score on right of the grid
    *score = create_newscore(SIZE * MAXROW + 1, SIZE*MAXCOL, starty, startx + SIZE * MAXCOL * 2 + 1, game_nb_moves(newGame), game_over_hr(newGame));
    wmove(*grid, 0, 0);
}

/*
 * @brief Erase a game
 * 
 * @param[in] The game to erase
 * @param[in] grid Address of pointer to your grid's window
 * @param[in] car Your array of car
 * @param[in] score Address of pointer to your score's window
 * @return Nothing
 */
void erase_game(game newGame, WINDOW *my_win, WINDOW **car, WINDOW *score)
{
    destroy_win(my_win); /*Destroy window    */
    for (int i = 0; i < game_nb_pieces(newGame); i++) /*to create new one */
        destroy_win(car[i]); /*                  */
    destroy_win(score); /*                  */
}

/*
 * @brief Play move using input
 * 
 * @param[in] The game to play on
 * @param ch Input
 * @param[out] car The choosen car
 * @return Nothing
 */
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
            play_move(newGame, *choosenCar, UP, 1);
            break;
        case KEY_UP:
            play_move(newGame, *choosenCar, DOWN, 1);
            break;
        }
    }
}

/*
 * @brief Create a new grid
 * @param[in] height the height of the grid
 * @param[in] width the width of the grid
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @return Created window
 */
WINDOW *create_newgrid(int starty, int startx, int nbRow, int nbCol, int spaceBetween)
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
    wmove(local_win, (spaceBetween)*3 + 1, (spaceBetween * 2) * nbCol);
    wattron(local_win, COLOR_PAIR(1)); //COLOR init with init_pair
    wattron(local_win, A_BOLD); //Bold char
    wvline(local_win, '#', spaceBetween);
    wattroff(local_win, A_BOLD);
    wattroff(local_win, COLOR_PAIR(1));

    wrefresh(local_win); /* Show that box 		*/

    return local_win;
}

/*
 * @brief Create a new car
 * 
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @param small indicates if the vehicle is of size 2 (small=true) or 3 (small=false)
 * @param horizontal indicates whether the vehicle can move horizontally or vertically
 * @param number indicates the number of the vehicule
 * @return Created window
 */
WINDOW *create_newcar(cpiece newPiece, int number, bool selected, int spaceBetween)
{
    WINDOW *local_win;
    int height = 0;
    int width = 0;
    int starty = get_y(newPiece);
    int startx = get_x(newPiece);
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

/*
 * @brief Create a new score
 * @param[in] height the height of the grid
 * @param[in] width the width of the grid
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @param[in] score the current score to be drawing on screen
 * @param[in] over wether the game is over or not
 * @return Created window
 */
WINDOW *create_newscore(int height, int width, int starty, int startx, int score, bool over)
{
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+'); /* 0, 0 gives default characters 
                                                                    * for the vertical and horizontal
                                             * lines			*/
    char msg[] = "Status : ";
    char msg2[] = "Score : ";
    wmove(local_win, 0, 0); //Set cur pos to top left car
    mvwprintw(local_win, 0, 0, "%s%s", msg, over ? "Game Over" : "Playing");
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