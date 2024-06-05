#include "Main.h"
#include <iostream>
#include <limits>

using namespace std;

Main::Main()
{
    // Constructor - no initialization needed
}

void Main::printAsciiTitle() {
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
    cout << "3. Run with Value Iteration" << endl;
    cout << "4. Run with Actor-Critic" << endl;
    cout << "5. Run with Actor-Critic (DEBUG MODE)" << endl;
    cout << "6. Exit" << endl;
    cout << "Choose an option: ";
}

int Main::askForEpisodeCount()
{
    cout << "How many episodes would you like to run? (Integer inputs only!)" << endl;

    string str_choice;
    cin >> str_choice;
    
    // Convert string to integer
    int choice = 0;
    try {
        choice = std::stoi(str_choice);
    } catch (std::invalid_argument& e) {
        cout << "Invalid input! Please enter an integer." << endl;
    } catch (std::out_of_range& e) {
        cout << "Input out of range! Please enter a smaller number." << endl;
    }

    // Clear input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Print separator
    cout << "--------------------" << endl << endl;
    
    return choice;
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
    cout << "--------------------" << endl
         << endl;

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
            cout << "<Water> <Nitrogen> Input: " << water_input << " " << nitro_input << endl; // Format for showing the user what the
            continue;
        }
        break;
    }
    case 3:
    {
        ValueIteration valueiter;
        cout << "Running Value Iteration..." << endl;

        break;
    }
    case 4:
    {
        method.setDebug(false);

        vector<int> episodeRewards;
        int totalReward = 0;

        ofstream dataFile("episode_rewards.dat");

        int episodeCount = askForEpisodeCount();

        cout << "Running with Actor-Critic with " << episodeCount << " episodes." << endl;

        for (int i = 0; i < episodeCount; i++) {
            cout << "\n--------\nEpisode: " << i << "\n--------\n";
            int episodeReward = method.runEpisode(); // Assuming method.runEpisode() returns episode reward
            episodeRewards.push_back(episodeReward);
            cout << "\n--------\nEpisode " << i << " concluded. Total reward: " << episodeReward << "\n--------\n";
            dataFile << i << " " << episodeReward << endl;
        }

        dataFile.close();

        // Plotting
        FILE *gnuplotPipe = _popen("gnuplot -persistent", "w");
        fprintf(gnuplotPipe, "plot 'episode_rewards.dat' with lines title 'Episode Rewards'\n");
        fflush(gnuplotPipe);

        break;
    }
    case 5:
    {
        method.setDebug(true);

        vector<int> episodeRewards;
        int totalReward = 0;

        ofstream dataFile("episode_rewards.dat");

        int episodeCount = askForEpisodeCount();

        cout << "Running with Actor-Critic with " << episodeCount << " episodes." << endl;

        for (int i = 0; i < episodeCount; i++) {
            cout << "\n--------\nEpisode: " << i << "\n--------\n";
            int episodeReward = method.runEpisode(); // Assuming method.runEpisode() returns episode reward
            episodeRewards.push_back(episodeReward);
            cout << "\n--------\nEpisode " << i << " concluded. Total reward: " << episodeReward << "\n--------\n";
            dataFile << i << " " << episodeReward << endl;
            std::string input;

            // Wait for the user to press Enter
            std::cout << "\nPress Enter to continue...\n";
            std::getline(std::cin, input);
        }

        dataFile.close();

        // Plotting
        FILE *gnuplotPipe = _popen("gnuplot -persistent", "w");
        fprintf(gnuplotPipe, "plot 'episode_rewards.dat' with lines title 'Episode Rewards'\n");
        fflush(gnuplotPipe);

        break;
    }
    case 6:
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
