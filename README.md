# projetEPP
Rush-Hour game

# Compiling
You need to install ncurses and MLV to build this project.
You can find instruction to install MLV here : http://www-igm.univ-mlv.fr/~boussica/mlv/api/French/html/installation.html

### Ubuntu
* If you are on Ubuntu use `sudo apt-get update && sudo apt-get install libncurses5-dev` to install ncurses.

### Mac
* If you are on Mac and you have Homebrew use `brew install ncurses` to install ncurses.

Go to the build folder with `cd build` and do `cmake ..` and `make`. 

# Use
Lib are placed to build/lib.
To get there, you must type in the terminal `cd build/lib`.

Binaries are placed to build/bin.
To get there, you must type in the terminal `cd build/bin`.

# Play
### CLI
To start the game, you have to go to the bin folder and do `./game_cli`.
To load levels there need to be a `level` folder in the same directory as your working directory, with `.txt` file inside.
If no levels were found, game will load default game.

### Solver
To start the solver, you have to go to the bin folder and do `./solver a|r PathToLevel ShowPath` last parameter is optional.
GameType are : 
* a = Ane Rouge
* r = Rush Hour

### GUI
To start the gui, you must be in the bin folder and do `./game_gui`.

# Code Coverage
Go to the build folder with `cd build` and do `cmake -DCoverage=on ..` and `make && make RushHour_coverage` and follow the instruction.

