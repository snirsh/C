#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHA 0.012299
#define BETA (1-ALPHA)
#define D_ROOT (1.5)
#define SQUARED 2
#define VAR_SIZE 256
#define MAX_LINE_SIZE ((VAR_SIZE)*5)
#define MAX_ARGS 4
#define TOTAL_ARGS 7
#define ERROR_O "Error Opening File\n"
#define D_BAD_VALUE "one of the D's got value 0, division by zero is imminent!\n"
#define ERR_PARAM "Bad parameters\n"
#define ERR_ARG_INPUT "Bad input format\n"
#define ERR_NAN "Entered input is not a number\n"
#define READ_ONLY "r"
#define WRITE_MODE "w"
#define COMA_SEPARATOR ", \r\n"
#define COMA ", "
#define IS_POS(x) (x>0.0)
#define IS_WHOLE(x) !(x-roundf(x))
#define WHOLE_AND_POS(x) (IS_POS(x)&&IS_WHOLE(x))
#define POSX_MSG "Enter initial pos x:\n"
#define POSY_MSG "Enter initial pos y:\n"
#define VELX_MSG "Enter initial vel x:\n"
#define VELY_MSG "Enter initial vel y:\n"
#define TIME_MSG "Enter total time T:\n"
#define N_MSG "Enter num of steps:\n"
#define M_MSG "Enter num of steps to save:\n"
#define OUTPUT_TYPE "%.3Le"
#define END_LINE "\n"
#define TWO_ARGS 2
#define THREE_ARGS 3
#define INPUT_FILE 1
#define OUTPUT_FILE 2
#define OUTPUT_FILE_USR_INPUT 1
#define NEW_LINE '\n'
/**
* DECLARATION OF GLOBAL VARIABLES
*/
long double gD1, gD2, gAx, gAy, gDt;
long double gTime; // me love u
int gN,gM;
long double args[TOTAL_ARGS];
enum argumentPositions {x, y, vx, vy, t, n, m};

/**
 * this function calculates D1 and D2
 */
void findD()
{
    gD1 = powl((powl((args[x]+ALPHA), SQUARED) + powl(args[y],SQUARED)), D_ROOT);
    gD2 = powl((powl((args[x]-BETA), SQUARED) + powl(args[y],SQUARED)), D_ROOT);
    if(gD1 == 0 || gD2 == 0) //checking if one of the d's is zero because we will devide by D later.
    {
        fprintf(stderr, D_BAD_VALUE);
        exit(EXIT_FAILURE);
    }
}


/**
 * this function calculates the acceleration of x and y
 * a given formula
 */
void findAcceleration()
{
    findD(); // first we find D1 and D2
    gAx = (args[x] + 2 * args[vy] - BETA *((args[x]+ALPHA)/gD1) - ALPHA * ((args[x]-BETA)/gD2));
    gAy = (args[y] - 2 * args[vx] - BETA * (args[y]/gD1)- ALPHA * (args[y]/gD2));
}


/**
 * this function calculates x,y, and the velocity of x and y according at time dt
 */
void forwardEulerFunc()
{
    args[x] += (args[vx] * gDt);
    args[y] += (args[vy] * gDt);
    args[vx] += (gAx * gDt);
    args[vy] += (gAy * gDt);
}


/**
 * this is the main calculation function.
 * the function claculates the arenstorf equation and writes the results to give file
 * @param file - output file that results will be written to
 */
void arenstorfFunction(FILE *file)
{
    gDt = gTime / gN;
    int interval = gN/gM;
    char *sep = COMA;
    for(int i = 1; i <= gN; i++)
    {
        findAcceleration();
        forwardEulerFunc();
        if(i % interval == 0)
        {
            //basically this is where we write back to the file we do so every m steps thus the i%interval
            char out[VAR_SIZE];
            snprintf(out, VAR_SIZE, OUTPUT_TYPE, args[x]);//fetch x
            fputs(out, file);
            fputs(COMA, file);
            snprintf(out, VAR_SIZE, OUTPUT_TYPE, args[y]);//fetch y
            fputs(out, file);
            if (i == gN)
            {
                sep = END_LINE; //if we've reached n steps we will also put a \n separator until then its a ','
            }
            fputs(sep, file);
        }
    }
    fclose(file);
}


/**
 * this function recieves the main array that has all the arguments inside and assigns them to according globals.
 */
void assignGlobals()
{
    gTime = args[t];
    gN = (int)args[n];
    gM = (int)args[m];
    if(!IS_POS(gTime) || !WHOLE_AND_POS(gN) || !WHOLE_AND_POS(gM) || gN % gM !=0) {
        fprintf(stderr, ERR_PARAM);
        exit(EXIT_FAILURE);
    }
}


/**
 * checks if given string is a number and not any illegal input i.e 102aaa 1.101.22 and so on
 * @param str - string to check
 */
void isNumber(char* str)
{
    int length =  strlen(str);
    int dotCounter = 0;
    for (int i=0;i<length; i++) //runs on every char in the given string
    {
        if (!isdigit(str[i]) && str[i] != '.' && str[i]!='-') //we can have either digit either - sign either .
        {
            fprintf(stderr, ERR_NAN);
            exit(EXIT_FAILURE);
        }
        else if(str[i] == '.' && dotCounter == 0) //if we have a dot we'll check there's only 1 thus we have a counter
        {
            dotCounter++;
        }
        else if(str[i] == '.' && dotCounter>0)
        {
            fprintf(stderr, ERR_NAN);
            exit(EXIT_FAILURE);
        }
    }
}


