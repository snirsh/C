#include "LineSeparator.h"
#define LINE_SIZE 400
#define EPSILON 0.00001
#define DECIMAL_BASE 10
#define READ_ONLY "r"
#define COMA_SIGN ","
#define BAD_INPUT "Bad input format"
#define PRINT_STATUS "%d\n"
#define GOOD_STATUS 1
#define BAD_STATUS (-1)
#define FILE_SRC_POSITION 1
#define INPUT_ARGS_NUMBER 2
#define TRUE 1
#define NO_ERRORS 0
#define NEW_LINE '\n'
#define STR_TO_DOUBLE_ERR "function strtod has a remainder which is not NULL check input"
#define SEGMENT_NULL_ERR "Segment returned from strtok is null!"
#define READ_LINE_ERR "Error reading line"
#define MEM_ERR "Not enough memory"
#define STR_TO_LONG_ERR "Function strtol returned an error"
#define ERR_CLOSE_FILE "Error closing file"
#define ERR_READING_FILE "Error reading file"


/**
 * this functions checks if file closed properly
 * @param file - currently opened file (not a const because we need to use close)
 * @return returns 0 if no errors occur
 */
int closeFileAndCheck(FILE *file);

/**
 * this functions checks the return value of a malloc functions
 * @param line
 * @return
 */
int checkMalloc(const char *mallocd);

/**
 * checks fgets for errors
 * @param line - fgets output
 */
int checkFgets(const char *line);

/**
 * this function checks if file was opened successfully
 * @param file
 * @return
 */
int checkFile(const FILE *file);

/**
 * this functions checks strtol output
 * @param i
 * @return
 */
int checkStrTol(int toCheck);

/**
 * checking if the remainder from strtod is a NULL else returning an exit failure
 * @param remainder - strtod(segment, &REMAINDER)  -- the remainder of strtod func
 * @return returns 0 if no errors occur
 */
int checkStrTod(char *remainder)
;

/**
 * this function checks the strtok segment output
 * @param segment - strtok output
 * @return 0 if ok else exit failure occurs
 */
int checkSegment(const char *segment);

/**
 * this function helps the readFile function to read a specific line and convert it to a vector
 * @param line - line to convert
 * @param dimSize - the dimension of the vectors
 * @return a Vector that was made from the given line
 */
Vector *readLine(char *line, const int dimSize, const int isTraining)
{
    assert(dimSize >= 0);
    char *segment = NULL;
    char *remainder = NULL;
    segment = strtok(line, COMA_SIGN);
    checkSegment(segment);
    int currentCoord = 0;
    Vector *v = newVec(dimSize);
    while(currentCoord < dimSize)
    {
        double coord = strtod(segment, &remainder);
        checkStrTod(remainder); //checking that the remainder is NULL
        v->coordinates[currentCoord] = coord;
        segment = strtok(NULL, COMA_SIGN);
        currentCoord++;
    }
    if(isTraining && checkSegment(segment) == NO_ERRORS)  // checking strtok again
    {
        v->status = (int) strtod(segment, &remainder);
        checkStrTod(remainder);
    }
    return v;
}


/**
 * this function calculates the standart inner product of 2 vectors of size dimSize
 * @param x - first vector
 * @param y - second vector
 * @param dimSize - dimension size
 * @return
 */
long double innerProduct(Vector *x, Vector *y, const int dimSize)
{
    assert(x != NULL && y != NULL && dimSize >= 0);
    long double sum = 0;
    for(int i = 0; i < dimSize; i++)
    {
        sum += x->coordinates[i] * y->coordinates[i];
    }
    return sum;
}


/**
 *  reads first two lines and recieves the dimension and m
 * @param file to read from
 * @param dim to write to the dim size
 * @param m to write to m
 */
void getFirstTwoLines(FILE *file, int *arguments)
{
    char *p;
    char line[sizeof(int)];
    for(int i = 0; i < 2; i++)
    {
        fgets(line, sizeof(int), file);
        checkFgets(line);
        arguments[i] = (int) strtol(line, &p, DECIMAL_BASE);
        checkStrTol(arguments[i]);
    }
}


/**
 * this function does w+=+x*x.status and updates W vector
 * @param w a vector
 * @param x a vector
 * @param dim size of dimension
 */
