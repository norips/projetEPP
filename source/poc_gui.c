
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <game.h>
#define SIZE 8
#define MINH 27
#define MINW 73

WINDOW *create_newgrid(int height, int width, int starty, int startx);
WINDOW *create_newcar(int starty, int startx,bool small,bool horizontal,int number);
WINDOW *create_newscore(int height,int width,int starty, int startx,int score,bool playing);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[])
{	
    WINDOW *my_win,*score;
    WINDOW **car;
    game newGame;
    piece pieces[4];
    pieces[0] = new_piece_rh(3, 3, true, true);
    pieces[1] = new_piece_rh(3, 0, true, false);
    pieces[2] = new_piece_rh(4, 1, true, true);
    pieces[3] = new_piece_rh(5, 3, false, false);
    newGame = new_game_hr(4,pieces);
    car = malloc(sizeof(WINDOW*)*4);
    int startx, starty, width, height;
    int carch,dir,dist;

    initscr();			/* Start curses mode 		*/
    cbreak();			/* Line buffering disabled, Pass on
                                     * everty thing to me 		*/
    noecho();
    keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
    start_color();              /* Init color */
    use_default_colors();       /* Use default color, prevent screen to use default color aka black bg */
    init_pair(1, COLOR_RED, -1);/* Init color pair with red foreground and no color background aka transparent */
    height = (3*SIZE)+1;        /* Height of the grid */
    width = (3*SIZE)*2+1;       /* Width of the grid multiply by 2 because two char have the same size as a carriage return*/
    starty = 0;                 /* Top left pos of the terminal*/
    startx = 0;                 
    printw("Press 'q' to exit");
    refresh(); 

    int row,col;
    getmaxyx(stdscr,row,col);
    //Wait for good size
    char mesg[]="Please resize to more than 50x25 actual";
    while((row < MINH) || (col < MINW)){  getmaxyx(stdscr,row,col); mvprintw(row/2,(col-strlen(mesg))/2,"%s %dx%d",mesg,col,row); refresh(); clear();}
    my_win = create_newgrid(height, width, starty, startx); // Create new grid
    wmove(my_win,0,0);
    //Create all car from the game
    for(int i=0;i<game_nb_pieces(newGame);i++)
        car[i] = create_newcar(get_y(game_piece(newGame,i))*(SIZE/2)+1, (get_x(game_piece(newGame,i)))*SIZE+1, is_small(game_piece(newGame,i)),is_horizontal(game_piece(newGame,i)),i); // Create new car
    score = create_newscore(height,width/2,starty, startx+width,game_nb_moves(newGame),game_over_hr(newGame));
    //Loop while the game is not finished
    while(!game_over_hr(newGame))
    {	
        mvprintw(height,0,"Please choose car :");
        carch = getch();
        //Exit
        if(carch == 'q'){ endwin();return 0;}
        mvprintw(height+1,0,"Please choose dest :");
        dir = getch();
        //Exit
        if(dir == 'q'){ endwin();return 0;}
        mvprintw(height+1,0,"Please choose dist :");
        dist = getch();
        //Exit
        if(dist == 'q'){ endwin();return 0;}
        switch(dir){
            case KEY_LEFT:
                play_move(newGame,carch-'0',LEFT,dist-'0');
                break;
            case KEY_RIGHT:
                play_move(newGame,carch-'0',RIGHT,dist-'0');
                break;
            case KEY_DOWN:
                play_move(newGame,carch-'0',UP,dist-'0');
                break;
            case KEY_UP:
                play_move(newGame,carch-'0',DOWN,dist-'0');
                break;
        }
        destroy_win(my_win);                        /*Destroy window    */
        for(int i=0;i<game_nb_pieces(newGame);i++)  /*to create new one */
            destroy_win(car[i]);                    /*                  */
        destroy_win(score);                         /*                  */
        my_win = create_newgrid(height, width, starty,startx);
        wmove(my_win,0,0);
        for(int i=0;i<game_nb_pieces(newGame);i++)
            car[i] = create_newcar(get_y(game_piece(newGame,i))*(SIZE/2)+1, get_x(game_piece(newGame,i))*SIZE+1,is_small(game_piece(newGame,i)),is_horizontal(game_piece(newGame,i)),i); // Create new car
        wmove(my_win,0,0);
        score = create_newscore(height,width/2,starty, startx+width,game_nb_moves(newGame),game_over_hr(newGame));
        wmove(my_win,0,0);

    }

    endwin();			/* End curses mode		  */
    return 0;
}
/*
 * @brief Create a new grid
 * @param[in] height the height of the grid
 * @param[in] width the width of the grid
 * @param[in] starty the y pos to start drawing the grid
 * @param[in] startx the x pos to start drawing the grid
 * @return Created window
 */
