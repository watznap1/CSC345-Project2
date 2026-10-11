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

            if (value < 1 || value > 9 || marked[value] == 1) {
                valid1[0] = 0;
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

            if (value < 1 || value > 9 || marked[value] == 1) {
                valid1[1] = 0;
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
    parameters* data = (parameters*)param;
    int startRow = (data->row / 3) * 3;
    int startCol = (data->column / 3) * 3;
    int marked[10] = {0};

    for (int row = startRow; row < startRow + 3; row++) {
        for (int column = startCol; column < startCol + 3; column++) {
            int value = puzzle[row][column];

            if (value < 1 || value > 9 || marked[value] == 1) {
                valid1[2 + (startRow / 3) * 3 + (startCol / 3)] = 0;
                pthread_exit(NULL);
            }
            marked[value] = 1;
        }
    }
    
    valid1[2 + (startRow / 3) * 3 + (startCol / 3)] = 1;
    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s input.txt\n", argv[0]);
        return 1;
    }

FILE *file = fopen(argv[1], "r");
if (file == NULL) {
    perror("Error opening input file");
    return 1;
}

for (int row = 0; row < 9; row++){
    for (int column = 0; column < 9; column++) {
        if (fscanf(file, "%d", &puzzle[row][column]) != 1) {
            printf("Error reading input file\n");
            fclose(file);
            return 1;
        }
    }
}
fclose(file);

pthread_t threads[11];
parameters data[11];

for (int i = 0; i < 11; i++) {
    if (i == 0) {
        pthread_create(&threads[i], NULL, validate_all_rows, NULL);
    } else if (i == 1){
        pthread_create(&threads[i], NULL, validate_all_columns, NULL);
    } else{
        data[i].row = ((i - 2) / 3) * 3;
        data[i].column = ((i - 2) % 3) * 3;
        pthread_create(&threads[i], NULL, validateSub, &data[i]);

    }
}
for (int i = 0; i < 11; i++){
    pthread_join(threads[i], NULL);
}
int valid =1;
for (int i = 0; i < 11; i++) {
    if (valid1[i] == 0) {
        valid = 0;
    }
}
if (valid) {
    printf("sudoku solution is valid.\n");
} else {
    printf("sudoku solution is invalid.\n");
}

return 0;
}
    