void updateWVector(Vector *w, Vector *x, const int dim)
{
    for(int i = 0; i < dim; i++)
    {
        w->coordinates[i] += ((x->status) * x->coordinates[i]);
    }
}


/**
 * main function here.
 * we read given file src and calculate W accordingly then printing 1 or -1 for each subject we got
 * @param src
 */
void readFile(char *src)
{
    FILE *file = fopen(src, READ_ONLY);
    checkFile(file);
    int dim, m;
    int isTraining = TRUE; //default because we train w first
    int args[argCount];
    getFirstTwoLines(file, args);
    dim = args[dimArg];
    m = args[mArg];
    Vector *w = newVec(dim);
    // training begins
    for(int i = 0; i < m; i++)
    {
        char *line = malloc(LINE_SIZE);
        checkMalloc(line);
        Vector *x = NULL; // i dont want to malloc for nothing the readLine will do it for me soon if fgets has no err
        fgets(line, LINE_SIZE, file);
        if(checkFgets(line) == NO_ERRORS)
        {
            x = readLine(line, dim, isTraining);
        }
        int newStatus = (innerProduct(x, w, dim) > 0) ? GOOD_STATUS : BAD_STATUS;
        if((x->status) != newStatus)
        {
            updateWVector(w, x, dim);
        }
        freeVec(x);
        free(line); // don't think i need to NULL the line. it will just get malloc'd again
    }
    isTraining--; // end training w and start calculating the vectors
    char *line = malloc(LINE_SIZE);
    checkMalloc(line);
    while(fgets(line, LINE_SIZE, file))
    {
        Vector *v = readLine(line, dim, isTraining);
        long double product = innerProduct(v, w, dim);
        int newStatus = (product >= EPSILON) ? GOOD_STATUS : BAD_STATUS;
        printf(PRINT_STATUS, newStatus);
        freeVec(v);
    }
    free(line);
    freeVec(w);
    closeFileAndCheck(file);
}


int main(int argc, char *argv[])
{
    if(argc != INPUT_ARGS_NUMBER)
    {
        fprintf(stderr, BAD_INPUT);
        exit(EXIT_FAILURE);
    }
    else
    {
        readFile(argv[FILE_SRC_POSITION]);
    }
    return NO_ERRORS;
}


//DATA CHECKING FUNCTIONS
/**
 * this functions checks the return value of a malloc functions
 * @param line
 * @return
 */
int checkMalloc(const char *mallocd)
{
    if(mallocd == NULL)
    {
        fprintf(stderr, MEM_ERR);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}


/**
 * this function checks the strtok segment output
 * @param segment - strtok output
 * @return 0 if ok else exit failure occurs
 */
int checkSegment(const char *segment)
{
    if(segment == NULL)
    {
        fprintf(stderr, SEGMENT_NULL_ERR);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}


/**
 * checking if the remainder from strtod is a NULL else returning an exit failure
 * @param remainder - strtod(segment, &REMAINDER)  -- the remainder of strtod func
 * @return returns 0 if no errors occur
 */
int checkStrTod(char *remainder)
{
    if(strlen(remainder) != 0 && *remainder != NEW_LINE)
    {
        fprintf(stderr, STR_TO_DOUBLE_ERR);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}


/**
 * checks fgets for errors
 * @param line - fgets output
 */
int checkFgets(const char *line)
{
    if(line == NULL)
    {
        fprintf(stderr, READ_LINE_ERR);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}


/**
 * this functions checks strtol output
 * @param i
 * @return
 */
int checkStrTol(int toCheck)
{
    if(toCheck == 0)
    {
        fprintf(stderr, STR_TO_LONG_ERR);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}


/**
 * this function checks if file was opened successfully
 * @param file
 * @return
 */
int checkFile(const FILE *file)
{
    if(file == NULL)
    {
        fprintf(stderr, ERR_READING_FILE);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}

/**
 * this functions checks if file closed properly
 * @param file - currently opened file (not a const because we need to use close)
 * @return returns 0 if no errors occur
 */
int closeFileAndCheck(FILE *file)
{
    if(fclose(file))
    {
        fprintf(stderr, ERR_CLOSE_FILE);
        exit(EXIT_FAILURE);
    }
    return NO_ERRORS;
}
