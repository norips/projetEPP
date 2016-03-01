#include "piece.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

static const int  smallSize = 2;
static const int  bigSize   = 3;
static const int thickness  = 1;
  

struct position{
  int x;   // (x;y) est le point de la piece en bas a gauche
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
  if (p1->isHorizontal && !(p2->isHorizontal)){
    if (get_x(p1)<= get_x(p2) && get_x(p2)< (get_x(p1)+get_width(p1)))
      if (get_y(p2)<= get_y(p1) && (get_y(p2)+get_height(p2))> get_y(p1))
	return true;
    return false;
  }
  if (p1->isHorizontal && p2->isHorizontal){
    if (get_x(p1)<= get_x(p2) && get_x(p2)< (get_x(p1)+get_width(p1)))
      if (get_y(p2)== get_y(p1))
	return true;
    return false;
  }
  if (!(p1->isHorizontal) && !(p2->isHorizontal)){
    if (get_x(p1) == get_x(p2))
      if (get_y(p2)<= get_y(p1)+get_height(p1) && (get_y(p2)+get_height(p2))> get_y(p1))
	return true;
    return false;
  }
  if (!(p1->isHorizontal) && p2->isHorizontal){
    if (get_x(p2)<= get_x(p1) && get_x(p1)< (get_x(p2)+get_width(p2)))
      if (get_y(p1)<= get_y(p2) && (get_y(p1)+get_height(p1))> get_y(p2))
	return true;
    return false;
  }
  return false;
}


int get_x(cpiece p){
  return p->position.x;
}

int get_y(cpiece p){
  return p->position.y;
}

int get_height(cpiece p){
  if(!(p->isHorizontal)){
    if (p->isSmall)
      return smallSize * thickness;
    else
      return bigSize * thickness;
  }
  else
    return thickness;
}

int get_width(cpiece p){
    if((p->isHorizontal)){
      if (p->isSmall)
	return smallSize * thickness;
      else
	return bigSize * thickness;
    }
    else
      return thickness;
}

bool is_horizontal(cpiece p){
  return p->isHorizontal;
}
