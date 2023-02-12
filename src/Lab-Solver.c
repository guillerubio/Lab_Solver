// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <string.h>

// TUPLE CLASS //
struct Tuple {
    int i;
    int j;
    float distanceToF;
    struct Tuple * prevTuple;
};

struct Tuple* Tuple(int i, int j){
    struct Tuple* ans = malloc(sizeof(struct Tuple));
    ans->i = i;
    ans->j = j;
    ans->distanceToF = -1;
    ans->prevTuple = NULL;
    return ans;
}

void path (struct Tuple* tuple, int distanceToF, struct Tuple * prevTuple) {
    tuple->distanceToF = distanceToF;
    tuple->prevTuple = prevTuple;
};

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

double distanceToF (struct Maze maze, struct Tuple* tuple) {
    double ans = 0;
    ans = sqrtf(pow(tuple->i - maze.finish.i,2) + pow(tuple->j - maze.finish.j,2));
    return ans;
}

// TUPLE Queue CLASS //
struct TupleQueue {
    struct Tuple* list;
    int size;
};

struct TupleQueue* TupleQueue(int maxSize){
    struct TupleQueue* ans = malloc(sizeof(TupleQueue));
    ans->list = malloc(maxSize * sizeof(struct Tuple));
    ans->size = 0;
    return ans;
}

struct Tuple dequeue (struct TupleQueue* tupleQueue){
    tupleQueue->size--;
    struct Tuple ans = tupleQueue->list[0];
    tupleQueue->list++;
    return ans;
}

void enqueue (struct TupleQueue* tupleQueue, struct Tuple* tuple) {
    tupleQueue->list[tupleQueue->size] = *tuple;
    tupleQueue->size++;

}
// END OF TUPLE Queue CLASS //

// Solve maze //

struct Maze* solveMazeDFS(struct Maze* maze, int noSolution) {
    struct Maze *ans = copyMaze(maze); // Copy of the maze
    char **matrix = ans->matrix; // Matrix of the answer maze
    struct TupleQueue *toVisit = TupleQueue(maze->columns * maze->rows); // Queue of coordinates to visit
    struct Tuple *weAt = Tuple(maze->start.i, maze->start.j); // Where are we
    enqueue(toVisit, weAt); // We need to visit the start
    int foundAns = 0; // Boolean: Are we done?
    struct Tuple fCoordinates; // finishTuple, will be the path to ans
    }

int main() {
     struct TupleQueue* queue = TupleQueue(20);
    enqueue(queue, Tuple(0,0));
    enqueue(queue, Tuple(1,1));
    enqueue(queue, Tuple(2,2));
    printf("%d\n",dequeue(queue));
    printf("%d\n",dequeue(queue));
    printf("%d\n",dequeue(queue));




    char* input = "s #########\n"
                  "  # #     #\n"
                  "# # # #####\n"
                  "#         #\n"
                  "# ####### #\n"
                  "# #     # #\n"
                  "# ### # ###\n"
                  "#     # # #\n"
                  "##### ### #\n"
                  "#          \n"
                  "######### f";
    struct Maze* maze = Maze(input);

    printf("%f", distanceToF(*maze, &(maze->start)));

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
