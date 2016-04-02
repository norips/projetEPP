#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "piece.h"

#define NB_PIECES 4

/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 * @return true if expected==value
 */
bool test_equality_int(int expected, int value, char * msg)
{
    if (expected != value)
        fprintf(stderr, "ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
    return expected == value;
}

/**
 * @brief test if value is equal to expected; if not, displays an error message containing msg to standard error output 
 */
bool test_equality_bool(bool expected, bool value, char * msg)
{
    if (expected != value)
        fprintf(stderr, "ERR: value expected %d ; value computed %d. %s\n", expected, value, msg);
    return expected == value;
}

piece pieces[NB_PIECES];

/* configue de test
.....3
.....3
...003
......
...122
...1..
 */
void set_up_1()
{
    pieces[0] = new_piece_rh(3, 3, true, true);
    pieces[1] = new_piece_rh(3, 0, true, false);
    pieces[2] = new_piece_rh(4, 1, true, true);
    pieces[3] = new_piece_rh(5, 3, false, false);
}

void set_up_2()
{
    pieces[0] = new_piece(3, 3, 2, 1, true, false);
    pieces[1] = new_piece(3, 0, 1, 2, false, true);
    pieces[2] = new_piece(4, 1, 2, 1, true, false);
    pieces[3] = new_piece(5, 3, 1, 3, false, true);
}

void tear_down()
{
    for (int i = 0; i < NB_PIECES; i++)
        delete_piece(pieces[i]);
}

bool test_new_piece()
{
    bool result = true;
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 5; y++) {
            for (int compt_small = 0; compt_small < 2; ++compt_small) {
                for (int compt_horizontal = 0; compt_horizontal < 2; ++compt_horizontal) {
                    int size;
                    bool small;
                    bool horizontal;

                    if (compt_small == 0) {
                        small = true;
                        size = 2;
                    } else {
                        small = false;
                        size = 3;
                    }

                    if (compt_horizontal == 0)
                        horizontal = true;
                    else
                        horizontal = false;

                    piece p = new_piece_rh(x, y, small, horizontal);
                    result = result && test_equality_int(x, get_x(p), "get_x");
                    result = result && test_equality_int(y, get_y(p), "get_y");
                    if (horizontal) {
                        result = result && test_equality_int(1, get_height(p), "get_height");
                        result = result && test_equality_int(size, get_width(p), "get_width");
                        result = result && test_equality_bool(true, can_move_x(p), "can_move_x");
                        result = result && test_equality_bool(false, can_move_y(p), "can_move_y");
                    } else {
                        result = result && test_equality_int(size, get_height(p), "get_height");
                        result = result && test_equality_int(1, get_width(p), "get_width");
                        result = result && test_equality_bool(false, can_move_x(p), "can_move_x");
                        result = result && test_equality_bool(true, can_move_y(p), "can_move_y");
                    }
                    delete_piece(p);
                }
            }
            for (int width = 1; width < 4; width++) {
                for (int height = 1; height < 4; height++) {
                    for (int compt_move_x = 0; compt_move_x < 2; ++compt_move_x) {
                        for (int compt_move_y = 0; compt_move_y < 2; ++compt_move_y) {
                            bool move_x;
                            bool move_y;

                            if (compt_move_x == 0)
                                move_x = true;
                            else
                                move_x = false;

                            if (compt_move_y == 0)
                                move_y = true;
                            else
                                move_y = false;

                            piece p = new_piece(x, y, width, height, move_x, move_y);

                            result = result && test_equality_int(x, get_x(p), "get_x");
                            result = result && test_equality_int(y, get_y(p), "get_y");
                            result = result && test_equality_int(height, get_height(p), "get_height");
                            result = result && test_equality_int(width, get_width(p), "get_width");
                            if (move_x)
                                result = result && test_equality_bool(true, can_move_x(p), "can_move_x");
                            else
                                result = result && test_equality_bool(false, can_move_x(p), "not can_move_x");
                            if (move_y)
                                result = result && test_equality_bool(true, can_move_y(p), "can_move_y");
                            else
                                result = result && test_equality_bool(false, can_move_y(p), "not can_move_y");

                            delete_piece(p);
                        }
                    }
                }
            }
        }
    }
    return result;
}

