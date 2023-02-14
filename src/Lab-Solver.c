// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 #include <string.h>

// TUPLE CLASS //
struct Tuple {
    int i;
    int j;
    float distanceToF;
    struct Tuple * prevTuple;
};

struct Tuple* buildTuple(int i, int j){
    struct Tuple* ans = malloc(sizeof(struct Tuple));
    ans->i = i;
    ans->j = j;
    ans->distanceToF = -1;
    return ans;
}

void distance (struct Tuple* tuple, int distanceToF) {
    tuple->distanceToF = distanceToF;
};

char *tupleToString(struct Tuple tuple) {
    char str[8]; // Create an array to store the string
    // Use sprintf to format the string and store it in the array
    sprintf(str, "(%d, %d)", tuple.i, tuple.j);
    // Allocate memory for the result string and copy the contents of the array
    char *result = malloc(strlen(str) + 1);
    strcpy(result, str);
    return result;
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
                m->start = *buildTuple(i,j);
            }
            if(*aux == 'f') {
                f = 1;
                m->finish = *buildTuple(i,j);
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
char* mazeToString(struct Maze maze){
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
    ans->start  = *buildTuple(maze->start.i, maze->start.j);
    ans->finish = *buildTuple(maze->finish.i, maze->finish.j);
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

// TUPLE Queue CLASS // -> Checked
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

int arrayCorr (struct Tuple tuple, struct Maze maze){
    return tuple.i * maze.columns + tuple.j;
}

struct Tuple ** neighbours (struct Maze maze, struct Tuple tuple, int * numNeighbours ) {
    *numNeighbours = 0;
    if (tuple.j < maze.columns - 1 && maze.matrix[tuple.i][tuple.j+1] == ' ') ++(*numNeighbours); // right
    if (tuple.i < maze.rows - 1 && maze.matrix[tuple.i + 1][tuple.j] == ' ') ++(*numNeighbours); // down
    if (tuple.j > 0 && maze.matrix[tuple.i][tuple.j - 1] == ' ') ++(*numNeighbours); // left
    if (tuple.i > 0 && maze.matrix[tuple.i - 1][tuple.j] == ' ') ++(*numNeighbours); // up
    struct Tuple **ans = malloc(*numNeighbours * sizeof(struct Tuple*)); // Create an array that can contain every neighbour

    if (*numNeighbours != 0) { // We only do this if there is actually any neighbours to return
        int index = 0;
        if (tuple.j < maze.columns - 1 && maze.matrix[tuple.i][tuple.j+1] == ' ') { // right
            ans[index++] = buildTuple(tuple.i, tuple.j + 1);
        }
        if (tuple.i < maze.rows - 1 && maze.matrix[tuple.i + 1][tuple.j] == ' ') { // down
            ans[index++] = buildTuple(tuple.i + 1, tuple.j);
        }
        if (tuple.j > 0 && maze.matrix[tuple.i][tuple.j - 1] == ' ') { // left
            ans[index++] = buildTuple(tuple.i, tuple.j - 1);
        }
        if (tuple.i > 0 && maze.matrix[tuple.i - 1][tuple.j] == ' ') { // up
            ans[index++] = buildTuple(tuple.i - 1, tuple.j);
        }
    }
     return ans;
}

// Solve maze //

struct Tuple** solveMazeDFS (struct Maze* maze, int noSolution) {
    struct TupleQueue * toVisit = TupleQueue((maze->columns) * (maze->rows)); // Cells to visit
    enqueue(toVisit, &maze->start);
    int arrSize = (maze->columns) * (maze->rows);
    int visited [arrSize]; // Array, have we visited the cell?
    for (int i = 0; i < arrSize; i++)  // We have not visited any cells
        visited[i] = 0;
    visited[arrayCorr(maze->start, *maze)] = 1; // We have visited starter node
    for (int i = 0; i < arrSize; i++)
        printf("%d, ", visited [i]);

    printf("\n");



    /*
    struct Tuple * prevTuple [arrSize];
    while(toVisit.size != 0) { // While we still have cells to visit
        struct Tuple visiting = dequeue(&toVisit);
        // For each neighbour, if it hasn't been visited, enqueue it, mark it as visited, and save it's parent node in it's correspondant slot in the prev array
        int n = 0;
        int * numNeighbours = &n;
        struct Tuple ** arrNeigh = neighbours(*maze, visiting, numNeighbours);
        for (int i = 0; i < *numNeighbours; i++){
            struct Tuple * neighbour = arrNeigh[i];
            int arC = 0;
            arC = arrayCorr(*neighbour, *maze);
            if(!visited[arC]) {
                enqueue(&toVisit,neighbour); // enqueue it
                visited[arC] = 1; // mark it as visited
                *prevTuple[arC] = visiting; // save its parent node
            }
        }
    }
    for (int i = 0; i < arrSize; i++) {
        printf("%d",* visited[i]);
    } printf("\n");
    return prevTuple; */
}

int main() {

    char* input = "###########\n"
                  "  # #     #\n"
                  "# # # #####\n"
                  "#         #\n"
                  "# ####### #\n"
                  "# #     # #\n"
                  "# ### # ###\n"
                  "#     # # #\n"
                  "##### ### #\n"
                  "#          \n"
                  "#########s#";
    struct Maze* maze = Maze(input);

    int numN = 0;
    struct Tuple ** n = neighbours(*maze, maze->start, &numN);
    printf("Number of neighbours = %d \n", numN);
    printf("Neighbour 1: (%d, %d), code = %d \n" , n[0]->i, n[0]->j, arrayCorr(*n[0],*maze));
    printf("start = %s ", tupleToString(maze->start));
    //printf("Neighbour 2: (%d, %d), code = %d \n" , n[1]->i, n[1]->j,arrayCorr(*n[1],*maze));

    struct Tuple ** path = solveMazeDFS(maze, 0);
    //struct Tuple t = *path[0];
   // printf("(%d, %d), ", t.i, t.j);



  /*  printf("%f", distanceToF(*maze, &(maze->start)));

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

*/
}
