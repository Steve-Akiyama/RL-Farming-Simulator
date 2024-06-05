#include "ActorCritic.h"

ActorCritic::ActorCritic() {
    //Iniitalizes a bunch of values
    initValueFunct();
    initTheta();

    // Seed random number generator
    srand(time(0));

    gamma = 0.85;
    epsilon = 1;
    alpha = 0.15;
}

void ActorCritic::runActorCritic(int episodeCount) {

        std::vector<int> episodeRewards;
        int totalReward = 0;

        std::ofstream dataFile("episode_rewards.dat");

        std::cout << "Running with Actor-Critic with " << episodeCount << " episodes." << std::endl;

        for (int i = 1; i <= episodeCount; i++) {
            if (i == episodeCount) {
                debug = true;
            }

            std::cout << "\n--------\nEpisode: " << i << "\n--------\n";
            int episodeReward = runEpisode(); // Assuming method.runEpisode() returns episode reward
            episodeRewards.push_back(episodeReward);
            std::cout << "\n--------\nEpisode " << i << " concluded. Total reward: " << episodeReward << "\n--------\n";
            dataFile << i << " " << episodeReward << std::endl;
            std::string input;

            double ep = getEpsilon();
            setEpsilon(ep * 0.95);

            if (debug) {
                // Wait for the user to press Enter
                std::cout << "\nPress Enter to continue...\n";
                std::getline(std::cin, input);
            }
        }

        dataFile.close();

        // Plotting
        FILE *gnuplotPipe = _popen("gnuplot -persistent", "w");
        fprintf(gnuplotPipe, "plot 'episode_rewards.dat' with lines title 'Episode Rewards'\n");
        fflush(gnuplotPipe);
}

int ActorCritic::runEpisode() {

    //This defines the maximum number of loops the simulation can run for (Used in array size)
    const int maxLoops = 11;

    //This array stores the rewards over an episode
    int rewards[maxLoops];

    //Stores if the sim is complete
    bool complete = false;

    int currentWater;
    int currentNitro;
    int * action = new int[2];
    int newWater;
    int newNitro;
    int reward;
    double TD;
    int totalReward = 0;

    farm.printStatus();

    //Episode Loop: Runs as long as transition doesn't return 0 (Returns 0 when plant is dead or time is complete)
    while (!complete) {
        //rewards[farm.getTime()] = farm.reward(); //Stores away the reward at the current time

        // Obtain current state values
        currentWater = farm.getWater();
        currentNitro = farm.getNitro();

        // Actor selects action
        getAction(action);

        //Print action
        std::cout << "INPUT:  W: " << action[0] << " | N: " << action[0] << std::endl;

        //Take action
        complete = farm.transition(action[0], action[1]);

        //Store away the reward
        reward = farm.reward();
        totalReward += farm.reward();

        // Obtain new state values, for TD error calculation
        newWater = farm.getWater();
        newNitro = farm.getNitro();

        // Calculate TD Error
        TD = TDError(currentWater, currentNitro, reward, newWater, newNitro);

        if (debug) {
            std::cout << "\nTD Error: " << TD;
        }

        // Update critic's value function
        updateValue(currentWater, currentNitro, TD);

        // Update actor's policy
        updatePolicy(currentWater, currentNitro, action[0], action[1], TD);

    } 

    if (debug) {
        std::cout 
        << "\n\nEPSILON (Exploration rate): " << epsilon
        << "\nGAMMA (Future reward decay rate): " << gamma 
        << "\nALPHA (Learning rate): " << alpha;
    }

    
    if (debug) {
        std::cout << "\n\nValue Function (V) after episode:\n" << std::endl;
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                std::cout << V[i * 5 + j] << " ";
            }
            std::cout << std::endl << std::endl;
        }

        // Print theta as a table after each episode
        std::cout << "Policy (Theta) after episode:" << std::endl << std::endl;
        // Print column labels
        std::cout << "      | W0N0 | W0N1 | W0N2 | W0N3 | W0N4 | W1N0 | W1N1 | W1N2 | W1N3 | W1N4 | W2N0 | W2N1 | W2N2 | W2N3 | W2N4 | W3N0 | W3N1 | W3N2 | W3N3 | W3N4 | W4N0 | W4N1 | W4N2 | W4N3 | W4N4 |" << std::endl;

        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 5; ++j) {
                std::cout << "(" << i << "," << j << ") |";
                for (int k = 0; k < 5; ++k) {
                    for (int l = 0; l < 5; ++l) {
                        // Format output to be same length numbers rounded to 2 decimal places
                        std::cout << std::fixed << std::setprecision(2) << std::setw(6) << std::internal << std::right << theta[i * 125 + j * 25 + k * 5 + l] << "|";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
    
    //Post-loop cleanup
    farm.reset();
    complete = false;

    return totalReward;
}

void ActorCritic::initValueFunct() {
    V = new double[5 * 5];
    for (int i = 0; i < 25; i++) {
        V[i] = 0;
    }
}

void ActorCritic::initTheta() {
    theta = new double[5 * 5 * 5 * 5];
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            for (int k = 0; k < 5; ++k) {
                for (int l = 0; l < 5; ++l) {
                    theta[i * 125 + j * 25 + k * 5 + l] = smallRandomValue();
                }
            }
        }
    }

}