bool test_intersect()
{
    bool result = true;
    //the first one (set_up_1()) is with new_piece_rh (int x, int y, bool small, bool horizontal)
    set_up_1();
    for (int i = 0; i < NB_PIECES; i++)
        for (int j = 0; j < NB_PIECES; j++) {
            result = result && test_equality_bool(i == j, intersect(pieces[i], pieces[j]), "intersect");
        }

    piece pb_piece1 = new_piece_rh(3, 3, false, false);
    piece pb_piece2 = new_piece_rh(3, 1, false, false);
    result = result && test_equality_bool(true, intersect(pieces[0], pb_piece1), "set up 1 intersect pb1");
    result = result && test_equality_bool(true, intersect(pb_piece2, pb_piece1), "set up 1 intersect pb2");
    tear_down();
    delete_piece(pb_piece1);
    delete_piece(pb_piece2);

    //the second one (set_up_2()) is with new_piece(int x, int y, int width, int height, bool move_x, bool move_y)
    set_up_2();
    for (int i = 0; i < NB_PIECES; i++)
        for (int j = 0; j < NB_PIECES; j++) {
            result = result && test_equality_bool(i == j, intersect(pieces[i], pieces[j]), "intersect");
        }

    piece pb_piece3 = new_piece(3, 3, 1, 3, false, true);
    piece pb_piece4 = new_piece(3, 1, 1, 3, false, true);
    result = result && test_equality_bool(true, intersect(pieces[0], pb_piece3), "set up 2 intersect pb1");
    result = result && test_equality_bool(true, intersect(pb_piece4, pb_piece3), "set up 2 intersect pb2");
    tear_down();
    delete_piece(pb_piece3);
    delete_piece(pb_piece4);

    return result;
}

bool test_move()
{
    bool result = true;
    piece p = new_piece_rh(0, 0, true, true);

    //the first one (set_up_1()) is with new_piece_rh (int x, int y, bool small, bool horizontal)
    set_up_1();
    for (int dist = 1; dist < NB_PIECES; dist++)
        for (int i = 0; i < NB_PIECES; i++) {
            copy_piece(pieces[i], p);
            move_piece(p, LEFT, dist);
            if (is_horizontal(pieces[i]))
                result = result && test_equality_int(get_x(pieces[i]) - dist, get_x(p), "move LEFT");
            else
                result = result && test_equality_int(get_x(pieces[i]), get_x(p), "move LEFT");
            copy_piece(pieces[i], p);
            move_piece(p, RIGHT, dist);
            if (is_horizontal(pieces[i]))
                result = result && test_equality_int(get_x(pieces[i]) + dist, get_x(p), "move RIGHT");
            else
                result = result && test_equality_int(get_x(pieces[i]), get_x(p), "move RIGHT");
            copy_piece(pieces[i], p);
            move_piece(p, UP, dist);
            if (!is_horizontal(pieces[i]))
                result = result && test_equality_int(get_y(pieces[i]) + dist, get_y(p), "move UP");
            else
                result = result && test_equality_int(get_y(pieces[i]), get_y(p), "move UP");
            copy_piece(pieces[i], p);
            move_piece(p, DOWN, dist);
            if (!is_horizontal(pieces[i]))
                result = result && test_equality_int(get_y(pieces[i]) - dist, get_y(p), "move DOWN");
            else
                result = result && test_equality_int(get_y(pieces[i]), get_y(p), "move DOWN");
        }
    tear_down();

    //the second one (set_up_2()) is with new_piece(int x, int y, int width, int height, bool move_x, bool move_y)
    set_up_2();
    for (int dist = 1; dist < NB_PIECES; dist++)
        for (int i = 0; i < NB_PIECES; i++) {
            copy_piece(pieces[i], p);
            move_piece(p, LEFT, dist);
            if (is_horizontal(pieces[i]))
                result = result && test_equality_int(get_x(pieces[i]) - dist, get_x(p), "move LEFT");
            else
                result = result && test_equality_int(get_x(pieces[i]), get_x(p), "move LEFT");
            copy_piece(pieces[i], p);
            move_piece(p, RIGHT, dist);
            if (is_horizontal(pieces[i]))
                result = result && test_equality_int(get_x(pieces[i]) + dist, get_x(p), "move RIGHT");
            else
                result = result && test_equality_int(get_x(pieces[i]), get_x(p), "move RIGHT");
            copy_piece(pieces[i], p);
            move_piece(p, UP, dist);
            if (!is_horizontal(pieces[i]))
                result = result && test_equality_int(get_y(pieces[i]) + dist, get_y(p), "move UP");
            else
                result = result && test_equality_int(get_y(pieces[i]), get_y(p), "move UP");
            copy_piece(pieces[i], p);
            move_piece(p, DOWN, dist);
            if (!is_horizontal(pieces[i]))
                result = result && test_equality_int(get_y(pieces[i]) - dist, get_y(p), "move DOWN");
            else
                result = result && test_equality_int(get_y(pieces[i]), get_y(p), "move DOWN");
        }
    tear_down();

    delete_piece(p);
    return result;
    return false;
}

