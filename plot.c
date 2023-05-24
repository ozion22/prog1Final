#include <ncurses.h>
#include <unistd.h>
#include "/home/timsaa/C/Headers/timstandard.h"

double *values;
int valuenumber = 0;
char *readData;
FILE *fp;

void parse(char inputString[]) // Parses strings taken from the output of the python file
{
    values = malloc(__SIZEOF_DOUBLE__);     // Initial memory
    char *token = strtok(inputString, ";"); // Tokenizes the string
    if (token == NULL)
    {
        throwErr("No input data to parse", __LINE__);
    }
    while (token != NULL) // Loops through tokens
    {
        valuenumber++;
        // printf("Token: %s\n", token); DEBUG
        values = realloc(values, valuenumber * 8);
        // printf("Reallocated at : %d", valuenumber); // DEBUG
        if (!(token[0] == 0))
        {
            values[valuenumber - 1] = strtod(token, NULL);
        }                          // Converts to double
        token = strtok(NULL, ";"); // Gets new token
    }
}

void getdata() // Gets data from output file
{
    char readChar;
    readData = malloc(sizeof(char)); // initial memory
    int numberOfChars = 1;
    fp = fopen("pythonOutput.txt", "r"); // Reads from pythonOutput.txt
    while ((readChar = fgetc(fp)) != EOF)
    {
        // printf("%c\n", readChar); DEBUG
        readData = realloc(readData, numberOfChars);
        readData[numberOfChars - 1] = readChar;
        numberOfChars += 1;
    }
    readData = realloc(readData, numberOfChars);
    readData[numberOfChars - 1] = 0; // Ensure the string is null-terminated
    parse(readData);                 // Parses the data read
}

void plot(double valuesToPlot[], int numberOfValues, int beginningX, int endX, int mode) // mode is 0 for standard, 1 for squished
{
    clear();
    int rows, columns;
    getmaxyx(stdscr, rows, columns);
    // printf("ROWS: %d | COLUMNS: %d | BEGINNINGX: %d | ENDX:%d\n", rows, columns, beginningX, endX); DEBUG
    // printf("PRE: %d", (endX - beginningX)); DEBUG
    double step = (double)(endX - beginningX) / rows;
    // printf("Step: %lf\n", step); DEBUG
    int scrMidY = rows / 2; // Establish midpoints of the monitor
    if (scrMidY % 2 != 0)
    {
        scrMidY -= 1;
    }

    int scrMidX = columns / 2; // Another midpoint
    if (scrMidX % 2 != 0)
    {
        scrMidX -= 1;
    }
    double maxYReached = 0; // The local maximum of the function given the range.
    double minYReached = 0;
    for (int i = 0; i < numberOfValues; i++)
    {
        if (valuesToPlot[i] > maxYReached)
        {
            maxYReached = valuesToPlot[i];
        }
        else if (valuesToPlot[i] < minYReached)
        {
            minYReached = valuesToPlot[i];
        }
    }

    if (mode == 0)
    {
        for (int i = 0; i < rows - 1; i++)
        {
            mvprintw(i, scrMidX, "|");
            usleep(5000);
            refresh();
        }
        mvprintw(rows - 1, scrMidX, "%.1lf", (-1) * (step * (rows / 2)));
        mvprintw(0, scrMidX, "%.1lf", (step * (rows / 2)));

        for (int i = 0; i < columns - 3; i++)
        {
            mvprintw(scrMidY, i, "-");
            usleep(5000);
            refresh();
        }
        mvprintw(scrMidY, columns - 5, "%3d", endX);
        mvprintw(scrMidY, 0, "%3d", beginningX);
    }
    double yToPlot;
    for (int i = 0; i < columns; i++) // Plots the actual points on the graph
    {
        if (mode == 1)
        {
            yToPlot = (-1) * ((((valuesToPlot[i] - minYReached) / (maxYReached - minYReached)) * rows) - rows); // Calculates the y to plot, streches the graph drastically
            mvprintw(yToPlot, i, "*");
        }
        else
        {
            yToPlot = scrMidY - (valuesToPlot[i] / step); // More accurate
            if (!((yToPlot > rows) || (yToPlot < 0)))
            {
                mvprintw(yToPlot, i, "*");
            }
        }
        refresh();
        usleep(10000);
    }
    refresh();
    getch();
}

