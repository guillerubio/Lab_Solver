// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// Maze class
struct Maze {
    int rows;
    int columns;
    // char** maze;
};

// Maze constructor, a function that returns a pointer to a new maze
struct Maze* Maze(char* maze_string){
    // Local counters, variables and pointers
    char* aux = maze_string; // Auxiliary pointer for iteration
    int lRows = 0;
    int lColumns = 0;
    int doneColumns = 0; // Boolean
    while(*aux) { // Count columns and rows
        if (!doneColumns && *aux != '\n') {
            lColumns ++;
        } else if (*aux == '\n' || *aux == '/0'){
            doneColumns = 1;
            lRows ++;
        }
        aux++;
    }
    // Declaration of pointer m to allocation of memory that will hold the object of type Maze, and assignment of its attributes
    struct Maze *m = (struct Maze*) malloc(sizeof(struct Maze));
    (*m).rows = lRows;
    (*m).columns = lColumns;

    return m;

};

int main(){

    char* input = "## ##  #\n"
                  "###   # \n"
                  "######  \n";
    struct Maze* maze = Maze(input);
    printf("%d\n", maze->columns);
    printf("%d\n", maze->rows);
    return 0;

//    char *input = "  #  \n ##  \n#   #\n";
//    struct maze *m = create_maze(input);
//
//    char* array = (char*) malloc(20*sizeof(char));
//    strcpy(array, "buenas tardes");
//    printf("%s\n",array);
//    int b = 1;
//    printf("%i\n",b);
//    if (b) {
//        printf("%s\n", "true");
//    } else {
//        printf("%s\n", "false");
//  }



}