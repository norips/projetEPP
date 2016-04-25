#include <stdlib.h>
#include <stdio.h>
#include <MLV/MLV_all.h>
#include <game.h>

#include "tool/level_handler.h"
#include "tool/level_drawerMLV.h"
#include "tool/gfxMLV.h"

static bool game_over_an(cgame newGame)
{
    return get_x(game_piece(newGame, 0)) == 1 && get_y(game_piece(newGame, 0)) == 0;
}

int main(int argc, char *argv[])
{
    //
    // Créé et affiche la fenêtre
    //
    MLV_create_window("Test grid", "grid", 640, 480);
    MLV_change_frame_rate(100);
    MLV_clear_window(MLV_COLOR_WHITE);

    game newGame = NULL;
    int type = RUSH;
    MLV_Image * imgpieces[4];
    //Get level
    while (1) {
        bool gameOver = false;
        type = choose_game_type(50, 100);
        switch(type){
        case ANE:
            game_over = game_over_an;
            break;
        case RUSH:
            game_over = game_over_hr;
            break;
        }
        handle_level(&newGame);
        if (!newGame) {
            exit(EXIT_FAILURE);
        }
        imgpieces[0] = init_car(newGame, 0);
        for (int i = 1; i < game_nb_pieces(newGame); i++) {
            imgpieces[i] = init_car(newGame, i);
            if (!imgpieces[i]) {
                printf("Failed to load img");
                return -1;
            }
        }
        draw_grid(0, 0, game_width(newGame), game_height(newGame), RATIO, type);
        MLV_actualise_window();
        int click = -1;
        do { 
            play_events(newGame, &click, &gameOver);
            //
            // Met à jour l'affichage
            //
            MLV_clear_window(MLV_COLOR_WHITE);
            draw_score(game_width(newGame), 0, 3, game_height(newGame), RATIO, game_nb_moves(newGame));
            draw_grid(0, 0, game_width(newGame), game_height(newGame), RATIO, type);
            for (int i = 0; i < game_nb_pieces(newGame); i++) {
                int y = game_height(newGame) - get_y(game_piece(newGame, i)) - get_height(game_piece(newGame, i));
                draw_car(imgpieces[i], get_x(game_piece(newGame, i)), y, RATIO);
            }

            MLV_actualise_window();
            MLV_delay_according_to_frame_rate();
        } while (
                !game_over(newGame) && !gameOver
                );

        for (int i = 0; i < game_nb_pieces(newGame); i++) {
            MLV_free_image(imgpieces[i]);
        }
        delete_game(newGame);

        //
        // Met à jour l'affichage.
        //

        MLV_clear_window(MLV_COLOR_WHITE);
        if(gameOver) {
            MLV_draw_adapted_text_box(
                640 / 2 - (strlen("You loose !\nPress a key to continue")*2), 480 / 2 - 20,
                "You loose !\nPress a key to continue",
                9,
                MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                MLV_TEXT_CENTER
                );
        } else {
        MLV_draw_adapted_text_box(
                640 / 2 - (strlen("You win !\nPress a key to continue")*2), 480 / 2 - 20,
                "You win !\nPress a key to continue",
                9,
                MLV_COLOR_RED, MLV_COLOR_BLACK, MLV_COLOR_WHITE,
                MLV_TEXT_CENTER
                );
        }
        MLV_actualise_window();
        MLV_wait_keyboard(NULL,NULL,NULL);
    }
    //
    // Fermer la fenêtre
    //
    MLV_free_window();
    return 0;
}

