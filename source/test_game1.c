#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "game.h"

#define NB_PIECES 4

game newGame;
piece pieces[NB_PIECES];
/* configue de test
.....3
.....3
...003
......
...122
...1..
 */
void set_up() {
  pieces[0] = new_piece_rh(3, 3, true, true);
  pieces[1] = new_piece_rh(3, 0, true, false);
  pieces[2] = new_piece_rh(4, 1, true, true);
  pieces[3] = new_piece_rh(5, 3, false, false);
  newGame = new_game_hr(NB_PIECES,pieces);
}
void tear_down() {
  delete_game(newGame);
}
/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 * @return true if expected==value
 */
bool test_equality_int(int expected, int value, char * msg) {
  if (expected != value)
    fprintf(stderr,"ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
  return expected == value;
}

/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 */
bool test_equality_bool(bool expected, bool value, char * msg) {
  if (expected != value)
    fprintf(stderr,"ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
  return expected == value;
}


bool test_new_game(){
    bool result=true;
    piece tmp[4];
    tmp[0] = new_piece_rh(3, 3, true, true);
    tmp[1] = new_piece_rh(3, 0, true, false);
    tmp[2] = new_piece_rh(4, 1, true, true);
    tmp[3] = new_piece_rh(5, 3, false, false);
    game TmpGame;
    TmpGame = new_game_hr(NB_PIECES,tmp);
    result = result && TmpGame;
    result = result && test_equality_int(NB_PIECES, game_nb_pieces(TmpGame),"get_nb_piece");
    result = result && test_equality_int(get_x(tmp[0]), get_x(game_piece(TmpGame,0)),"get_x game piece 1");
    result = result && test_equality_int(get_y(tmp[0]), get_y(game_piece(TmpGame,0)),"get_y game piece 1");
    result = result && test_equality_int(get_x(tmp[1]), get_x(game_piece(TmpGame,1)),"get_x game piece 2");
    result = result && test_equality_int(get_y(tmp[1]), get_y(game_piece(TmpGame,1)),"get_y game piece 2");
    result = result && test_equality_int(get_x(tmp[2]), get_x(game_piece(TmpGame,2)),"get_x game piece 3");
    result = result && test_equality_int(get_y(tmp[2]), get_y(game_piece(TmpGame,2)),"get_y game piece 3");
    result = result && test_equality_int(get_x(tmp[3]), get_x(game_piece(TmpGame,3)),"get_x game piece 4");
    result = result && test_equality_int(get_y(tmp[3]), get_y(game_piece(TmpGame,3)),"get_y game piece 4");
    return result;
}

bool test_copy_game(){
    piece piecetmp = new_piece_rh(0,0,false,false);
    game tmp = new_game_hr(1,&piecetmp);
    bool result= true;
    set_up();
    copy_game(newGame,tmp);
    result = result && test_equality_int(game_nb_pieces(newGame), game_nb_pieces(tmp), "copy get_Pieces");
    tear_down();
    return result;
}
bool test_nb_pieces(){
    bool result= true;
    set_up();
    result = result && test_equality_int(NB_PIECES,game_nb_pieces(newGame),"game nb pieces");
    tear_down();
    return result;
}
bool test_game_piece(){
    bool result= true;
    set_up();
    for(int i=0;i<game_nb_pieces(newGame);i++){
        cpiece p = game_piece(newGame,i);
        result = result && test_equality_int(get_height(pieces[i]), get_height(p), "getpiece get_height");
        result = result && test_equality_int(get_width(pieces[i]), get_width(p), "getpiece get_width");
        result = result && test_equality_int(get_x(pieces[i]), get_x(p), "getpiece get_x");
        result = result && test_equality_int(get_y(pieces[i]), get_y(p), "getpiece get_y");
        result = result && test_equality_bool(is_horizontal(pieces[i]), is_horizontal(p), "getpiece is_horizontal");
    }
    tear_down();
    return result;
          
}
bool test_game_over(){
    bool result= true;
    set_up();
    result = result && test_equality_bool(true,play_move(newGame,0,LEFT,3),"1 move");
    result = result && test_equality_bool(true,play_move(newGame,1,UP,4),"2 move");
    result = result && test_equality_bool(true,play_move(newGame,2,LEFT,1),"3 move");
    result = result && test_equality_bool(true,play_move(newGame,2,LEFT,1),"4 move");
    result = result && test_equality_bool(true,play_move(newGame,3,DOWN,3),"5 move");
    result = result && test_equality_bool(true,play_move(newGame,0,RIGHT,4),"6 move");
    result = result && test_equality_bool(true,game_over_hr(newGame),"Game_Over");
    tear_down();
    return result;
          
}

bool test_play_move(){
    bool result= true;
    set_up();
    result = result && test_equality_bool(false,play_move(newGame,0,RIGHT,1),"RIGHT 1 piece 0 Intersect");
    result = result && test_equality_bool(true,play_move(newGame,0,LEFT,3),"LEFT 3 piece 0");
    result = result && test_equality_bool(true,play_move(newGame,0,RIGHT,2),"RIGHT 2 piece 0");
    result = result && test_equality_bool(false,play_move(newGame,3,RIGHT,2),"RIGHT 2 piece 3 Impossible");
    result = result && test_equality_bool(false,play_move(newGame,3,LEFT,2),"LEFT 2 piece 3 Impossible");
    result = result && test_equality_bool(true,play_move(newGame,3,DOWN,1),"DOWN 1 piece 3 ");
    result = result && test_equality_bool(true,play_move(newGame,3,UP,1),"UP 1 piece 3 ");
    result = result && test_equality_bool(false,play_move(newGame,1,DOWN,1),"DOWN 1 piece 1 Out of Bounds > 6");
    result = result && test_equality_bool(false,play_move(newGame,3,UP,1),"UP 1 piece 3 Out of Bounds < 0");
    result = result && test_equality_bool(false,play_move(newGame,0,LEFT,4),"LEFT 1 piece 0 Out of Bounds < 0");
    tear_down();
    return result;
          
}

bool test_game_nb_move(){
    bool result= true;
    set_up();
    result = result && test_equality_bool(true,play_move(newGame,0,LEFT,3),"Play move LEFT");
    result = result && test_equality_bool(true,play_move(newGame,0,RIGHT,2),"Play move LEFT");
    result = result && test_equality_int(5,game_nb_moves(newGame),"Nb move");
    tear_down();
    return result;
          
}


int main (int argc, char *argv[])
{
  bool result= true;
  result = result && test_equality_bool(true, test_new_game(), "test_new_game");
  result = result && test_equality_bool(true, test_nb_pieces(), "test_nb_pieces");
  result = result && test_equality_bool(true, test_game_piece(),"test_game_piece");
  result = result && test_equality_bool(true, test_copy_game(), "test_copy_game");
  result = result && test_equality_bool(true, test_game_nb_move(), "test_game_nb_move");
  result = result && test_equality_bool(true, test_play_move(), "test_play_move");
  result = result && test_equality_bool(true, test_game_over(), "test_game_over");

  if (result) {
    printf("Youpi !\n");
    return EXIT_SUCCESS;
  }
  else
    return EXIT_FAILURE;
}

