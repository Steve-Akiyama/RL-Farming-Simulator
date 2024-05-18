#include "Main.h"
#include "TerminalUI.h"
#include <iostream>
#include <limits>

using namespace std;

Main::Main()
{
    // Constructor - no initialization needed
}

void Main::start()
{
    cout << "Welcome to the Plant Farm Simulator!\n\n";

    while (true)
    {
        displayMenu();
        processInput();
    }
}

void Main::displayMenu()
{
    cout << "Menu:\n";
    cout << "1. Run Plant Farm Simulator with manual inputs\n";
    cout << "2. Auto-Run Plant Farm Simulator with auto-input values: \"2 2\"\n";
    cout << "3. Run Plant Farm Simulator with Value Iteration\n";
    cout << "4. Exit\n";
    cout << "Choose an option: ";
}

void Main::processInput()
{
    // Take user input
    char *str_choice = new char[1];
    cin >> str_choice;
    int choice = (int)(str_choice[0]) - 48; // Input is converted from string to int so that the program will continue to work even if the user enters a non-number

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Handles choice inputs for terminal UI, exit, error handling
    switch (choice)
    {
    case 1:
    {
        TerminalUI terminalUI;
        terminalUI.start();
        break;
    }
    case 2:
    {
        PlantFarm plantfarm;
        int water_input = 2;
        int nitro_input = 2;
        while (!plantfarm.transition(water_input, nitro_input))
        {
            cout << "<Water> <Nitrogen> Input: " << water_input << " " << nitro_input << "\n"; // Format for showing the user what the
            continue;
        }
        break;
    }
    case 3:
    {
        ValueIteration valueiter;
        cout << "Running Value Iteration...\n";
    
        break;
    }
    case 4:
    {
        cout << "Exiting...\n";
        exit(0);
    }
    default:
    {
        cout << "\nInvalid choice. Please try again.\n";
        break;
    }
    }
}

int main()
{
    Main main;
    main.start();
    return 0;
}
