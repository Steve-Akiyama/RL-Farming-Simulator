#ifndef TERMINAL_UI_H
#define TERMINAL_UI_H

#include "PlantFarm.h"
#include "ValueIteration.h"

class TerminalUI {
public:
    TerminalUI(); // Constructor
    void start(); // Method to start the UI

private:
    PlantFarm plantFarm; // Instance of PlantFarm
    void displayMenu(); // Method to display menu options
    bool processInput(); // Method to process user input
};

#endif // TERMINAL_UI_H
