/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2025
 * Date: 07.02.2025
 */

#ifndef _WATER_SEVEN_CAMPAIGN_H_
#define _WATER_SEVEN_CAMPAIGN_H_

#include "main.h"

const int MAX_NAME = 100;
const int FIXED_CHARACTER = 7;
const int MAX_GRID = 50;

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

extern char character[FIXED_CHARACTER][MAX_NAME];
extern int hp[FIXED_CHARACTER];
extern int skill[FIXED_CHARACTER];
extern int shipHP, repairCost;
extern char duel[FIXED_CHARACTER][MAX_NAME];
extern char resultText[];
extern int grid[MAX_GRID][MAX_GRID];
extern int rows, cols;
extern int n_support;
//Task 0
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER],
    int skill[FIXED_CHARACTER], int &shipHP, int &repairCost);



// Task 1
int damageEvaluation(int shipHP, int repairCost);

// Task 2

int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost);

// Task 3
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]);

// Task 4
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME],
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex,
    int repairCost, char cipherText[], char resultText[]);

// Task 5
void read_grid();
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols);
bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit);

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////

#endif // _WATER_SEVEN_CAMPAIGN_H_
