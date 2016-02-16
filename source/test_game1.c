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
  for (int i = 0 ; i < NB_PIECES; i++)
    delete_piece(pieces[i]);
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
    tmp[0]= new_piece_rh(3, 3, true, true);
    tmp[1] = new_piece_rh(3, 0, true, false);
    tmp[2] = new_piece_rh(4, 1, true, true);
    tmp[3] = new_piece_rh(5, 3, false, false);
    game TmpGame;
    TmpGame = new_game_hr(NB_PIECES,pieces);
    result = result && test_equality_int(NB_PIECES, game_nb_pieces(TmpGame),"get_nb_piece");
    result = result && test_equality_int(4, get_x(game_piece(TmpGame,2)),"get_x game piece");
    result = result && test_equality_int(1, get_y(game_piece(TmpGame,2)),"get_x game piece");
    return result;
    
}

bool test_copy_game(){
    game tmp;
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
    result = result && test_equality_bool(true, game_over_hr(newGame), "game Over");
    if(!result){
        result = true;
        for(int i=0;i<game_nb_pieces(newGame);i++){
            if(get_x(game_piece(newGame,i)) == 4 && get_y(game_piece(newGame,i)) == 3)
                result = false;
        }
    } else {
        result = false;
        for(int i=0;i<game_nb_pieces(newGame);i++){
            if(get_x(game_piece(newGame,i)) == 4 && get_y(game_piece(newGame,i)) == 3)
                result = true;
        }
    }
    tear_down();
    return result;
          
}

bool test_play_move(){
    bool result= true;
    piece p;
    set_up();
    for (int dist = 1; dist < NB_PIECES; dist++)
    for (int i=0; i < NB_PIECES; i++) {
      copy_piece(game_piece(newGame,i),p);
      if (is_horizontal(game_piece(newGame,i))){
        result = result && test_equality_bool(true,play_move(newGame,i,LEFT,dist),"Play move LEFT");
        if(result){
            for(int j=0;j<game_nb_pieces(newGame);j++){
               result = result && test_equality_bool(false,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect LEFT");
            }
        } else {
            for(int j=0;j<game_nb_pieces(newGame);j++)
               if(test_equality_bool(true,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect LEFT"))
                   result = true;
        }
      }
      else
        result = result && test_equality_bool(false,play_move(newGame,i,LEFT,dist),"Play move LEFT");
      if (is_horizontal(game_piece(newGame,i))){
        result = result && test_equality_bool(true,play_move(newGame,i,RIGHT,dist),"Play move RIGHT");
        if(result){
            for(int j=0;j<game_nb_pieces(newGame);j++){
               result = result && test_equality_bool(false,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect RIGHT");
            }
        } else {
            for(int j=0;j<game_nb_pieces(newGame);j++)
               if(test_equality_bool(true,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect RIGHT"))
                   result = true;
        }
      }
      else
        result = result && test_equality_bool(false,play_move(newGame,i,RIGHT,dist),"Play move RIGHT");
      if (!is_horizontal(game_piece(newGame,i))){
        result = result && test_equality_bool(true,play_move(newGame,i,UP,dist),"Play move UP");
        if(result){
            for(int j=0;j<game_nb_pieces(newGame);j++){
               result = result && test_equality_bool(false,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect UP");
            }
        } else {
            for(int j=0;j<game_nb_pieces(newGame);j++)
               if(test_equality_bool(true,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect UP"))
                   result = true;
        }
      }
      else
        result = result && test_equality_bool(false,play_move(newGame,i,UP,dist),"Play move UP");
      if (is_horizontal(game_piece(newGame,i))){
        result = result && test_equality_bool(true,play_move(newGame,i,DOWN,dist),"Play move DOWN");
        if(result){
            for(int j=0;j<game_nb_pieces(newGame);j++){
               result = result && test_equality_bool(false,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect DOWN");
            }
        } else {
            for(int j=0;j<game_nb_pieces(newGame);j++)
               if(test_equality_bool(true,intersect(game_piece(newGame,i),game_piece(newGame,j)),"Intersect DOWN"))
                   result = true;
        }
      }
      else
        result = result && test_equality_bool(false,play_move(newGame,i,DOWN,dist),"Play move DOWN");
      

    }
    return result;
          
}

bool test_game_nb_move(){
    bool result= true;
    set_up();
    result = result && test_equality_bool(true,play_move(newGame,0,LEFT,2),"Play move LEFT");
    result = result && test_equality_bool(true,play_move(newGame,0,RIGHT,2),"Play move LEFT");
    result = result && test_equality_int(4,game_nb_moves(newGame),"Nb move");
    tear_down();
    return result;
          
}


int main (int argc, char *argv[])
{
  bool result= true;
  result = result && test_equality_bool(true, test_nb_pieces(), "test_nb_pieces");
  result = result && test_equality_bool(true, test_copy_game(), "test_copy_game");
  result = result && test_equality_bool(true, test_game_piece(),"test_game_piece");
  result = result && test_equality_bool(true, test_game_over(), "test_game_over");
  result = result && test_equality_bool(true, test_game_over(), "copy");

  if (result) {
    printf("Youpi !\n");
    return EXIT_SUCCESS;
  }
  else
    return EXIT_FAILURE;
}

