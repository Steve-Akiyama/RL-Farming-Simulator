#include "TerminalUI.h"
#include <iostream>
#include <limits>
#include <sstream>

TerminalUI::TerminalUI() {
    // Constructor - no initialization needed
}

void TerminalUI::start() {
    std::cout << "Welcome to the Plant Farm Simulator!\n\n";

    //While the UI should run
    while (true) {
        displayMenu(); //Display menu options

        if (processInput()) {
            return; //If processInput returns exit code, return
        }
    }
}

void TerminalUI::displayMenu() {
    std::cout << "Options:\n";
    std::cout << "1. \"<Water [0, 4]> <Nitrogen [0, 4]>\" to enter a water and nitrogen value\n";
    std::cout << "2. \"exit\" to exit the program\n";
    std::cout << "Choose an option: ";
}

bool TerminalUI::processInput() {
    std::string input;
    std::getline(std::cin, input);

    //Exits if the user desires to
    if (input == "exit") {
        std::cout << "Exiting...\n";
        exit(0);
    }

    //Recieves water and nitrogen inputs
    std::istringstream iss(input);
    int water, nitro;
    if (!(iss >> water >> nitro)) {
        std::cout << "Invalid input. Please enter integers for water and nitrogen values.\n";
        return 0;
    }

    //Returns the exit code and prints a message when time is complete
    if (plantFarm.transition(water, nitro)) {
        std::cout << "\n---- Plant growth complete! ----\n\n";
        return 1;
    }

    return 0;
}