#include "SudokuTree.h"

#define NOT_ENOUGH_MEM "Not enough memory for malloc"

#define NULL_PTR "Null pointer, cant return value"


void checkMem(unsigned int **pInt)
{
    if(pInt == NULL)
    {
        fprintf(stderr, NOT_ENOUGH_MEM);
        exit(EXIT_FAILURE);
    }
}


pNode *newBoard(int toInitDim, unsigned int **toInitBoard)
{
    pNode *newBoard = malloc(sizeof(board));
    if(newBoard == NULL)
    {
        fprintf(stderr, NOT_ENOUGH_MEM);
        exit(EXIT_FAILURE);
    }
    ((board *) newBoard)->dim = toInitDim;
    ((board *) newBoard)->numOfChildren = 0;
    //creating the children array
    ((board *) newBoard)->children;
    ((board *) newBoard)->table = malloc(sizeof(int*) * toInitDim);
    //creating the board array
    for(int i = 0; i < toInitDim; i++)
    {
        ((board *) newBoard)->table[i] = malloc(sizeof(int) * toInitDim);
        for(int j = 0; j < toInitDim; j++)
        {
            ((board *) newBoard)->table[i][j] = toInitBoard[i][j];
            if(toInitBoard[i][j] != 0)
            {
                ((board *) newBoard)->solvedCells++;
            }
        }
    }
    return newBoard;
}

void freeSudoku(pNode toFree)
{
    if(toFree == NULL)
    {
        return;
    }
    board *toDel = (board *) toFree;
    for(int i = 0; i < toDel->dim; ++i)
    {
        free((*toDel).table[i]);
    }
    free((*toDel).table);
    for(int j = 0; j < toDel->numOfChildren; ++j)
    {
        freeSudoku((*toDel).children[j]);
    }
    free((*toDel).children);
    free(toDel);
}


unsigned int getVal(pNode p)
{
    if(p == NULL)
    {
        return 0;
    }
    return ((board *) p)->solvedCells;
}


int getDim(pNode p)
{
    if(p == NULL)
    {
        return 0;
    }
    return ((board *) p)->dim;
}

unsigned int *getRow(pNode p, int rowNum)
{

    unsigned int *newRow = malloc(sizeof(int) * getDim(p));
    unsigned int *currRow = ((board *) p)->table[rowNum];
    for(int i = 0; i < getDim(p); i++)
    {
        newRow[i] = currRow[i];
    }
    return newRow;
}

unsigned int *getCol(pNode p, int colNum)
{
    unsigned int *newCol = malloc(sizeof(unsigned int) * ((board *) p)->dim);
    for(int i = 0; i < getDim(p); i++)
    {
        newCol[i] = ((board *) p)->table[i][colNum];
    }
    return newCol;
}

unsigned int getCell(pNode p, int rowNum, int colNum)
{
    return ((board *) p)->table[rowNum][colNum];
}

unsigned int *getCube(pNode p, int rowNum, int colNum)
{
    int dim = ((board *) p)->dim;
    int cubeSide = (int) sqrt(dim);
    int rowStart = rowNum - (rowNum % cubeSide);
    int rowEnd = rowStart + cubeSide;
    int colStart = colNum - (colNum % cubeSide);
    int colEnd = colStart + cubeSide;
    int pos = 0;
    unsigned int *cube = malloc(sizeof(unsigned int) * dim);
    for(; rowStart < rowEnd; rowStart++)
    {
        for(colStart; colStart < colEnd; colStart++)
        {
            cube[pos] = ((board *) p)->table[rowStart][colStart];
            pos++;
        }
    }
    return cube;
}

pNode copyBoard(pNode nodeToCopy)
{
    board *topCopy = (board *) nodeToCopy;
    board *copied = (board *) malloc(sizeof(topCopy));
    copied->dim = topCopy->dim;
    copied->numOfChildren = topCopy->numOfChildren;
    copied->solvedCells = topCopy->solvedCells;
    copied->table = (unsigned int **) malloc(sizeof(unsigned int *) * copied->dim);
    copied->children = (pNode **) malloc(sizeof(board) * copied->numOfChildren);
    for(int i = 0; i < copied->dim; i++)
    {
        copied->table[i] = (unsigned int *) malloc(sizeof(unsigned int) * copied->dim);
        for(int j = 0; j < copied->dim; j++)
        {
            copied->table[i][j] = topCopy->table[i][j];
        }
    }
    for(int k = 0; k < copied->numOfChildren; k++)
    {
        copied->children[k] = topCopy->children[k];
    }
    return (pNode *) copied;
}

void findFirstUnsolved(pNode p, int *row, int *col)
{
    for(int r = 0; r < ((board *) p)->dim; r++)
    {
        for(int c = 0; c < ((board *) p)->dim; c++)
        {
            if(((board *) p)->table[r][c] == 0)
            {
                (*row) = r;
                (*col) = c;
                return;
            }
        }
    }
}

int checkValidNumbers(pNode p, int rowNum, int colNum, int toCheck)
{
    unsigned int *currRow = getRow(p, rowNum);
    unsigned int *currCol = getCol(p, colNum);
    unsigned int *cube = getCube(p, rowNum, colNum);
    int isAnswer = TRUE;
    for(int j = 0; j < getDim(p); j++)
    {
        if(currRow[j] == toCheck || currCol[j] == toCheck || cube[j] == toCheck)
        {
            isAnswer = FALSE;
            break;
        }
    }
    return isAnswer;
}


int getPossibilities(pNode sudoku, pNode **possibilities)
{
    int coords[2] = {0};
    findFirstUnsolved(sudoku, &coords[0], &coords[1]);
    unsigned int ans[getDim(sudoku)];
    int numOfPossibilities = 0;
    for(int i = 1; i <= getDim(sudoku); i++)
    {
        if(checkValidNumbers(sudoku, coords[0], coords[1], i))
        {
            ans[numOfPossibilities] = (unsigned int) i;
            numOfPossibilities++;
        }
    }
    board **children = malloc(sizeof(sudoku) * numOfPossibilities);
    for(int i = 0; i < numOfPossibilities; i++)
    {
        children[i] = (board *) copyBoard(sudoku);
        children[i]->table[coords[0]][coords[1]] = ans[i];
        children[i]->solvedCells++;
    }
    *possibilities = (pNode *) children;
    return numOfPossibilities;
}