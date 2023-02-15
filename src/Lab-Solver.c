// Created by Guillermo (William) Rubio Bolger on 6/2/23.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* TUPLE CLASS */
struct Tuple { // To store coordinates in the maze
    int i;
    int j;
};

struct Tuple* buildTuple(int i, int j){ // Class constructor
    struct Tuple* ans = malloc(sizeof(struct Tuple));
    ans->i = i;
    ans->j = j;
    return ans;
}

char *tupleToString(struct Tuple * tuple) { // Visualize Tuple instances
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
/* END OF TUPLE CLASS*/

/* MAZE CLASS */
// To represent the maze of chars
struct Maze {
    int rows;
    int columns;
    char** matrix; // Where the maze data actually is
    struct Tuple start;
    struct Tuple finish;
};
// Maze constructor, a function that returns a pointer to a new maze
struct Maze* buildMaze(char* maze_string){
    // Local counters, variables and pointers
    char* aux = maze_string; // Auxiliary pointer for iteration
    int lRows = 0;
    int lColumns = 0;
    int doneColumns = 0; // Boolean
    // Count columns and rows
    while(*aux) {
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
    // Assignment of start Tuple, finish Tuple and matrix
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
    (*m).matrix = matrix; // Now in the class
    return m; // pointer
}
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
// Maze copier, useful for representing solved mazes
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
/* END OF MAZE CLASS */

/* TUPLE Queue CLASS */
// LIFO data type to store Tuples
struct TupleQueue {
    struct Tuple **list;
    int size;
};
// TupleQueue constructor
struct TupleQueue *buildTupleQueue(int maxSize) {
    struct TupleQueue *ans = malloc(sizeof(struct TupleQueue));
    ans->list = malloc(maxSize * sizeof(struct Tuple *));
    ans->size = 0;
    return ans;
}
// dequeue
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
// enqueue
void enqueue(struct TupleQueue *tQ, struct Tuple *tuple) {
    tQ->list[tQ->size] = tuple;
    ++tQ->size;
}
/* END OF TUPLE Queue CLASS */

/* "arrayCorr" is a function that returns the relative address where a Tuple should be placed in an array.
 * This address should be the same for any Tuple with the same values of its attributes.
 * We use this function for the boolean array visited and the parent Tuple array prevTuple */
int arrayCorr (struct Tuple tuple, struct Maze maze){
    return tuple.i * maze.columns + tuple.j;
}
/* "neighbours" is a function that returns an array of Tuple pointers to the neighbours (not walls) of a tuple (coordinate) in
 * a certain maze, this is useful for the BFS search of the maze*/
struct Tuple ** neighbours (struct Maze maze, struct Tuple tuple, int * numNeighbours ) {
    // First, let's count the number of neighbours
    *numNeighbours = 0;
    if (tuple.j < maze.columns - 1 && (maze.matrix[tuple.i][tuple.j+1] == ' ' || maze.matrix[tuple.i][tuple.j+1] == 'f' )) ++(*numNeighbours); // right
    if (tuple.i < maze.rows - 1 && (maze.matrix[tuple.i + 1][tuple.j] == ' ' || maze.matrix[tuple.i + 1][tuple.j] == 'f' )) ++(*numNeighbours); // down
    if (tuple.j > 0 && (maze.matrix[tuple.i][tuple.j - 1] == ' ' || maze.matrix[tuple.i][tuple.j - 1] == 'f')) ++(*numNeighbours); // left
    if (tuple.i > 0 && (maze.matrix[tuple.i - 1][tuple.j] == ' ' || maze.matrix[tuple.i - 1][tuple.j] == 'f')) ++(*numNeighbours); // up
    struct Tuple **ans = malloc(*numNeighbours * sizeof(struct Tuple*)); // Create an array that can contain every neighbour
    // Then, let's build that array
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

/* Solve maze */
/* "searchMazeDFS" searches the Maze in a Breadth First Search fashion, and returns a Tuple pointer array with the
 * parent array of each blank coordinate of the maze in its corresponding spot, given by "arrayCorr"*/
struct Tuple** searchMazeBFS (struct Maze* maze) {
    struct TupleQueue * toVisit = buildTupleQueue((maze->columns) * (maze->rows)); // Cells to visit
    enqueue(toVisit, &maze->start);
    int arrSize = (maze->columns) * (maze->rows);
    int visited [arrSize]; // Array, have we visited the cell?
    for (int i = 0; i < arrSize; i++)  // We have not visited any cells
        visited[i] = 0;
    visited[arrayCorr(maze->start, *maze)] = 1; // We have visited starter node
    struct Tuple ** prevTuple = malloc(maze->columns * maze->rows * sizeof(struct Tuple*)); // Where we store the parent coordinate of a Tuple
    for (int i = 0; i < arrSize; i++)  // We mark everything to NULL because most Tuples of a maze won't have a parent coordinate
        prevTuple[i] = NULL;
    while(toVisit->size > 0) { // While we still have cells to visit
        struct Tuple * visiting = dequeue(toVisit); // We save the Tuple that we have to visit
        // For each neighbour, if it hasn't been visited, we enqueue it, mark it as visited, and save its parent node in it's corresponding slot in the prev array
        int numNeighbours = 0;
        // Neighbour array
        struct Tuple ** arrNeigh = neighbours(*maze, *visiting, &numNeighbours);
        for (int i = 0; i < numNeighbours; i++){ // for each neighbour
            struct Tuple * neighbour = arrNeigh[i];
            int arC = 0;
            arC = arrayCorr(*neighbour, *maze);
            if(!visited[arC]) {
                enqueue(toVisit,neighbour); // enqueue it
                visited[arC] = 1; // mark it as visited
                prevTuple[arC] = visiting; // save its parent node
            }
        }
    }
    return prevTuple;
} // end of searchMazeDFS

/* "path" is a function that returns a Tuple pointer array that represents the shortest path from f to
 * s, yes, it's backwards, backtracking from the finish Tuple to the start Tuple with the use of the
 * parent array given by the "searchMazeDFS"*/
struct Tuple **path(struct Maze *maze, struct Tuple **search, int *pathSize) {
    struct Tuple **ans = malloc(maze->columns * maze->rows * sizeof(struct Tuple *)); // Memory allocation of the answer
    *pathSize = 0;
    struct Tuple *t = &maze->finish; // Finish Tuple of maze
    while (t != NULL) { // We do this until there is no parent Tuple
        ans[*pathSize] = t; // Tuple t is now t's parent
        int arC = arrayCorr(*t, *maze);
        t = search[arC]; // We save t in its spot
        ++*pathSize; // Next spot of the ans array
    }
    //ans[*pathSize] = &maze->start; // Lastly, we save the start Tuple from maze in ans
    //++*pathSize;

    return ans;
} // end of "path"

/* "solveMaze" is a function that returns a Maze pointer to the solved maze, changing the chars of the
 *  matrix representing the shortest path to 'x'. It utilizes both the search algorithm "searchMazeBFS"
 *  and the backtracking algorithm "path"*/
struct Maze * solveMaze (struct Maze maze, int * isThereAns){
    struct Maze * ans = copyMaze(&maze);
    struct Tuple ** search = searchMazeBFS(&maze); // BFS search
    int pS = 0;
    struct Tuple ** p = path(&maze, search, &pS); // path
    for(int k = 0; k < pS; k++){ // Visual representation
        struct Tuple changing = *p[k];
        int i = changing.i;
        int j = changing.j;
        if (ans->matrix[changing.i][changing.j] == ' ')
            ans->matrix[changing.i][changing.j] = 'x';
    }
    if (p[pS-1]->i == ans->start.i && p[pS-1]->j == ans->start.j) { // If we cannot backtrack until the start, that means the maze has no path from s to f
        *isThereAns = 1;
    } else {
        *isThereAns = 0;
    }
    return ans;
}

int main() {
    char* input1 = " ######s###\n"
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

    char* input = "s ###########################################################\n"
                  "      #       #       #   #           # #     # # #         #\n"
                  "##### # ##### # ####### # ### ### ### # # ##### # # ##### # #\n"
                  "# #   #     #     #     #   # #   #   #     # #   #     # # #\n"
                  "# # ####### ### ########### # ### # ####### # # # ##### ### #\n"
                  "# #       # #     # #             #   #   #     #   #     # #\n"
                  "# # # ### # ### ### ### # ##### ##### ### ### ##### # ##### #\n"
                  "#   #   #         #     #     # #   #         # # #       # #\n"
                  "# ##### # ### # ### ### ### ##### ##### ####### # ### ##### #\n"
                  "# #   # # #         #       # #     #                 #   # #\n"
                  "########################################################### f";
    struct Maze* maze = buildMaze(input);
    printf("--INPUT MAZE-- \n %s \n", mazeToString(*maze));
    printf ("s = %s\n", tupleToString(&maze->start));
    printf ("f = %s\n", tupleToString(&maze->finish));

    struct Tuple ** search = searchMazeBFS(maze);


    int isThereAns = 0;

    printf("--OUTPUT MAZE-- \n%s\n", mazeToString(*solveMaze(*maze, &isThereAns)));
    printf("ans? = %d", isThereAns);


}
