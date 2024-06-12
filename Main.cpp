#include "Main.h"
#include "PlantFarm.h"
#include "MonteCarlo.h"

using namespace std;

Main::Main() {
    // Constructor - no initialization needed
}

void Main::printAsciiTitle() {
    std::cout << "______ _             _    ______                   " << endl;
    std::cout << "| ___ \\ |           | |   |  ___|                  " << endl;
    std::cout << "| |_/ / | __ _ _ __ | |_  | |_ __ _ _ __ _ __ ___  " << endl;
    std::cout << "|  __/| |/ _` | '_ \\| __| |  _/ _` | '__| '_ ` _ \\ " << endl;
    std::cout << "| |   | | (_| | | | | |_  | || (_| | |  | | | | | |" << endl;
    std::cout << "\\_|   |_|\\__,_|_| |_|\\__| \\_| \\__,_|_|  |_| |_| |_|" << endl;
    std::cout << endl;
    std::cout << " _____ _                 _       _ " << endl;
    std::cout << "/  ___(_)               | |     | |" << endl;
    std::cout << "\\ `--. _ _ __ ___  _   _| | __ _| |_ ___  _ __" << endl;
    std::cout << " `--. \\ | '_ ` _ \\| | | | |/ _` | __/ _ \\| '__|" << endl;
    std::cout << "/\\__/ / | | | | | | |_| | | (_| | || (_) | |" << endl;
    std::cout << "\\____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__\\___/|_|" << endl;
    return;
}

void Main::start() {
    printAsciiTitle();

    while (true) {
        displayMenu();
        processInput();
    }
}

void Main::displayMenu() {
    std::cout << "Menu:" << endl;
    std::cout << "1. Run Plant Farm Simulator with manual inputs" << endl;
    std::cout << "2. Auto-Run Plant Farm Simulator with auto-input values: \"2 2\"" << endl;
    std::cout << "3. Run with Value Iteration" << endl;
    std::cout << "4. Run with Monte Carlo" << endl;
    std::cout << "5. Exit" << endl;
    std::cout << "Choose an option: ";
}

int Main::askForEpisodeCount() {
    std::cout << "How many episodes would you like to run? (Integer inputs only!)" << endl;

    string str_choice;
    cin >> str_choice;

    // Convert string to integer
    int choice = 0;
    try {
        choice = std::stoi(str_choice);
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid input! Please enter an integer." << endl;
    } catch (std::out_of_range& e) {
        std::cout << "Input out of range! Please enter a smaller number." << endl;
    }

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Print separator
    std::cout << "--------------------" << endl << endl;

    return choice;
}

void Main::processInput() {
    // Take user input
    char str_choice;
    cin >> str_choice;
    int choice = str_choice - '0'; // Input is converted from char to int

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Print separator
    std::cout << "--------------------" << endl << endl;

    // Handles choice inputs for terminal UI, exit, error handling
    switch (choice) {
        case 1: {
            PlantFarm plantfarm;
            while (true) {
                int water_input, nitro_input;
                std::cout << "Enter water and nitrogen inputs: ";
                std::cin >> water_input >> nitro_input;
                if (plantfarm.transition(water_input, nitro_input)) {
                    break;
                }
            }
            break;
        }
        case 2: {
            PlantFarm plantfarm;
            int water_input = 2;
            int nitro_input = 2;
            while (!plantfarm.transition(water_input, nitro_input)) {
                std::cout << "<Water> <Nitrogen> Input: " << water_input << " " << nitro_input << endl;
            }
            break;
        }
        case 3: {
            // ValueIteration valueiter;
            std::cout << "Running Value Iteration..." << endl;
            // valueiter.run();
            break;
        }
        case 4: {
            MonteCarloMDP montecarlo;
            int episodes = askForEpisodeCount();
            montecarlo.runMonteCarlo(episodes);
            montecarlo.printQValues();
            std::cout << "Running Every Visit Monte Carlo..." << endl;
            break;
        }
        case 5: {
            std::cout << "Exiting..." << endl << endl;
            exit(0);
        }
        default: {
            std::cout << "Invalid choice. Please try again." << endl;
            break;
        }
    }
    std::cout << endl << "--------------------" << endl;
}

int main() {
    Main main;
    main.start();
    return 0;
}
