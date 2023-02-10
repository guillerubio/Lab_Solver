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

// TUPLE STACK CLASS //
struct TupleStack {
    struct Tuple* list;
    int size;
};

struct TupleStack* TupleStack(int maxSize){
    struct TupleStack* ans = malloc(sizeof(TupleStack));
    ans->list = malloc(maxSize * sizeof(struct Tuple));
    ans->size = 0;
    return ans;
}

struct Tuple pop (struct TupleStack* tupleStack){
    tupleStack->size--;
    return tupleStack->list[tupleStack->size];
}

void push (struct TupleStack* tupleStack, struct Tuple* tuple) {
    tupleStack->list[tupleStack->size] = *tuple;
    tupleStack->size++;
}
// END OF TUPLE STACK CLASS //

// Solve maze //

struct Maze* solveMazeDFS(struct Maze* maze, int noSolution) {
    struct Maze *ans = copyMaze(maze); // Answer will be a copy of the maze, as to nt change it's original form
    char **matrix = ans->matrix;
    struct TupleStack *toExplore = TupleStack(maze->columns * maze->rows);
    struct Tuple *weAt = Tuple(maze->start.i, maze->start.j);
    push(toExplore, weAt);
    int foundAns = 0;
    while (!foundAns && toExplore->size != 0) {
        struct Tuple analyzing = pop(toExplore);
        int i1 = analyzing.i;
        int j1 = analyzing.j;
        if (matrix[i1][j1] == 'f') {
            foundAns = 1; // We're done bud
        } else {
            matrix[i1][j1] = 'x';
            if (j1 < maze->columns-1 && matrix[i1][j1 + 1] == ' ')  // right
                push(toExplore, Tuple(i1, j1 + 1));
            if (i1 < maze->rows-1 && matrix[i1 + 1][j1] == ' ')  // down
                push(toExplore, Tuple(i1 + 1, j1));
            if (j1 > 0 && matrix[i1][j1 - 1] == ' ')  // left
                push(toExplore, Tuple(i1, j1 - 1));
            if (i1 > 0 && matrix[i1 - 1][j1] == ' ')  // up
                push(toExplore, Tuple(i1 - 1, j1));
        }
    }
    if (!foundAns) {
        noSolution = -1;
    }
    return ans;
}

int main() {
    /* struct TupleStack* stack = TupleStack(20);
    push(stack, Tuple(0,0));
    push(stack, Tuple(1,1));
    push(stack, Tuple(2,2));
    printf("%d\n",pop(stack));
    printf("%d\n",pop(stack));
    printf("%d\n",pop(stack)); */


    char* input = "s  ##  #\n"
                  "#     # \n"
                  "#####  f\0";
    struct Maze* maze = Maze(input);
    printf("columns: %d\n", maze->columns);
    printf("rows: %d\n", maze->rows);
    printf("start = %d, %d\n", maze->start.i, maze->start.j);
    printf("finish = %d, %d\n", maze->finish.i, maze->finish.j);
    char* totring = toString(*maze);
    printf("---Unsolved Maze---\n%s\n", totring);
    int isThereAns = 0;
    struct Maze * solvedMaze = solveMazeDFS(maze, isThereAns);
    if (isThereAns == 0) {
        printf("---Solved Maze---\n%s\n",toString(*solvedMaze));
    } else {
        printf("Oh no! This maze has no solution hon...");
    }


}
