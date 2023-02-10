// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

// TUPLE CLASS //
struct Tuple {
    int i;
    int j;
};

struct Tuple* Tuple(int i, int j){
    struct Tuple* ans = malloc(sizeof(struct Tuple));
    ans->i = i;
    ans->j = j;
    return ans;
}

// END OF TUPLE CLASS//

// MAZE CLASS //
struct Maze {
    int rows;
    int columns;
    char** matrix;
    struct Tuple start;
    struct Tuple finish;
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
        } else if (*aux == '\n'){
            doneColumns = 1;
            lRows ++;
        }
        aux++;
    }
    lRows++; // Don't forget that last one
    // Declaration of pointer m to allocation of memory that will hold the object of type Maze, and assignment of its attributes
    struct Maze *m = (struct Maze*) malloc(sizeof(struct Maze));
    (*m).rows = lRows;
    (*m).columns = lColumns;

    // Memory allocation for maze matrix
    char ** matrix = (char **) malloc(((*m).columns+1) * sizeof(char*));
    for (int i=0; i < lRows; i++)
        matrix[i] = (char*) malloc(((*m).columns + 1) * sizeof(char));

    // From string to matrix huzzah!
    aux = maze_string; // Auxiliary pointer reset
    int i = 0; // row iteration variable
    int j = 0; // column iteration variable
    int s = 0; // boolean start found
    int f = 0; // boolean finished found
    while(aux && i<lRows) {
        if (*aux != '\n' && *aux != '\0') {
            if (*aux == 's'){
                s = 1;
                m->start = *Tuple(i,j);
            }
            if(*aux == 'f') {
                f = 1;
                m->finish = *Tuple(i,j);
            }
            matrix[i][j] = *aux;
            j++;
        } else {
            matrix[i][j] = '\0';
            i++;
            j=0;
        }
        aux++;
    }
    (*m).matrix = matrix;
    return m;
};

// Maze to String
char* toString(struct Maze maze){
    char* ans = (char*) malloc(sizeof(char) * maze.columns * maze.rows + maze.rows);
    char* aux = ans;
    // matrix[i][j]
    for(int i = 0; i < maze.rows; i++){
        for (int j = 0; j < maze.columns; j++){
            *aux = maze.matrix[i][j];
            aux++;
        }
        *aux = '\n';
        aux++;
    }
    *aux = '\0';
    return ans;
}

struct Maze* copyMaze(struct Maze* maze){
    struct Maze* ans = malloc(sizeof(*maze));
    ans->columns = maze->columns;
    ans->rows   = maze->rows;
    ans->start  = *Tuple(maze->start.i, maze->start.j);
    ans->finish = *Tuple(maze->finish.i, maze->finish.j);
    ans->matrix = (char**)malloc(maze->rows * sizeof(char*));
    for (int i = 0; i < maze->rows; i++) {
        ans->matrix[i] = (char*)malloc(maze->columns * sizeof(char));
        for (int j = 0; j < maze->columns; j++) {
            ans->matrix[i][j] = maze->matrix[i][j];
        }
    }
    return ans;
}

// END OF MAZE CLASS //

// Solve maze //

struct Maze* solveMazeDFS(struct Maze* maze){
    struct Maze* ans = malloc(sizeof(maze));

    return ans;
}


int main(){

    char* input = "s# ##  #\n"
                  "###   # \n"
                  "###### f\0";
    struct Maze* maze = Maze(input);
    printf("columns: %d\n", maze->columns);
    printf("rows: %d\n", maze->rows);

    char** matrix = (*maze).matrix;
    printf("address of matrix: %p\n", matrix);
    printf("s = %d, %d\n", maze->start.i, maze->start.j);
    printf("f = %d, %d\n", maze->finish.i, maze->finish.j);
    printf("");
    char* totring = toString(*maze);

    printf("%s\n", totring);

    printf("%c\n", maze->matrix[1][1]);

    // copyMaze



    printf("----CopyMaze----\n");

    struct Maze* cmaze = copyMaze(maze);
    printf("columns: %d\n", cmaze->columns);
    printf("rows: %d\n", cmaze->rows);

    char** cmatrix = (*cmaze).matrix;
    printf("address of matrix: %p\n", cmatrix);
    printf("s = %d, %d\n", cmaze->start.i, cmaze->start.j);
    printf("f = %d, %d\n", cmaze->finish.i, cmaze->finish.j);
    printf("");
    char* ctotring = toString(*cmaze);

    printf("%s\n", ctotring);
    return 0;

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