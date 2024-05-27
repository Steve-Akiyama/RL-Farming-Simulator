#ifndef MAIN_H
#define MAIN_H

#include "TerminalUI.h"
#include "ActorCriticController.h"

class Main {
public:
    Main(); // Constructor
    void start(); // Method to start the program
    ActorCriticController _actorCriticController; 

private:
    void printAsciiTitle(); // Method to print the title in ASCII art
    void displayMenu(); // Method to display menu options
    void processInput(); // Method to process user input
};

#endif // MAIN_H