void displaySpecialPoints(double valuesToPlot[])
{
    int rows, columns, numberOfMaximi, numberOfMinimi;
    double *localMaximi, *localMinimi;

    getmaxyx(stdscr, rows, columns);
    double maxYValue = 0, minYValue = 0;
    for (int i = 0; i < columns; i++)
    {
        if (valuesToPlot[i] > maxYValue)
        {
            maxYValue = valuesToPlot[i];
        }
        else if (valuesToPlot[i] < minYValue)
        {
            minYValue = valuesToPlot[i];
        }

        if ((valuesToPlot[i] > valuesToPlot[(i + 1)]) && (valuesToPlot[i] > valuesToPlot[(i - 1)])) // Gets the local maximi
        {
            if (numberOfMaximi == 0)
            {
                localMaximi = malloc(__SIZEOF_DOUBLE__);
                numberOfMaximi++;
                localMaximi[0] = valuesToPlot[i];
            }
            else
            {
                numberOfMaximi++;
                localMaximi = realloc(localMaximi, __SIZEOF_DOUBLE__ * numberOfMaximi);
                localMaximi[numberOfMaximi - 1] = valuesToPlot[i];
            }
        }
        if (valuesToPlot[i] < valuesToPlot[(i + 1)] && (valuesToPlot[i] < valuesToPlot[(i - 1)])) // Gets the local minimi
        {
            if (numberOfMinimi == 0)
            {
                localMinimi = malloc(__SIZEOF_DOUBLE__);
                numberOfMinimi++;
                localMinimi[0] = valuesToPlot[i];
            }
            else
            {
                numberOfMinimi++;
                localMinimi = realloc(localMinimi, __SIZEOF_DOUBLE__ * numberOfMinimi);
                localMinimi[numberOfMinimi - 1] = valuesToPlot[i];
            }
        }
    }
    clear();
    refresh();
    printw("Maximum value: %lf\n", maxYValue);
    printw("Minimum value: %lf\n", minYValue);
    printw("Local minimum values:\n");
    for (int i = 0; i < numberOfMinimi; i++)
    {
        printw("%lf\n", localMinimi[i]);
    }
    printw("\nLocal Maximum values:\n");
    for (int i = 0; i < numberOfMaximi; i++)
    {
        printw("%lf\n", localMaximi[i]);
    }
    printw("Press ENTER to return\n");
    getch();
}

int modeSelect()
{
    printw("Which mode would you like to use?\n");
    printw("1: Standard graph\n2: General graph(innacurate)\n3: Special points\n4: Quit\n");
    char choice = getch();
    printw("Choice : %c\n", choice);
    switch (choice)
    {
    case '1':
        return 1;
        break;
    case '2':
        return 2;
        break;
    case '3':
        return 3;
        break;
    case '4':
        return 4;
        break;
    default:
        clear();
        refresh();
        return modeSelect();
    }
}

int main(int argc, char *argv[])
{
    int top = (int)strtol(argv[1], NULL, 10);
    int bottom = (int)strtol(argv[2], NULL, 10);
    getdata();
    initscr();
    int mode;
    while (1)
    {
        clear();
        refresh();
        mode = modeSelect();
        clear();
        refresh();
        if (mode == 3)
        {
            displaySpecialPoints(values);
        }
        else if (mode == 4)
        {
            break;
        }
        else
        {
            plot(values, valuenumber, bottom, top, mode - 1);
        }
    }
    free(values);
    endwin();
    return 0;
}
