#include "TerminalUI.h"
#include <iostream>
#include <limits>
#include <sstream>

TerminalUI::TerminalUI() {
    // Constructor - no initialization needed
}

void TerminalUI::start() {
    std::cout << "Welcome to the Plant Farm Simulator!\n\n";

    while (true) {
        displayMenu();
        processInput();
    }
}

void TerminalUI::displayMenu() {
    std::cout << "Options:\n";
    std::cout << "1. \"<Water [0, 4]> <Nitrogen [0, 4]>\" to enter a water and nitrogen value\n";
    std::cout << "2. \"exit\" to exit the program\n";
    std::cout << "Choose an option: ";
}

void TerminalUI::processInput() {
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit") {
        std::cout << "Exiting...\n";
        exit(0);
    }

    std::istringstream iss(input);
    int water, nitro;
    if (!(iss >> water >> nitro)) {
        std::cout << "Invalid input. Please enter integers for water and nitrogen values.\n";
        return;
    }

    plantFarm.transition(water, nitro);
}
