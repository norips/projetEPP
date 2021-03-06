
#include <ncurses.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tool/level_handler.h"
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
    if (fgets(buff, 1023, f) == NULL)
        return NULL;
    char *sch = strtok(buff, " ");
    int width = atoi(sch);
    sch = strtok(NULL, " ");
    int height = atoi(sch);
    if (fgets(buff, 1023, f) == NULL)
        return NULL;
    int n = atoi(buff);
    while (i < n) {
        if (fgets(buff, 1023, f) == NULL)
            return NULL;
        int x, y, widthp, heightp, move_x, move_y;
        sch = strtok(buff, " ");
        x = atoi(sch);
        sch = strtok(NULL, " ");
        y = atoi(sch);
        sch = strtok(NULL, " ");
        widthp = atoi(sch);
        sch = strtok(NULL, " ");
        heightp = atoi(sch);
        sch = strtok(NULL, " ");
        move_x = atoi(sch);
        sch = strtok(NULL, " ");
        move_y = atoi(sch);
        sch = strtok(NULL, " ");
        if (x < 0 || x > width || y < 0 || y > height || move_x < 0 || move_x > 1 || move_y < 0 || move_y > 1 || widthp <= 0 || heightp <= 0) {
            return NULL;
        }
        pieces[i] = new_piece(x, y, widthp, heightp, move_x, move_y);
        i++;
    }
    game newGame = new_game(width, height, n, pieces);
    for (int i = 0; i < game_nb_pieces(newGame); i++)
        delete_piece(pieces[i]);
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
                if (strcmp(ext + 1, "txt") == 0) {
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

