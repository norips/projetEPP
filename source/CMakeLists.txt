find_package(Curses REQUIRED)
add_library(game libgame/piece.c libgame/game.c)


add_executable(test_piece1 test/test_piece1.c)
add_executable(test_game1 test/test_game1.c)
add_executable(game_cli cli/game_cli.c)
add_executable(level_1 cli/level_1.c)
target_link_libraries(test_piece1 game)
target_link_libraries(test_game1 game)
target_link_libraries(game_cli game ncurses)
target_link_libraries(level_1 game)
