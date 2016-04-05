/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <ncurses.h>
#include <stdlib.h>
#include "tool/level_handler.h"
#include "tool/level_drawer.h"

bool draw_level(int choosen, char **name, int n, int* mini)
{
    if (choosen >= 6 + (*mini)) {
        (*mini)++;
    }
    if (choosen - (*mini) < 0) {
        (*mini)--;
    }
    int tmp = *mini;
    int nbLevel = n;
    if (n > 6) {
        n = 6;
    }
    if (n == 0) {
        mvprintw(0, 0, "No level found !");
        refresh();
        return false;
    }
    mvprintw(0, 0, "Please choose your level : %d / %d", choosen + 1, nbLevel);
    for (int i = 0; i < n; i++, tmp++) {
        if (choosen - (*mini) == i) {
            mvprintw(1 + i, 0, "->%s", name[tmp]);
            continue;
        }
        mvprintw(1 + i, 2, "%s", name[tmp]);
    }
    return true;
}

void get_input(int *choosen, int nblevel, bool *enter, int mini)
{
    int event = 0;
    event = getch();
    //
    // Traite l'évènement s'il s'agit d'un évènement clavier
    //
    if (event == KEY_DOWN) {
        if (*choosen < nblevel - 1) {
            (*choosen)++;
        }
    } else if (event == KEY_UP) {
        if (*choosen > 0) {
            (*choosen)--;
        }
    } else if (event == '\n') {
        *enter = true;
    }
}

bool handle_level(game *newLevel)
{
    int choosen = 0;
    int nbLevel = 0;
    int mini = 0;
    bool enter = false;
    char *path = "level/";
    char **levels = find_levels(path, &nbLevel);
    while (1) {
        clear();
        if (!draw_level(choosen, levels, nbLevel, &mini)) {
            return false;
        }
        get_input(&choosen, nbLevel, &enter, mini);
        if (enter) {
            char buff[1024];
            snprintf(buff, 1023, "%s%s", path, levels[choosen]);
            game result = parse_level(buff);
            if (!result) {
                fprintf(stderr, "Can't parse level ! ");
                return false;
            }
            for (int i = 0; i < nbLevel; i++)
                free(levels[i]);
            free(levels);
            *newLevel = result;
            return true;
        }
        refresh();
    }
}