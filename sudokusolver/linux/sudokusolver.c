#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_FILENAME_LENGTH 30


typedef struct Coords2D
{
    int x;
    int y;
}Coords2D;


typedef enum Ebool
{
    false = 0, true = 1
}bool;


int sudokuField[9][9];
int numOfPossibilitys;


Coords2D next(Coords2D before,  int maxX)
{
    Coords2D after = before;

    if(++after.x > maxX)
    {
        after.x = 0;
        after.y++;
    }

    return after;
}


void initSudoku()
{
    memset(sudokuField, 0, sizeof(sudokuField));

    numOfPossibilitys = 0;
}


bool isNumber(char character)
{
    return ((character >= '0') && (character <= '9'));
}


int getNumber(FILE *fhandle)
{
    char character = 0;

    while(!isNumber(character))
    {
        if(feof(fhandle))
            return 10;

        character = getc(fhandle);
    }

    return (character - '0');
}


bool loadSudoku(const char *filename)
{
    int i, j;
    FILE *f = NULL;

    if((f = fopen(filename, "r")) == NULL)
        return false;

    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if((sudokuField[j][i] = getNumber(f)) == 10)
                return false;

    return true;
}


bool linePossible(int nr)
{
    if(nr >= 9 || nr <= -1)
    {
        fprintf(stderr, "*** sudoku error: linePossible got false argument %d. ***\n", nr);
        fprintf(stderr, "*** comment: this should never happen.                ***\n");
        return false;
    }

    int i;
    bool numberSeen[9];

    memset(numberSeen, false, sizeof(numberSeen));

    for(i = 0; i < 9; i++)
    {
        if(sudokuField[nr][i] == 0)
            continue;

        if(numberSeen[sudokuField[nr][i] - 1] == true)
            return false;
        else
            numberSeen[sudokuField[nr][i] - 1] = true;
    }

    return true;
}


bool columnPossible(int nr)
{
    if(nr >= 9 || nr <= -1)
    {
        fprintf(stderr, "*** sudoku error: columnPossible got false argument %d. ***\n", nr);
        fprintf(stderr, "*** comment: this should never happen.                  ***\n");
        return false;
    }

    int i;
    bool numberSeen[9];

    memset(numberSeen, false, sizeof(numberSeen));

    for(i = 0; i < 9; i++)
    {
        if(sudokuField[i][nr] == 0)
            continue;

        if(numberSeen[sudokuField[i][nr] - 1] == true)
            return false;
        else
            numberSeen[sudokuField[i][nr] - 1] = true;
    }

    return true;
}


bool boxPossible(int nr)
{
    int i;

    if(nr >= 9 || nr <= -1)
    {
        fprintf(stderr, "*** sudoku error: boxPossible got false argument %d. ***\n", nr);
        fprintf(stderr, "*** comment: this should never happen.               ***\n");
        return false;
    }

    Coords2D baseCoords = {(nr % 3) * 3, (nr / 3) * 3};
    Coords2D localCoords = {0, 0};
    Coords2D resultingCoords;
    bool numberSeen[9];

    memset(numberSeen, false, sizeof(numberSeen));

    for(i = 0; i < 9; i++)
    {
        resultingCoords.x = baseCoords.x + localCoords.x;
        resultingCoords.y = baseCoords.y + localCoords.y;

        if(sudokuField[resultingCoords.x][resultingCoords.y] == 0)
            continue;

        if(numberSeen[sudokuField[resultingCoords.x][resultingCoords.y] - 1] == true)
            return false;
        else
            numberSeen[sudokuField[resultingCoords.x][resultingCoords.y] - 1] = true;

        localCoords = next(localCoords, 2);
    }

    return true;
}


bool linesPossible()
{
    int i;

    for(i = 0; i < 9; i++)
        if(linePossible(i) == false)
            return false;

    return true;
}


bool columnsPossible()
{
    int i;

    for(i = 0; i < 9; i++)
        if(columnPossible(i) == false)
            return false;

    return true;
}


bool boxesPossible()
{
    int i;

    for(i = 0; i < 9; i++)
        if(boxPossible(i) == false)
            return false;

    return true;
}


bool possible()
{
    return (linesPossible() && columnsPossible() && boxesPossible());
}


bool completed()
{
    int i, j;

    for(i = 0; i < 9; i++)
        for(j = 0; j < 9; j++)
            if(sudokuField[i][j] == 0)
                return false;

    return true;
}


bool solved()
{
    return (possible() && completed());
}


