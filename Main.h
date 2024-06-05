#ifndef MAIN_H
#define MAIN_H

#include "TerminalUI.h"
#include "MonteCarlo.h"
#include "ValueIteration.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdio>
#include <string>
#include <limits>

class Main {

    MonteCarlo method;

public:
    Main(); // Constructor
    void start(); // Method to start the program

private:
    int askForEpisodeCount();
    void printAsciiTitle(); // Method to print the title in ASCII art
    void displayMenu(); // Method to display menu options
    void processInput(); // Method to process user input
};

#endif // MAIN_H