/*
* Ho Chi Minh City University of Technology
* Faculty of Computer Science and Engineering
* Initial code for Assignment 1
* Programming Fundamentals Spring 2026
* Date: 27.01.2026
*/

//The library here is concretely set, students are not allowed to include any other libraries.

#include "water_seven.h"
using namespace std;
int conflictIndex = 0;
int main(int argc, const char * argv[]) {

    /// Task 0
    cout << readInput("opw_tc_01_input.txt", character, hp, skill, shipHP, repairCost) << "\n";

    for (long i = 0; i < FIXED_CHARACTER; i++) {
        cout << character[i] << " ";
    }
    cout << "\n";

    for (long i = 0; i < FIXED_CHARACTER; i++) {
        cout << hp[i] << " ";
    }
    cout << "\n";

    for (long i = 0; i < FIXED_CHARACTER; i++) {
        cout << skill[i] << " ";
    }
    cout << "\n";
    cout << shipHP << " " << repairCost << "\n";

    /// Task 1
    cout << "Task 1" << "\n";
    cout << "Damage: " << damageEvaluation(shipHP, repairCost) << "\n";

    /// Task 2
    cout << "Task 2" << "\n";
    cout << "ConflictIndex: " << conflictSimulation(character, hp, skill, shipHP, repairCost) << "\n";
    conflictIndex = 120;

    /// Task 3
    cout << "Task 3" << "\n";
    resolveDuel(character, hp, skill, conflictIndex, repairCost, duel);
    cout << "The number of support: " << n_support << "\n";
    for (long i = 0; i < n_support; i++) {
        cout << duel[i] << " ";
    }

    /// Task 4
    cout << "Task 4" << "\n";
    char cipherText[] = "AQ 7XK#96";
    decodeCP9Message(character, hp, skill, conflictIndex, repairCost, cipherText, resultText);
    cout << resultText << "\n";

    /// Task 5
    cout << "Task 5" << "\n";
    //read_grid();
    int DangerLimit = analyzeDangerLimit(grid, rows, cols);
    cout << "DangerLimit: " << DangerLimit << "\n";

    if (evaluateRoute(grid, rows, cols, DangerLimit)) {
        cout << "Pass!";
    }
    else cout << "Fail!";


    return 0;
}