void ActorCritic::updatePolicy(int currentWater, int currentNitro, int waterInput, int nitroInput, double TD) {
    theta[currentWater * 125 + currentNitro * 25 + waterInput * 5 + nitroInput] += alpha * TD;
}

//Updates the critic using the current state, reward, and future state values
double ActorCritic::TDError(int stateWater, int stateNitro, int reward, int nextWater, int nextNitro) {
    // Calculate TD error
    double delta = reward + gamma * V[nextWater * 5 + nextNitro] - V[stateWater * 5 + stateNitro];

    
    
    return delta;
}

void ActorCritic::updateValue(int stateWater, int stateNitro, double TD) {
    // Update value function
    V[stateWater * 5 + stateNitro] += alpha * TD;


    if (debug) {
        std::cout << "\n\nUPDATE TO V: " << alpha * TD << "\nINDEX: " << stateWater * 5 + stateNitro << "\nALPHA: " << alpha << "\n\n";
    }
}

void ActorCritic::getAction(int * action) {
    double randomValue = (double)rand() / RAND_MAX; // Generate a random value between 0 and 1
    
    //If the random value is larger than epsilon, chooses optimally
    if (randomValue > epsilon) {
        // Exploitation: Choose the action that maximizes the expected reward
        int bestWater = 0;
        int bestNitro = 0;
        double bestActionValue = -100000;

        //Loops through action space
        for (int water = 0; water <= 4; ++water) {
            for (int nitro = 0; nitro <= 4; ++nitro) {

                double actionValue = theta[farm.getWater() * 125 + farm.getNitro() * 25 + water * 5 + nitro]; // Assuming theta is 4-dimensional
                
                //If this is the new best action
                if (actionValue > bestActionValue) {
                    action[0] = water;
                    action[1] = nitro;
                    bestActionValue = actionValue;
                }

            }
        }

        return;
    } else {
        // If it is smaller than epsilon, choose a random action (probably bad)
        // Exploration: Choose a random action
        action[0] = rand() % 5;
        action[1] = rand() % 5;
        return; // Randomly choose an action between 0 and 4 (inclusive)
    }
}

// Function to generate a small random value
double ActorCritic::smallRandomValue() {

    // Generate random value in the range [-0.1, 0.1]
    return ((double)rand() / RAND_MAX) * 0.2 - 0.1;
}

void ActorCritic::setDebug(bool val) {
    debug = val;
}

void ActorCritic::setEpsilon(double newEpsilon) {
    epsilon = newEpsilon;
}

void ActorCritic::setAlpha(double newAlpha) {
    alpha = newAlpha;
}

double ActorCritic::getAlpha() {
    return alpha;
}

double ActorCritic::getEpsilon() {
    return epsilon;
}