WINDOW *create_newgrid(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
        for(int i=1;i<SIZE*3;i++){
            wmove(local_win,(SIZE/2)*i,1);
            whline(local_win,0,(3*SIZE)*2-1);
        }
        for(int i=1;i<SIZE*3;i++){
            wmove(local_win,1,(SIZE)*i);
            wvline(local_win,0,(SIZE*3)-1);
        }
        wmove(local_win,0,0);
        wmove(local_win,(SIZE/2)*3,(SIZE)*6);
        wattron(local_win,COLOR_PAIR(1)); //COLOR init with init_pair
        wattron(local_win,A_BOLD); //Bold char
        wvline(local_win,0,SIZE/2);
        wattroff(local_win,A_BOLD);
        wattroff(local_win,COLOR_PAIR(1));
        
	wrefresh(local_win);		/* Show that box 		*/

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
WINDOW *create_newcar(int starty, int startx,bool small,bool horizontal,int number)
{	
    WINDOW *local_win;
    int height=0;
    int width=0;
    //Setup size
    if(small && horizontal){
        height=(SIZE/2);
        width=(SIZE/2)*2*2;
    } else if(small && !horizontal){
        height=(SIZE/2)*2;
        width=(SIZE/2)*2+1;
    } else if(!small && horizontal){
        height=(SIZE/2);
        width=(SIZE/2)*3*2;
    } else if(!small && !horizontal){
        height=(SIZE/2)*3;
        width=(SIZE/2)*2+1;
    }
    //End setup
    //New car with border    
    local_win = newwin(height-1, width-2, starty, startx);
    wborder(local_win, '|', '|', '-','-','+','+','+','+');		/* 0, 0 gives default characters 
                                                                    * for the vertical and horizontal
                                             * lines			*/
    //Print number of the vehicle with color 
    wattron(local_win,COLOR_PAIR(1)); //COLOR init with init_pair
    wattron(local_win,A_BOLD); //Bold char
    mvwprintw(local_win,(height-1)/2, (width-2)/2,"%d",number);
    wattroff(local_win,A_BOLD);
    wattroff(local_win,COLOR_PAIR(1));
    wmove(local_win,0,0); //Set cur pos to top left car
    wrefresh(local_win);		/* Show that box 		*/

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
WINDOW *create_newscore(int height,int width,int starty, int startx,int score,bool over)
{	
    WINDOW *local_win;
    
    local_win = newwin(height, width, starty, startx);
    wborder(local_win, '|', '|', '-','-','+','+','+','+');		/* 0, 0 gives default characters 
                                                                    * for the vertical and horizontal
                                             * lines			*/
    char msg[] = "Status : ";
    char msg2[] = "Score : ";
    wmove(local_win,0,0); //Set cur pos to top left car
    mvwprintw(local_win,0,0,"%s%s",msg,over ? "Game Over" : "Playing");
    mvwprintw(local_win,height/2,(width-(strlen(msg2)+1))/2,"%s%d",msg2,score);
    wrefresh(local_win);		/* Show that box 		*/

    return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
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
