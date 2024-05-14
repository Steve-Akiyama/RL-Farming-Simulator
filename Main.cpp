#include "Main.h"
#include "TerminalUI.h"
#include <iostream>
#include <limits>

Main::Main() {
    // Constructor - no initialization needed
}

void Main::start() {
    std::cout << "Welcome to the Plant Farm Simulator!\n\n";

    while (true) {
        displayMenu();
        processInput();
    }
}

void Main::displayMenu() {
    std::cout << "Menu:\n";
    std::cout << "1. Run Plant Farm Simulator\n";
    std::cout << "2. Auto-Run with values = 2\n";
    std::cout << "3. Exit\n";
    std::cout << "Choose an option: ";
}

void Main::processInput() {
    int choice;
    std::cin >> choice;

    // Clear input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Handles choice inputs for terminal UI, exit, error handling
    switch (choice) {
        case 1: {
            TerminalUI terminalUI;
            terminalUI.start();
            break;
        }
        case 2: {
            PlantFarm plantfarm;
            while (!plantfarm.transition(2, 2)) {
                continue;
            }
            break;
        }
        case 3:
            std::cout << "Exiting...\n";
            exit(0);
        default:
            std::cout << "Invalid choice. Please try again.\n";
    }
}

int main() {
    Main main;
    main.start();
    return 0;
}
