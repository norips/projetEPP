
#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "levelHandler.h"
#include <game.h>

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

game parse_level(char *levelFound)
{
    FILE *f = fopen(levelFound, "r");
    if (!f) {
        return NULL;
    }
    char buff[1024];
    piece pieces[1024];
    int i = 0;
    fgets(buff, 1023, f);
    int width = buff[0] - '0';
    int height = buff[2] - '0';
    fgets(buff, 1023, f);
    int n = atoi(buff);
    while (i < n) {
        fgets(buff, 1023, f);
        int x, y, widthp, heightp, move_x, move_y;
        x = buff[0] - '0';
        y = buff[2] - '0';
        widthp = buff[4] - '0';
        heightp = buff[6] - '0';
        move_x = buff[8] - '0';
        move_y = buff[10] - '0';
        if (x < 0 || x > 9 || y < 0 || y > 9 || move_x < 0 || move_x > 1 || move_y < 0 || move_y > 1 || widthp <= 0 || heightp <= 0) {
            return NULL;
        }
        pieces[i] = new_piece(x, y, widthp, heightp, move_x, move_y);
        i++;
    }
    game newGame = new_game(width, height, i, pieces);
    return newGame;
}

char** find_levels(char *path, int *nblevel)
{
    char **arrFile, **tmp;
    int i = 0, taille = 4;
    arrFile = malloc(sizeof (char*)*taille);
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (i > taille) {
                taille *= 2;
                tmp = malloc(sizeof (char*)*taille);
                for (int j = 0; j < i; j++)
                    tmp[j] = arrFile[j];
                free(arrFile);
                arrFile = tmp;
            }
            //Get extension name
            char *ext = strrchr(dir->d_name, '.');
            if (!ext) {
                /* no extension */
            } else {
                //Get only *.lvl file
                if (strcmp(ext + 1, "lvl") == 0) {
                    arrFile[i] = malloc(sizeof (char)*strlen(dir->d_name) + 1);
                    strcpy(arrFile[i], dir->d_name);
                    i++;
                }
            }
        }
        closedir(d);
    }
    *nblevel = i;
    return arrFile;

}

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
    int nblevel = 0;
    int mini = 0;
    bool enter = false;
    char *path = "level/";
    char **levels = find_levels(path, &nblevel);
    while (1) {
        clear();
        if (!draw_level(choosen, levels, nblevel, &mini)) {
            return false;
        }
        get_input(&choosen, nblevel, &enter, mini);
        if (enter) {
            char buff[1024];
            snprintf(buff, 1023, "%s%s", path, levels[choosen]);
            *newLevel = parse_level(buff);
            if (!*newLevel) {
                fprintf(stderr, "Can't parse level ! ");
                return false;
            }
            return true;
        }
        refresh();
    }
}

