// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>

// Maze class
struct Maze {
    int rows;
    int columns;
    char** maze;
};

// Maze constructor, a function that returns a pointer to a new maze
struct Maze* Maze(int rows, int columns, string maze_string){
    Struct Maze *m = (struct Maze*) malloc(sizeof(struct Maze)); /* We declare a pointer m of the type struct Maze,
     and point it to an allocation of enough memory to hold the data type */
};