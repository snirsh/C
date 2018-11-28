//
// Created by Snir on 12/8/17.
//

#ifndef EX2_LINESEPARATOR_H
#define EX2_LINESEPARATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#define DEL_ERR "Couldn't delete vector"


enum argNums
{
    dimArg,
    mArg,
    argCount
};

/**
 * this is a structure i made for a Vector
 * a vector will have a coordinates array
 * and a status which will be 1 or -1 according to our algorithm
 */
struct vec
{
    double *coordinates;
    int status;
};

typedef struct vec Vector;

/**
 * this function creates a new vector of size of dimSize
 * @param dimSize - required dimension of the vector
 * @return a Vector of size dimSize
 */
Vector *newVec(int dimSize)
{
    assert(dimSize >= 0);
    Vector *x = (Vector *) malloc(sizeof(Vector));
    if(x != NULL)
    {
        x->coordinates = (double *) malloc(sizeof(double) * dimSize);
        for(int i = 0; i < dimSize; i++)
        {
            x->coordinates[i] = 0;
        }
        x->status = 0;
    }
    return x;
}


/**
 * this function deletes a given vector and frees space
 * @param del - vector to delete
 */
void freeVec(Vector *del)
{
    if(del == NULL)
    {
        fprintf(stderr, DEL_ERR);
        exit(EXIT_FAILURE);
    }
    free(del->coordinates);
    del->coordinates = NULL;
    free(del);
}


#endif //EX2_LINESEPARATOR_H
