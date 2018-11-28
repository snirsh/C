//
// Created by Snir on 26/8/17.
//

#ifndef EX3_SUDOKUTREE_H
#define EX3_SUDOKUTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "GenericDFS.h"
#define NOT_ENOUGH_MEM "Not enough memory for malloc"

#define SQUARED(x) ((x)*(x))
#define TRUE 1
#define FALSE 0

typedef struct board
{
    int dim;
    unsigned int **table;
    unsigned int solvedCells;
    int numOfChildren;
    pNode **children;
} board;


pNode *newBoard(int toInitDim, unsigned int **toInitBoard);

unsigned int getVal(pNode p);

unsigned int *getRow(pNode p, int rowNum);

unsigned int getCell(pNode p, int rowNum, int colNum);

unsigned int *getCube(pNode p, int rowNum, int colNum);


void findFirstUnsolved(pNode board, int *row, int *col);

pNode copyBoard(pNode toCopy);

void freeSudoku(pNode toFree);

int checkValidNumbers(pNode p, int rowNum, int colNum, int toCheck);

int getPossibilities(pNode sudoku, pNode **possibilities);

#endif //EX3_SUDOKUTREE_H
