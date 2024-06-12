#ifndef MAIN_H
#define MAIN_H

#include "MonteCarlo.h"
#include <iostream>
#include <limits>
#include <string>

class Main {
    MonteCarloMDP method;

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
