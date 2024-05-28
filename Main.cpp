#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include "Main.h"
#include "TerminalUI.h"

using namespace std;

Main::Main()
{
    // Constructor - no initialization needed
}

void Main::printAsciiTitle()
{
    // I promise it doesn't look this weird when you run it
    cout << "______ _             _    ______                   " << endl;
    cout << "| ___ \\ |           | |   |  ___|                  " << endl;
    cout << "| |_/ / | __ _ _ __ | |_  | |_ __ _ _ __ _ __ ___  " << endl;
    cout << "|  __/| |/ _` | '_ \\| __| |  _/ _` | '__| '_ ` _ \\ " << endl;
    cout << "| |   | | (_| | | | | |_  | || (_| | |  | | | | | |" << endl;
    cout << "\\_|   |_|\\__,_|_| |_|\\__| \\_| \\__,_|_|  |_| |_| |_|" << endl;
    cout << endl;
    cout << " _____ _                 _       _ " << endl;
    cout << "/  ___(_)               | |     | |" << endl;
    cout << "\\ `--. _ _ __ ___  _   _| | __ _| |_ ___  _ __" << endl;
    cout << " `--. \\ | '_ ` _ \\| | | | |/ _` | __/ _ \\| '__|" << endl;
    cout << "/\\__/ / | | | | | | |_| | | (_| | || (_) | |" << endl;
    cout << "\\____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__\\___/|_|" << endl;
    return;
}

void Main::start()
{
    printAsciiTitle();

    while (true)
    {
        displayMenu();
        processInput();
    }
}

void Main::displayMenu()
{
    cout << "Menu:" << endl;
    cout << "1. Run Plant Farm Simulator with manual inputs" << endl;
    cout << "2. Auto-Run Plant Farm Simulator with auto-input values: \"2 2\"" << endl;
    cout << "3. Run Plant Farm Simulator with Value Iteration" << endl;
    cout << "4. Exit" << endl;
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

    // Print separator
    cout << "--------------------" << endl << endl;

    // Handles choice inputs for terminal UI, exit, error handling
    switch (choice)
    {
    case 1:
    {
        cout << "Running Manual Input..." << endl;
        TerminalUI terminalUI;
        terminalUI.start();
        break;
    }
    case 2:
    {
        cout << "Running Auto-Input (2 2)..." << endl;
        PlantFarm plantfarm;
        int water_input = 2;
        int nitro_input = 2;
        while (!plantfarm.transition(water_input, nitro_input))
        {
            cout << "<Water> <Nitrogen> Input: " << water_input << " " << nitro_input << endl; // Format for showing the user what the
            continue;
        }
        break;
    }
    case 3:
    {
        cout << "Running Value Iteration..." << endl;
        ValueIteration valueiter;
        clock_t time_elapsed = valueiter.run(); // Run value iteration
        cout << fixed << setprecision(6) 
            << "Time Elapsed: " << ((double)(time_elapsed) / 1000000) << " seconds" << endl;
        break;
    }
    case 4:
    {
        cout << "Exiting..." << endl
             << endl;
        exit(0);
    }
    default:
    {
        cout << "Invalid choice. Please try again." << endl;
        break;
    }
    }
    cout << endl << "--------------------" << endl;
}

int main()
{
    Main main;
    main.start();
    return 0;
}
