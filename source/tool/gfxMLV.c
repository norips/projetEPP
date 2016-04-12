/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdlib.h>
#include <stdio.h>
#include <game.h>
#include <MLV/MLV_all.h>

#include "tool/gfxMLV.h"
#include "tool/level_handler.h"

#define NB_OF_COLOR 3
char *carColor[] = {"aqua", "blue", "orange"};

void play_events(game newGame, int *click)
{

    MLV_Keyboard_button touche;
    MLV_Button_state state;
    MLV_Event event;
    int mouseX, mouseY;
    if (!click || !newGame) {
        fprintf(stderr, "play_events parameters error");
        exit(EXIT_FAILURE);
    }
    do {
        //
        // Récupère un évènement dans la file des évènements
        // non traités.
        //
        event = MLV_get_event(
                &touche, NULL, NULL,
                NULL, NULL,
                &mouseX, &mouseY, NULL,
                &state
                );
        //
        // Traite l'évènement s'il s'agit d'un évènement clavier
        //

        if (event == MLV_KEY || MLV_MOUSE_BUTTON) {
            if (event == MLV_MOUSE_BUTTON && state == MLV_PRESSED) {
                *click = getCarWithMouse(mouseX, mouseY, newGame);
            }
            if (event == MLV_MOUSE_BUTTON && state == MLV_RELEASED) {
                *click = -1;
            }
            if (event == MLV_MOUSE_MOTION && *click != -1) {
                cpiece tmp = game_piece(newGame, *click);
                int x = get_x(tmp);
                int y = game_height(newGame) - get_y(tmp) - get_height(tmp);
                if (mouseY > (y + get_height(tmp)) * RATIO)
                    play_move(newGame, *click, DOWN, 1);
                else if (mouseY < y * RATIO)
                    play_move(newGame, *click, UP, 1);
                else if (mouseX > (x + get_width(tmp)) * RATIO)
                    play_move(newGame, *click, RIGHT, 1);
                else if (mouseX < x * RATIO)
                    play_move(newGame, *click, LEFT, 1);
            }
        }
    } while (event != MLV_NONE);
}

void draw_score(int startx, int starty, int height, int width, int spaceBetweenRow, int score)
{
    char tmp[128];
    sprintf(tmp, "%d", score);
    MLV_draw_text_box(startx*spaceBetweenRow, starty*spaceBetweenRow, height*spaceBetweenRow, width*spaceBetweenRow, tmp, 9, MLV_COLOR_BROWN, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
}

int choose_game_type(int startx, int starty)
{
    MLV_Event event;
    int mouseX, mouseY;
    starty += 50;
    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        MLV_draw_text(startx, starty-50, "Please choose your game :", MLV_COLOR_RED);
        MLV_draw_text_box(
                startx, starty,
                540, 50,
                "Rush Hour",
                10,
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                MLV_TEXT_LEFT,
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
                );
        MLV_draw_text_box(
                startx, starty + 50,
                540, 50,
                "Ane Rouge",
                10,
                MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                MLV_TEXT_LEFT,
                MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER
                );
        do {
        event = MLV_get_event(
                NULL, NULL, NULL,
                NULL, NULL,
                &mouseX, &mouseY, NULL,
                NULL
                );
        if (event == MLV_MOUSE_BUTTON) {
            if (mouseX > startx && mouseX < (startx + 540) &&
                    mouseY > starty && mouseY < (starty + 50)) {
                return RUSH;
            }
            if (mouseX > startx && mouseX < (startx + 540) &&
                    mouseY > (starty + 50) && mouseY < (starty + 50 + 50)) {
                return ANE;
            }
        }
        
        } while (event != MLV_NONE);
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }
}

void draw_finish_line(int x1, int y1, int x2, int y2, int spaceBetweenRow, MLV_Color color)
{
    for (int i = -1; i < 2; i++)
        MLV_draw_line(x1 * spaceBetweenRow + i, y1 * spaceBetweenRow + i, x2 * spaceBetweenRow + i, y2 * spaceBetweenRow + i, color);
}

void draw_grid(int startx, int starty, int nbCol, int nbRow, int spaceBetweenRow, int type)
{
    int x = startx;
    int y = starty;
    for (int i = 0; i < nbRow; i++, y += spaceBetweenRow) {
        MLV_draw_rectangle(x, y, x + spaceBetweenRow*nbCol, spaceBetweenRow, MLV_COLOR_BLUE);
    }
    x = startx;
    y = starty;
    for (int i = 0; i < nbCol; i++, x += spaceBetweenRow) {
        MLV_draw_rectangle(x, y, spaceBetweenRow, y + spaceBetweenRow*nbRow, MLV_COLOR_BLUE);
    }
    switch (type) {
    case RUSH:
        draw_finish_line(6, 2, 6, 3, spaceBetweenRow, MLV_COLOR_RED);
        break;
    case ANE:
        draw_finish_line(1, 5, 3, 5, spaceBetweenRow, MLV_COLOR_RED);
        break;
    }
}

MLV_Image* init_car(game newGame, int indice_piece, int ratio)
{
    char size[6];
    char orientation[2];
    char color[30];
    char path[50];

    cpiece tmp = game_piece(newGame, indice_piece);
    int width = get_width(tmp) * ratio;
    int height = get_height(tmp) * ratio;
    if (get_width(tmp) >= 3 && get_height(tmp) >= 3)
        sprintf(size, "truck");
    else
        sprintf(size, "car");
    is_horizontal(tmp) ? sprintf(orientation, "h") : sprintf(orientation, "v");
    sprintf(color, "%s", carColor[indice_piece % NB_OF_COLOR]);
    sprintf(path, "../../assets/%s_%s_%s.png", size, color, orientation);
    if (indice_piece == 0)
        sprintf(path, "../../assets/car_red_h.png");
    MLV_Image *tmp_img = MLV_load_image(path);
    if (!tmp_img) {
        return NULL;
    }
    MLV_resize_image(tmp_img, width, height);
    return tmp_img;
}

void draw_car(MLV_Image *input, int col, int row, int spaceBetweenRow)
{
    MLV_draw_image(input, col*spaceBetweenRow, row * spaceBetweenRow);
}

int getCarWithMouse(int x, int y, game tmpGame)
{
    int miny, minx, maxx, maxy;
    for (int i = 0; i < game_nb_pieces(tmpGame); i++) {
        cpiece tmp = game_piece(tmpGame, i);
        minx = get_x(tmp) * RATIO;
        miny = (game_height(tmpGame) - get_y(tmp) - get_height(tmp)) * RATIO;
        maxx = (get_x(tmp) + get_width(tmp)) * RATIO;
        maxy = (game_height(tmpGame) - get_y(tmp)) * RATIO;
        if (x >= minx && x <= maxx &&
                y >= miny && y <= maxy) {
            return i;
        }
    }
    return -1;
}