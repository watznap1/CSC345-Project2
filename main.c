#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int puzzle[9][9]; //suduku puzzle gets read into this array

typedef struct {
    int row;
    int column;
} parameters;

/*1 threads checking all rows and columns + 9 threads each checking a subgrid
row checker writes to val[0], colunm to val[1] and subgrid to rest
if array is all 1, puzzle is valid, if any zeroes then a validate function has failed to validate and puzzle is invalid*/
int valid1[11] = {0};

//for 27 thread implementation (option 2)
int valid2[27] = {0};

//1 thread checks all rows
void* validate_all_rows(void* param) {
    for (int row = 0; row < 9; row ++) {
        int marked[10] = {0};    /*/rray marking seen values*/
        for (int column = 0; column < 9; column++) {
            int value = puzzle[row][column];

            if (marked[value] == 1) {
                pthread_exit(NULL); /* signifies value was already seen in row, so puzzle is invalid*/
            }

            marked[value] = 1;    /*mark array index corresponding to seen value to signify already seen in row*/
        }
    }

    valid1[0] = 1;  /*mark rows as valid*/
    pthread_exit(NULL);
}

/*1 thread checks all columns - same code as validate_all_row except columns and rows reversed in for loops*/
void* validate_all_columns(void* param) {
    for (int column = 0; column < 9; column ++) {
        int marked[10] = {0};   
        for (int row = 0; row < 9; row++) {
            int value = puzzle[row][column];

            if (marked[value] == 1) {
                pthread_exit(NULL);
            }

            marked[value] = 1;  
        }
    }

    valid1[1] = 1;  
    pthread_exit(NULL);

}




//each thread checks only 1 row
void* validate_single_row(void* param) {


    pthread_exit(NULL);
}

//each thread checks only 1 column
void* validate_single_column(void* param) {


    pthread_exit(NULL);
}






void* validateSub(void* param) {


    pthread_exit(NULL);
}

int main(int argc, char** argv) {


    return 0;
}