bool test_copy()
{
    piece p = new_piece_rh(0, 0, true, true);
    bool result = true;

    //the first one (set_up_1()) is with new_piece_rh (int x, int y, bool small, bool horizontal)
    set_up_1();
    for (int i = 0; i < NB_PIECES; i++) {
        copy_piece(pieces[i], p);
        result = result && test_equality_int(get_height(pieces[i]), get_height(p), "copy get_height");
        result = result && test_equality_int(get_width(pieces[i]), get_width(p), "copy get_width");
        result = result && test_equality_int(get_x(pieces[i]), get_x(p), "copy get_x");
        result = result && test_equality_int(get_y(pieces[i]), get_y(p), "copy get_y");
        result = result && test_equality_bool(is_horizontal(pieces[i]), is_horizontal(p), "copy is_horizontal");
        result = result && test_equality_bool(can_move_x(pieces[i]), can_move_x(p), "copy can_move_x");
        result = result && test_equality_bool(can_move_y(pieces[i]), can_move_y(p), "copy can_move_y");
    }
    tear_down();

    //the second one (set_up_2()) is with new_piece(int x, int y, int width, int height, bool move_x, bool move_y)
    set_up_2();
    for (int i = 0; i < NB_PIECES; i++) {
        copy_piece(pieces[i], p);
        result = result && test_equality_int(get_height(pieces[i]), get_height(p), "copy get_height");
        result = result && test_equality_int(get_width(pieces[i]), get_width(p), "copy get_width");
        result = result && test_equality_int(get_x(pieces[i]), get_x(p), "copy get_x");
        result = result && test_equality_int(get_y(pieces[i]), get_y(p), "copy get_y");
        result = result && test_equality_bool(is_horizontal(pieces[i]), is_horizontal(p), "copy is_horizontal");
        result = result && test_equality_bool(can_move_x(pieces[i]), can_move_x(p), "copy can_move_x");
        result = result && test_equality_bool(can_move_y(pieces[i]), can_move_y(p), "copy can_move_y");
    }
    tear_down();
    delete_piece(p);
    return result;
}

void test_failure_piece(cpiece src)
{
    piece dst = NULL;
    copy_piece(src, dst);
    move_piece(dst, LEFT, 1);
    intersect(src, src);
    get_y(src);
    get_x(src);
    get_width(src);
    get_height(src);
    is_horizontal(src);
    can_move_x(src);
    can_move_y(src);
    delete_piece(dst);
}

int main(int argc, char *argv[])
{
    bool result = true;
    piece no_piece = NULL;

    printf("result of test error\n");
    test_equality_int(0, 1, "error test_equality_int ");
    test_equality_bool(true, false, "error test_equality_bool ");
    test_failure_piece(no_piece);
    printf("----------------------------------------------------------\n");
    delete_piece(no_piece);
    result = result && test_equality_bool(true, test_new_piece(), "new_piece");
    result = result && test_equality_bool(true, test_intersect(), "intersect");
    result = result && test_equality_bool(true, test_move(), "move");
    result = result && test_equality_bool(true, test_copy(), "copy");


    if (result) {
        printf("Youpi !\n");
        return EXIT_SUCCESS;
    } else
        return EXIT_FAILURE;
}

