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
};

struct Tuple* buildTuple(int i, int j){
    struct Tuple* ans = malloc(sizeof(struct Tuple));
    ans->i = i;
    ans->j = j;
    return ans;
}

char *tupleToString(struct Tuple * tuple) {
    if(tuple == NULL)
        return "NULL";
    char str[15]; // Create an array to store the string
    // Use sprintf to format the string and store it in the array
    sprintf(str, "(%d, %d)", tuple->i, tuple->j);
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
    struct Tuple **list;
    int size;
};

struct TupleQueue *buildTupleQueue(int maxSize) {
    struct TupleQueue *ans = malloc(sizeof(struct TupleQueue));
    ans->list = malloc(maxSize * sizeof(struct Tuple *));
    ans->size = 0;
    return ans;
}

struct Tuple *dequeue(struct TupleQueue *tQ) {
    if (tQ->size == 0) {
        return NULL;
    }
    --tQ->size;
    struct Tuple *ans = tQ->list[0];
    for (int i = 1; i <= tQ->size; i++) {
        tQ->list[i-1] = tQ->list[i];
    }
    return ans;
}

void enqueue(struct TupleQueue *tQ, struct Tuple *tuple) {
    tQ->list[tQ->size] = tuple;
    ++tQ->size;
}
// END OF TUPLE Queue CLASS //

int arrayCorr (struct Tuple tuple, struct Maze maze){
    return tuple.i * maze.columns + tuple.j;
}

struct Tuple ** neighbours (struct Maze maze, struct Tuple tuple, int * numNeighbours ) {
    *numNeighbours = 0;
    if (tuple.j < maze.columns - 1 && (maze.matrix[tuple.i][tuple.j+1] == ' ' || maze.matrix[tuple.i][tuple.j+1] == 'f' )) ++(*numNeighbours); // right
    if (tuple.i < maze.rows - 1 && (maze.matrix[tuple.i + 1][tuple.j] == ' ' || maze.matrix[tuple.i + 1][tuple.j] == 'f' )) ++(*numNeighbours); // down
    if (tuple.j > 0 && (maze.matrix[tuple.i][tuple.j - 1] == ' ' || maze.matrix[tuple.i][tuple.j - 1] == 'f')) ++(*numNeighbours); // left
    if (tuple.i > 0 && (maze.matrix[tuple.i - 1][tuple.j] == ' ' || maze.matrix[tuple.i - 1][tuple.j] == 'f')) ++(*numNeighbours); // up
    struct Tuple **ans = malloc(*numNeighbours * sizeof(struct Tuple*)); // Create an array that can contain every neighbour

    if (*numNeighbours != 0) { // We only do this if there is actually any neighbours to return
        int index = 0;
        if (tuple.j < maze.columns - 1 && (maze.matrix[tuple.i][tuple.j+1] == ' ' || maze.matrix[tuple.i][tuple.j+1] == 'f' )) { // right
            ans[index++] = buildTuple(tuple.i, tuple.j + 1);
        }
        if (tuple.i < maze.rows - 1 && (maze.matrix[tuple.i + 1][tuple.j] == ' ' || maze.matrix[tuple.i + 1][tuple.j] == 'f' )) { // down
            ans[index++] = buildTuple(tuple.i + 1, tuple.j);
        }
        if (tuple.j > 0 && (maze.matrix[tuple.i][tuple.j - 1] == ' ' || maze.matrix[tuple.i][tuple.j - 1] == 'f')) { // left
            ans[index++] = buildTuple(tuple.i, tuple.j - 1);
        }
        if (tuple.i > 0 && (maze.matrix[tuple.i - 1][tuple.j] == ' ' || maze.matrix[tuple.i - 1][tuple.j] == 'f')) { // up
            ans[index++] = buildTuple(tuple.i - 1, tuple.j);
        }
    }
     return ans;
}

// Solve maze //

struct Tuple** solveMazeDFS (struct Maze* maze, int noSolution) {
    struct TupleQueue * toVisit = buildTupleQueue((maze->columns) * (maze->rows)); // Cells to visit

    enqueue(toVisit, &maze->start);




    int arrSize = (maze->columns) * (maze->rows);
    int visited [arrSize]; // Array, have we visited the cell?
    for (int i = 0; i < arrSize; i++)  // We have not visited any cells
        visited[i] = 0;
    visited[arrayCorr(maze->start, *maze)] = 1; // We have visited starter node
    /*for (int i = 0; i < arrSize; i++)
        printf("%d, ", visited [i]);

    printf("\n");*/

