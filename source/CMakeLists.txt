add_library(game piece.c game.c)


add_executable(test_piece1 test_piece1.c)
add_executable(test_game1 test_game1.c)
target_link_libraries(test_piece1 game)
target_link_libraries(test_game1 game)
