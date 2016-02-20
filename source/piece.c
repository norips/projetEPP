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
  bool isSmall;
  bool isHorizontal;
  
};

piece new_piece_rh (int x, int y, bool small, bool horizontal){
  piece newp = malloc(sizeof(struct piece_s));
  newp->position.x=x;
  newp->position.y=y;
  newp->isSmall = small;
  newp->isHorizontal = horizontal;
  return newp;
}

void delete_piece(piece p){
  free(p);
}

void copy_piece(cpiece src, piece dst){
  dst->position.x=src->position.x;
  dst->position.y=src->position.y;
  dst->isSmall = src->isSmall;
  dst->isHorizontal = src->isHorizontal;
  
}

void move_piece(piece p, dir d, int distance){
  if(p->isHorizontal){
    if (d == LEFT)
      p->position.x -= distance;
    if (d == RIGHT)
      p->position.x += distance;
  }else{
    if (d == UP)
      p->position.y += distance;
    if (d == DOWN)
      p->position.y -= distance;
  }   
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
