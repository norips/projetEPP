#include <stdlib.h>
#include <stdio.h>


/*struct position{
  int x;
  int y;
};

struct pieces_s{
  int x;
  int y;
  bool small;
  bool horizontal;
  
};*/

piece new_piece_rh (int x, int y, bool small, bool horizontal){
    return NULL;
}

void delete_piece(piece p){

}

void copy_piece(cpiece src, piece dst){

}

void move_piece(piece p, dir d, int distance){

}

bool intersect(cpiece p1, cpiece p2){
  return true;
}

int get_x(cpiece p){
  return 0;
}

int get_y(cpiece p){
  return 0;
}

int get_height(cpiece p){
  return 0;
}

int get_width(cpiece p){
  return 0;
}

bool is_horizontal(cpiece p){
  return true;
}
