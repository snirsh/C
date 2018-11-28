#include <stdlib.h>
#include "string.h"
#include "SudokuTree.h"

#define WRONG_INPUT_ARGS_ERR "Please supply a file!"
#define ERR_READING_FILE "Error reading from file"
#define STR_TO_LONG_ERR "No numbers to convert. please insert a valid input file."
#define BAD_FILE_FORMAT "Bad file format"
#define TOO_MANY_INPUTS_INLINE_ERR "Too many inputs in line"
#define NOT_ENOUGH_INPUT_ERR "Not enough rows in input file"
#define READ_ONLY "r"
#define WHITE_SPACE " "
#define SPACE_SEP ' '
#define BUFFER_SIZE 400
#define ARG_FILE_POS 1
#define DECIMAL 10
#define NEW_LINE "\n"
#define WRITE_MODE "w"

/**
 * helper that will turn a given string to int and check if it's ok
 * @param stringToConvert
 * @return
 */
unsigned int stringToInt(char *stringToConvert)
{
    char *remainder;
    unsigned int temp = (unsigned int) strtol(stringToConvert, &remainder, DECIMAL);
    if(stringToConvert == remainder)
    {
        fprintf(stderr, STR_TO_LONG_ERR);
        exit(EXIT_FAILURE);
    }
    return temp;
}

/**
 * this is a helper function that reads an individual row
 * @param line
 * @param dimSize
 * @param board
 * @param currentRow
 */
void readRow(char *line, int dimSize, unsigned int **board, int currentRow)
{
    char *p = strtok(line, WHITE_SPACE);
    int pos = 0;
    while(p)
    {
        if(pos > dimSize)
        {
            fprintf(stderr, TOO_MANY_INPUTS_INLINE_ERR);
            exit(EXIT_FAILURE);
        }
        unsigned int temp = stringToInt(p);
        if(temp < 0 || temp > dimSize)
        {
            fprintf(stderr, BAD_FILE_FORMAT);
            exit(EXIT_FAILURE);
        }
        board[currentRow][pos] = temp;
        p = strtok(NULL, " ");
        pos++;
    }
}

/**
 * main function of this file, will read the whole file and create a sudoku from it
 * @param fileSrc
 * @return
 */
pNode readFile(char *fileSrc)
{
    FILE *file = fopen(fileSrc, READ_ONLY);
    if(file == NULL)
    {
        fprintf(stderr, ERR_READING_FILE);
        exit(EXIT_FAILURE);
    }
    int dimension = 0;
    char dimString[BUFFER_SIZE];
    if(fgets(dimString, BUFFER_SIZE, file) != NULL)
    {
        dimension = stringToInt(dimString);
    }
    unsigned int **board;
    board = (unsigned int **) malloc(sizeof(unsigned int *) * dimension);
    if(board == NULL)
    {
        fprintf(stderr, NOT_ENOUGH_MEM);
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < dimension; ++i)
    {
        char line[BUFFER_SIZE];
        if((fgets(line, BUFFER_SIZE, file))!=NULL){
            board[i] = malloc(sizeof(unsigned int) * dimension);
            if(board[i] == NULL)
            {
                fprintf(stderr, NOT_ENOUGH_MEM);
                exit(EXIT_FAILURE);
            }
            readRow(line, dimension, board, i);
        }
        else
        {
            fprintf(stderr, NOT_ENOUGH_INPUT_ERR);
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);
    return newBoard(dimension, board);
}

/**
 * this function prints the sudoku onto screen
 * @param b
 * @param dim
 */
void printSoduko(pNode b, int dim)
{
    FILE *file = stdout;
    int dimension = dim;
    char dimStr[dim];
    sprintf(dimStr, "%d", dim);
    fprintf(file, "%s", dimStr);
    fprintf(file, "%s", "\n");
    for(int i = 0; i < dim; i++)
    {
        char *sep = " ";
        for(int j = 0; j < dim; j++)
        {
            char currentCell[2];
            fprintf(file, "%d", getCell(b, i, j));
            if(j == dimension - 1)
            {
                sep = "\n";
            }
            fprintf(file, "%s", sep);
        }
    }
    fclose(file);

}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        fprintf(stderr, WRONG_INPUT_ARGS_ERR);
        exit(EXIT_FAILURE);
    }
    pNode *sudoku = readFile(argv[ARG_FILE_POS]);
    pNode solution = getBest(sudoku, getPossibilities, getVal, freeSudoku, copyBoard,
                             ((unsigned int) SQUARED(((board *) sudoku)->dim)));
    printSoduko(solution, ((board *) sudoku)->dim);
}