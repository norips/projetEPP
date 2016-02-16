#include "piece.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct position{
  int x;
  int y;
};

struct piece_s{
  struct position position;
  bool small;
  bool horizontal;
  
};

piece new_piece_rh (int x, int y, bool small, bool horizontal){
  piece newp = malloc(sizeof(struct piece_s));
  newp->position.x=x;
  newp->position.y=y;
  newp->small = small;
  newp->horizontal = horizontal;
  return newp;
}

void delete_piece(piece p){
  p->position.x=0;
  p->position.y=0;
  p->small = false;
  p->horizontal = false;
  free(p);
}

void copy_piece(cpiece src, piece dst){
  dst->position.x=src->position.x;
  dst->position.y=src->position.y;
  dst->small = src->small;
  dst->horizontal = src->horizontal;
  
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