    struct Tuple * tTuple [arrSize];
    struct Tuple ** prevTuple = malloc(sizeof(tTuple));
    for (int i = 0; i < arrSize; i++) {
        prevTuple[i] = NULL;
    }
    /*printf("prev Tuple = [");
    for (int i = 0; i < arrSize; i++) {
        printf("%s, ", tupleToString(prevTuple[i]));
    }
    printf("]\n");*/
    //int iteration = 0;

    while(toVisit->size > 0) { // While we still have cells to visit
        //printf("---- iteration %d ----\n", iteration);

        struct Tuple * visiting = dequeue(toVisit);

       // printf("Visiting: %s\n", tupleToString(visiting));

        // For each neighbour, if it hasn't been visited, enqueue it, mark it as visited, and save its parent node in it's correspondant slot in the prev array
        int numNeighbours = 0;
        // Neighbour array
        struct Tuple ** arrNeigh = neighbours(*maze, *visiting, &numNeighbours);
       /* for (int i = 0; i < numNeighbours; i++) {
            printf("Neighbour %d: %s", i+1 ,tupleToString(arrNeigh[i]));
            printf("\n");
        }*/

        for (int i = 0; i < numNeighbours; i++){ // for each neighbour
            struct Tuple * neighbour = arrNeigh[i];
            int arC = 0;
            arC = arrayCorr(*neighbour, *maze);
            //printf("Neighbour %d saved in prevTuple[%d] \n", i+1, arC);
            if(!visited[arC]) {
                enqueue(toVisit,neighbour); // enqueue it
                visited[arC] = 1; // mark it as visited
                prevTuple[arC] = visiting; // save its parent node
            }
        }

        //printf("Queue.size= %d \n", toVisit->size);


/*
        for (int i = 0; i < arrSize; i++)
            printf("%d, ", visited [i]);
*/
       // iteration++;
    }
   /* printf("arrSize = %d\n", arrSize);
    printf("prev Tuple = [");
    for (int i = 0; i < arrSize; i++) {
        printf("%s, ", tupleToString(prevTuple[i]));
    }
    printf("]\n"); */
    return prevTuple;
} // end of solveMazeDFS

struct Tuple **path(struct Maze *maze, struct Tuple **search, int *pathSize) {
    struct Tuple **ans = malloc(maze->columns * maze->rows * sizeof(struct Tuple *));
    *pathSize = 0;
    struct Tuple *t = &maze->finish;
    while (t->i != maze->start.i || t->j != maze->start.j) {
        ans[*pathSize] = t;

        int arC = arrayCorr(*t, *maze);
        //printf(tupleToString(t));
        t = search[arC];
        ++*pathSize;
    }
    ans[*pathSize] = &maze->start;
    ++*pathSize;
    return ans;
}


int main() {
    char* input = " ######s###\n"
                  "  # #     #\n"
                  "# # # #####\n"
                  "#         #\n"
                  "# ####### #\n"
                  "# #     ## \n"
                  "# # # # ###\n"
                  "#     # # #\n"
                  "### # ### #\n"
                  "#          \n"
                  "####### # f";
    struct Maze* maze = Maze(input);
    printf ("s = %s\n", tupleToString(&maze->start));
    printf ("f = %s\n", tupleToString(&maze->finish));
/*
    printf("start = %s ", tupleToString(&maze->start));

    int numN = 0;
    struct Tuple ** n = neighbours(*maze, *buildTuple(0,10), &numN);
    printf("Number of neighbours = %d \n", numN);


    for (int i = 0; i < numN; i++) {
        printf("Neighbour %d: %s", i+1 ,tupleToString(n[i]));
        printf("\n");
    }

*/
    //printf("Neighbour 2: (%d, %d), code = %d \n" , n[1]->i, n[1]->j,arrayCorr(*n[1],*maze));

    struct Tuple ** search = solveMazeDFS(maze, 0);
    //printf("prevTuple = [");
    /*for(int i = 0; i < 121; i++)
        printf("%s, ", tupleToString(search[i]));
    printf("] \n");*/



    //printf (tupleToString(search[arrayCorr(maze->finish, *maze)]));
    int pS = 0;
    struct Tuple ** p = path(maze, search, &pS);
    printf("path = [");
    for(int i = 0; i < pS; i++)
        printf("%s, ", tupleToString(p[i]));
    printf("] \n");



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
