# projetEPP
Rush-Hour game

# Compiling
You need to install ncurses to build this project.

### Ubuntu
* If you are on Ubuntu use `sudo apt-get update && sudo apt-get install libncurses5-dev` to install ncurses.

### Mac
* If you are on Mac and you have Homebrew use `brew install ncurses` to install ncurses.

Go to the build folder with `cd build` and do `cmake ..` and `make`. 

# Use
Lib are placed to build/lib.
To get there, you must typing in the terminal `cd build/lib`.

Binaries are placed to build/bin.
To get there, you must typing in the terminal `cd build/bin`.

# Play
To start the game, you have to go to the bin folder and do `./game_cli`.

# Code Coverage
Go to the build folder with `cd build` and do `cmake -DCoverage ..` and `make RushHour_coverage` and follow the instruction.