void fillFromField(Coords2D coords)
{
    int i;

    if(coords.x < 0 || coords.x > 8 || coords.x < 0 || coords.x > 8)
    {
        fprintf(stderr, "*** sudoku error: fillFromField got impossible parameters: X = %d , Y = %d ***\n", coords.x, coords.y);
        fprintf(stderr, "*** comment: This should never happen.                                     ***\n");
        exit(-1);
    }

    if(sudokuField[coords.x][coords.y] != 0)
    {
        if(completed())
            return;

        fillFromField(next(coords, 8));
        return;
    }

    for(i = 1; i <= 9; i++)
    {
        sudokuField[coords.x][coords.y] = i;

        if(solved())
        {
            numOfPossibilitys++;
            printf("+---+---+---+\n");
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][0], sudokuField[1][0], sudokuField[2][0], sudokuField[3][0], sudokuField[4][0], sudokuField[5][0], sudokuField[6][0], sudokuField[7][0], sudokuField[8][0]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][1], sudokuField[1][1], sudokuField[2][1], sudokuField[3][1], sudokuField[4][1], sudokuField[5][1], sudokuField[6][1], sudokuField[7][1], sudokuField[8][1]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][2], sudokuField[1][2], sudokuField[2][2], sudokuField[3][2], sudokuField[4][2], sudokuField[5][2], sudokuField[6][2], sudokuField[7][2], sudokuField[8][2]);
            printf("+---+---+---+\n");
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][3], sudokuField[1][3], sudokuField[2][3], sudokuField[3][3], sudokuField[4][3], sudokuField[5][3], sudokuField[6][3], sudokuField[7][3], sudokuField[8][3]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][4], sudokuField[1][4], sudokuField[2][4], sudokuField[3][4], sudokuField[4][4], sudokuField[5][4], sudokuField[6][4], sudokuField[7][4], sudokuField[8][4]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][5], sudokuField[1][5], sudokuField[2][5], sudokuField[3][5], sudokuField[4][5], sudokuField[5][5], sudokuField[6][5], sudokuField[7][5], sudokuField[8][5]);
            printf("+---+---+---+\n");
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][6], sudokuField[1][6], sudokuField[2][6], sudokuField[3][6], sudokuField[4][6], sudokuField[5][6], sudokuField[6][6], sudokuField[7][6], sudokuField[8][6]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][7], sudokuField[1][7], sudokuField[2][7], sudokuField[3][7], sudokuField[4][7], sudokuField[5][7], sudokuField[6][7], sudokuField[7][7], sudokuField[8][7]);
            printf("|%d%d%d|%d%d%d|%d%d%d|\n", sudokuField[0][8], sudokuField[1][8], sudokuField[2][8], sudokuField[3][8], sudokuField[4][8], sudokuField[5][8], sudokuField[6][8], sudokuField[7][8], sudokuField[8][8]);
            printf("+---+---+---+\n");
            sudokuField[coords.x][coords.y] = 0;
            return;
        }
        else if(possible())
        {
            fillFromField(next(coords, 8));
        }
    }

    sudokuField[coords.x][coords.y] = 0;

    return;
}


int solve()
{
    numOfPossibilitys = 0;

    Coords2D start = {0, 0};

    fillFromField(start);

    return numOfPossibilitys;
}



int main(int argc, char **argv)
{
    int returnValue = 0;
    char filename[MAX_FILENAME_LENGTH];

    if(argc <= 0)
    {
        fprintf(stderr, "*** sudoku error: programm got impossible number of arguments: %d. ***\n", argc);
        fprintf(stderr, "*** comment: this should never happen.                             ***\n");

        return -1;
    }
    else if(argc == 1)
    {
        memset(filename, '\0', sizeof(filename));

        printf("Which sudoku-file do you want to load?\n");
        scanf("%s", filename);
        printf("\n");

        if(!loadSudoku(filename))
        {
            fprintf(stderr, "*** sudoku error: unable to load file %s. ***\n", filename);
            fprintf(stderr, "*** comment: maybe it is corrupted or not existing. ***\n");
            return -1;
        }

        returnValue = solve();
    }
    else
    {
        int i;

        for(i = 1; i < argc; i++)
        {
            if(argv[i][0] == '-')
            {
                if(!strncmp("-h", argv[i], 2) || !strncmp("-help", argv[i], 5))
                {
                    printf("\n");
                    printf("How to use peter's sudoku-solver\n");
                    printf("--------------------------------\n");
                    printf("\n");
                    printf("1. Write a sudoku-file. It has to contain the sudoku-numbers\n");
                    printf("   from the left to the right and from the top to the bottom. 0 means empty.\n");
                    printf("   You can use all the placeholders you want.\n");
                    printf("\n");
                    printf("2. Give the name of the file to the programm.\n");
                    printf("   You can do this in the command-line or in the dialoge which is displayed\n");
                    printf("   if you start the programm without parameters.\n");
                    printf("\n");
                    printf("Other commandline options: \n");
                    printf("-h or -help:    Displays this help.\n");
                    printf("-c or -credits: Shows the credits.\n");
                    printf("-v or -version: Displays the version of this programm.\n");
                    printf("\n");
                }
                else if(!strncmp("-c", argv[i], 2) || !strncmp("-credits", argv[i], 8))
                {
                    printf("\n");
                    printf("Credits\n");
                    printf("-------\n");
                    printf("\n");
                    printf("Peter Hoesch: Programmer (forever alone)\n");
                    printf("\n");
                    printf("Many thanks also to:\n");
                    printf("-The programmers of linux\n");
                    printf("-The Code::Blocks-Team\n");
                    printf("-The inventors of c++\n");
                    printf("-The inventors of sudoku\n");
                    printf("Without you this had not been possible!\n");
                    printf("\n");
                }
                else if(!strncmp("-v", argv[i], 2) || !strncmp("-version", argv[i], 8))
                {
                    printf("\n");
                    printf("Peter's sudoku-solver version 1.0.0\n");
                    printf("\n");
                }
                else
                {
                    fprintf(stderr, "*** sudoku error: unknown parameter \"%s\". ***\n", argv[i]);
                }
            }
            else
            {
                memset(filename, '\0', sizeof(filename));
                strncpy(filename, argv[i], MAX_FILENAME_LENGTH);

                if(!loadSudoku(filename))
                {
                    fprintf(stderr, "*** sudoku error: unable to load file %s. ***\n", filename);
                    fprintf(stderr, "*** comment: maybe it is corrupted or not existing. ***\n");
                    return -1;
                }

                returnValue += solve();
            }
        }
    }

    return returnValue;
}