/**
 * this function converts a given string to a long double
 * @param segment - string to convert
 * @return  a long double that was made from given segment
 */
long double strToDouble(char *segment)
{
    char *p = NULL;
    long double ans = strtod(segment, &p);
    char *pos;
    if ((pos=strchr(segment, '\n')) != NULL) //checking if the remaining of the string after strtod is a null
    {
        *pos = '\0';
    }
    if(strlen(p)!=0 && *p != NEW_LINE) //it might be a '\n' thus we check if it is
    {
        fprintf(stderr, ERR_ARG_INPUT);
        exit(EXIT_FAILURE);
    }
    isNumber(segment);
    return ans;
}


/**
 * this function reads a given line.
 * i've calculated how many arguments are supposed to be in the current line by the lineNum.
 * @param line - line to read
 * @param lineNum - #of the line given
 */
void readLine(char *line, const int lineNum)
{
    const int lineArgs = MAX_ARGS-lineNum; //line 0 has 4-0=4 arguments ==MAX and line 1 has 4-1=3 arguments ==MAX-line
    char *segment = strtok(line, COMA_SEPARATOR); //segment is a part of a line that goes from NULL to a coma ','
    int i=lineNum*MAX_ARGS; //we need to assign the arguments to 0-3(line0*4) or 4-6(line1*4)
    while(segment)
    {
        if(i>(lineArgs + (lineNum*MAX_ARGS)))
        {
            fprintf(stderr, ERR_ARG_INPUT);
            exit(EXIT_FAILURE);
        }
        else
        {
            args[i] = strToDouble(segment); //checking if valid and returning as a long double to assign in array
            segment = strtok(NULL, COMA_SEPARATOR);
            i++;
        }
    }
    if(i<lineArgs)
    {
        fprintf(stderr, ERR_ARG_INPUT);
        exit(EXIT_FAILURE);
    }
}


/**
 * this function reads a given file by lines.
 * @param fileSrc - the file name.
 */
void readFile(char *fileSrc)
{
    FILE* file;
    file = fopen(fileSrc, READ_ONLY);
    if(file == NULL)
    {
        fprintf(stderr, ERROR_O);
        exit(EXIT_FAILURE);
    }
    else
    {
        int i=0;
        char *line = malloc(MAX_LINE_SIZE); //allocating size for the coming line
        while(fgets(line, MAX_LINE_SIZE, file) != NULL){
            if(i>1)
            {
                fprintf(stderr, ERR_ARG_INPUT);
                exit(EXIT_FAILURE);
            }
            else
            {
                readLine(line, i); //here we send the current line to the readline function and read it by segments.
                i++;
            }
        }
        free(line);
        line = NULL; //as we learned :)
        if(i<1)
        {
            fprintf(stderr, ERR_ARG_INPUT);
            exit(EXIT_FAILURE);
        }
        assignGlobals(args); //assigning the remaining values to globals (time,n,m) and checking if they are valid
        fclose(file);
    }
}


/**
 * this function helps to read input from user. it will print msg first and then assign it as a long double to args
 * @param msg - message
 * @param i
 */
void readFromUserHelper(char* msg, int i)
{
    char buffer[VAR_SIZE];
    printf(msg); //prompt usr
    fgets(buffer,VAR_SIZE, stdin); //get usr input
    args[i] = strToDouble(buffer); //check usrinput and assign to arguments array
}


/**
 * this function reads input from user and assigns it to our arguments array
 */
void readFromUser()
{
    int pos = x;
    for(int i = 0; i<7; i++)
    {
        switch(pos) //i really wanted to ues a switch and it was just perfect here so... no further explanation, enjoy.
        {
            case x:
                readFromUserHelper(POSX_MSG, pos);
                break;
            case y:
                readFromUserHelper(POSY_MSG, pos);
                break;
            case vx:
                readFromUserHelper(VELX_MSG, pos);
                break;
            case vy:
                readFromUserHelper(VELY_MSG,pos);
                break;
            case t:
                readFromUserHelper(TIME_MSG,pos);
                break;
            case n:
                readFromUserHelper(N_MSG, pos);
                break;
            case m:
                readFromUserHelper(M_MSG, pos);
                break;
        }
        pos ++;
    }
    assignGlobals(args);
}

/**
 * main function
 * @param argc - #of arguments
 * @param argv - array of the given inputs
 * @return
 */
int main(int argc, char * argv[])
{
    FILE *file;
    if(argc == THREE_ARGS) // reading from file
    {
        readFile(argv[INPUT_FILE]);
        file = fopen(argv[OUTPUT_FILE], WRITE_MODE);
    }
    else if(argc == TWO_ARGS) //reading from user
    {
        readFromUser();
        file = fopen(argv[OUTPUT_FILE_USR_INPUT], WRITE_MODE);
    }
    else
    {
        fprintf(stderr, ERR_ARG_INPUT);
        exit(EXIT_FAILURE);
    }
    // calculating and writing to file
    arenstorfFunction(file);
    return 0;
}