
#include <dirent.h>
#include <stdio.h>
#include <MLV/MLV_text.h>
#include <MLV/MLV_keyboard.h>
#include <MLV/MLV_all.h>
#include "tool/level_drawerMLV.h"
#include "tool/level_handler.h"
#include <game.h>
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

void draw_level(int startx, int starty, int height, int width, int choosen, char **name, int n, int* mini)
{
    if (choosen >= 6 + (*mini)) {
        (*mini)++;
    }
    if (choosen - (*mini) < 0) {
        (*mini)--;
    }
    int tmp = *mini;
    if (n > 6) {
        n = 6;
    }
    for (int i = 0; i < n; i++, tmp++) {
        if (choosen - (*mini) == i) {
            MLV_draw_text_box(startx, starty + i*height, width, height, name[tmp], 10, MLV_COLOR_RED, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
            continue;
        }
        MLV_draw_text_box(startx, starty + i*height, width, height, name[tmp], 10, MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_COLOR_WHITE, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_CENTER);
    }

}

bool detect_click(int x, int y, int *choosen, int nblevel, int mini)
{
    if (x > 600 && x < 630) {
        if (y > 100 && y < 250) {
            if (*choosen > 0) {
                (*choosen)--;
                return false;
            }
        } else if (y > 250 && y < 400) {
            if (*choosen < nblevel - 1) {
                (*choosen)++;
                return false;
            }
        }
    }
    if (x > 50 && x < 590 && y > 100 && y < 400)
        return true;
    return false;
}

void handle_level(game *newLevel)
{
    int choosen = 0;
    int nblevel = 3;
    int mini = 0;
    bool enter = false;
    char **levels = find_levels("../../level/", &nblevel);
    MLV_Image *level_select = MLV_load_image("../../assets/level_select.png");
    MLV_Image *arrows = MLV_load_image("../../assets/arrows.png");
    if (!levels) return;
    if (!level_select) return;
    if (!arrows) return;
    while (1) {
        MLV_clear_window(MLV_COLOR_WHITE);
        get_input(&choosen, nblevel, &enter, mini);
        if (enter) {
            char buff[1024];
            snprintf(buff, 1023, "../../level/%s", levels[choosen]);
            *newLevel = parse_level(buff);
            if (!*newLevel) {
                fprintf(stderr, "Can't parse level ! ");
                *newLevel = NULL;
            }
            return;
        }
        draw_level(50, 100, 50, 640 - 100, choosen, levels, nblevel, &mini);
        MLV_draw_image(level_select, 100, 20); //Select level
        MLV_draw_image(arrows, 600, 100);
        MLV_actualise_window();
        MLV_delay_according_to_frame_rate();
    }
}

void get_input(int *choosen, int nblevel, bool *enter, int mini)
{
    MLV_Keyboard_button touche;
    MLV_Button_state state;
    MLV_Event event;
    int mouseX, mouseY;
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
        if (event == MLV_KEY && state == MLV_PRESSED) {
            if (touche == MLV_KEYBOARD_DOWN) {
                if (*choosen < nblevel - 1) {
                    (*choosen)++;
                }
            } else if (touche == MLV_KEYBOARD_UP) {
                if (*choosen > 0) {
                    (*choosen)--;
                }
            } else if (touche == MLV_KEYBOARD_RETURN) {
                *enter = true;
            }
        }
        if (event == MLV_MOUSE_BUTTON && state == MLV_PRESSED) {
            *enter = detect_click(mouseX, mouseY, choosen, nblevel, mini);
        }

    } while (event != MLV_NONE);
}

