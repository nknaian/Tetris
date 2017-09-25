# Tetris
- To try out the game, go in the "Release" folder and download the "Tetris.exe" executable. 
- To To view the source code, go in the "Tetris" folder. 
  - "Tetris.cpp" contains the main function, and is where the gameflow is managed and user input is handled
  - There are 3 main classes that define the objects used in the program:
    1. Grid: The smallest object, one square on the board. Drawing occurs in units of "Grid".
    2. Shape: The current falling shape is an object made up of 4 grids. There are 7 types of types of shapes that all inherit from the class "Shape," each having its own color and grid arrangement
    3. Board: The board is the playing field and is made up of an array of grids and one falling shape at a time (the "currentShape") that interacts with the grids already "locked in."
