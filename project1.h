#ifndef PROJECT1_H
#define PROJECT1_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>

//32768
#define ROWS_COUNT 32768
#define COLUMN_COUNT 32768

/* given a matrix, with pre-define number of rows (ROWS_COUNT)
 * and number of columns (COLUMN_COUNT), print_matrix function prints
 * all elements of the matrix.
*/
void print_matrix(int *matrix){

    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            printf("%d\t",*(matrix+i*COLUMN_COUNT+j));
        }
        printf("\n");
    }
}

/* Given a matrix, index of start_row and end_row, the function
 * sum_matrix returns sum of all elements of rows starting from (and including)
 * start_row to end_row (including end_row))
*/
long sum_matrix(int start_row, int end_row, int *matrix)
{
    long sum=0;

    /* ToDo1:
     * write the code for summing all elements from start_row to end_row
    */
    for(int r = start_row; r<=end_row; r++){
        for(int c = 0; c<COLUMN_COUNT; c++){
            sum += matrix[r * COLUMN_COUNT + c];
        }
    }

    return sum;
}


#endif // PROJECT1_H
