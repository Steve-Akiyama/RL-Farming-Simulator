#include <ctime>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
#include "ValueIteration.h"

using namespace std;

/**
 * Constructor
 * Some of the code in this file is adapted from the grid_VI_basecode.py file
 * provided as part of homework assignment 1 of this class (CS 499)
 */

ValueIteration::ValueIteration()
{
    // Initializes varialbes
    actions =
    {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},
        {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4},
        {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}
    };

    PlantFarm plant_farm;
    probabilities = plant_farm.getProbabilities();
}

/**
 * setup(PlantFarm &farm)
 * Set up the first state so that the algorithm can begin
 */
struct ValueIteration::State* ValueIteration::init_current_state(PlantFarm& plant_farm)
{
    // Setup state by getting the initial values from plant_farm
    State* current_state = new State
    {
        plant_farm.getTime(),
        plant_farm.getWater(),
        plant_farm.getNitro(),
        plant_farm.getStatus(),
        plant_farm.getGrowth(),
        plant_farm.getYield()
    };
    // Set V(s,a) for all actions in state to 0
    for (int i = 0; i < actions.size(); i++)
    {
        if (value_function.find(*current_state) == value_function.end()) {
            value_function[*current_state] = 0.0;
        }
    }

    return current_state;
}

/**
 * print_state_info(struct State& state)
 * Print the info about a state
 */
void ValueIteration::print_state_info(struct State& state)
{
    cout << "State: (time: " << state.time
        << ", water: " << state.water
        << ", nitrogen: " << state.nitro
        << ", status: " << state.time
        << ", growth: " << state.time
        << ", yield: " << state.time << ")" << endl;

    return;
}

/**
 * print_policy()
 * Print the full policy
 */
void ValueIteration::print_policy()
{
    // Print policy
    for (auto const& pair : policy)
    {
        cout << "State: {" <<
            "T: " << pair.first.time << " | " <<
            "Wat: " << pair.first.water << " | " <<
            "Nit: " << pair.first.nitro << " | " <<
            "Sta: " << pair.first.status << " | " <<
            "Grw: " << pair.first.growth << " | " <<
            "Yld: " << pair.first.yield <<
            "} Action: {" << pair.second.first << ", " << pair.second.second << "}" <<
            ", V(s,a): " << value_function[pair.first] << endl;
    }

    // Print reward data
    cout << "Rewards Data: " << endl;

    for (int i = 0; i < MAX_TRIALS; i++)
    {
        cout << rewards_data[i] << ", ";
    }

    // Print residual data
    cout << endl << endl << "Residual Data: " << endl;

    for (int i = 0; i < MAX_TRIALS; i++)
    {
        cout << max_residual_data[i] << ", ";
    }

    cout << endl << endl;

}

/**
 * qvalue(PlantFarm& plant_farm, struct State& S, pair<int, int>& A)
 * Inputs:
 *  plant_farm - used purely to find the probability function
 *  S - the state from the State, Action pair
 *  A - the action from the State, Action pair
 *
 * Calculates a State, Action pair's Q value, and updates the value function with it
 */
double ValueIteration::qvalue(PlantFarm& plant_farm, struct State& S, int A_id)
{
    double Q = 0;
    Action A = actions[A_id];

    // Initialize the probability function, 5 possible next states (s')
    map<State, pair<double, int>> P;    // The probability that each state has to be s'. double = prob, int = reward

    if (plant_farm.getTime() < 10)
    {
        // Get r(s,a,s') by seeing what s' occur with s and a over some trials.
        const int TESTS = 10;
        const double EQUAL_SLICE = 1.0 / TESTS;
        for (int i = 0; i < TESTS; i++) 
        {
            // Perform the action on the state
            PlantFarm* temp_farm = new PlantFarm(plant_farm);
            temp_farm->transition(A.first, A.second);
            State* temp_S = init_current_state(*temp_farm);

            // Record the result
            double old_prob = P[*temp_S].first;
            P[*temp_S] = make_pair(old_prob + EQUAL_SLICE, temp_farm->reward());
        }

        // Calculate Q value
        for (const auto& prob_function : P)
        {
            struct State S_prime = prob_function.first; // denotes s'
            double prob = prob_function.second.first;   // denotes the transition probability to s'
            int reward = prob_function.second.second;   // r(s,a,s')

            Q += prob * (reward + GAMMA * value_function[S_prime]); // Current equation: sum(P(s'|s)[r(s,a,s') + gamma*V(s')])
        }
    }
    else
    {
        // If time = 10, there is no next state (s'), so Q is just the reward
        Q = plant_farm.reward();
    }

    return Q;
}

/**
 * VI()
 * Run value iteration learning algorithm
 * Based on the grid_VI_basecode.py homework 1 project
 */
void ValueIteration::VI()
{
    // Setup rewards_data
    rewards_data = new int[MAX_TRIALS + 1];
    max_residual_data = new double[MAX_TRIALS + 1];

    // Value Iteration start
    for (int trial = 0; trial < MAX_TRIALS; trial++) {
        PlantFarm* plant_farm = new PlantFarm();    // Fresh new PlantFarm each trial

        double max_residual = 0.0;
        bool trial_over = false;

        int best_action_id = 0;
        double best_Q = -10000.0;

        while (!trial_over) {
            State* S = init_current_state(*plant_farm);

            // Iterate over all possible actions for this state
            for (int action_id = 0; action_id < actions.size(); action_id++) {
                Action A = actions[action_id];  // Action to be tested

                // Test an action using a temporary PlantFarm without progressing to the next state on the real PlantFarm
                PlantFarm* temp_farm = new PlantFarm(*plant_farm);
                temp_farm->transition(A.first, A.second);
                cout << "Input {<Water>, <Nitrogen>}: " << "{" << A.first << ", " << A.second << "}" << endl;

                // Calculate the Q-value for the action
                double Q = qvalue(*temp_farm, *S, action_id);

                // Update best_action_id and best_Q
                if (Q > best_Q) {
                    best_action_id = action_id;
                    best_Q = Q;
                }
            }

            // Update residual
            Action best_A = actions[best_action_id];
            double residual = fabs(best_Q - value_function[*S]);
            if (residual > max_residual) {
                max_residual = residual;
            }

            // Update the value function
            value_function[*S] = best_Q;

            // Update the policy
            policy[*S] = actions[best_action_id];

            // Perform the best action
            trial_over = plant_farm->transition(actions[best_action_id].first, actions[best_action_id].second);
            cout << "Input {<Water>, <Nitrogen>}: " << "{" << actions[best_action_id].first << ", " << actions[best_action_id].second << "}" << endl;
        }

        rewards_data[trial] = plant_farm->reward();     // Now that the trial ended, log final reward data
        max_residual_data[trial] = max_residual;

        if (max_residual < EPSILON) {
            cout << "Convergence!" << endl;
            break;
        }
    }

    return;
}

/**
 * run_with_policy()
 * Runs a plant farm using the current policy
 */
void ValueIteration::run_with_policy()
{
    PlantFarm* plant_farm = new PlantFarm();
    bool trial_over = false;

    while (!trial_over) {
        State* S = init_current_state(*plant_farm);
        Action A = policy[*S];
        int water_input = A.first;
        int nitro_input = A.second;

        trial_over = plant_farm->transition(water_input, nitro_input);
        cout << "Input {<Water>, <Nitrogen>}: " << "{" << A.first << ", " << A.second << "}" << endl << endl;
    }
}
/**
 * query_max_trials()
 * Asks the user how many max trials the algorithm should use
 */
int query_max_trials() {
    char* str_user_input = new char[16];

    cout << "How many episodes would you like to run? (Integer inputs only!)" << endl;

    cin >> str_user_input;
    cin.ignore();

    // Prevent a crash if the user enters an invalid input
    if (cin.fail())
    {
        cin.clear();
        return 1;
    }
    else {
        int user_input = atoi(str_user_input);
        return user_input;
    }

    return 1;
}

/**
 * run()
 * This is the "main" function for value iteration.
 * <ValueIteration object>.run() should be called in main.cpp.
 *
 * Timer code adapted from https://stackoverflow.com/questions/2297829/what-is-the-best-way-to-time-how-long-functions-take-in-c
 */
clock_t ValueIteration::run()
{
    // Start the timer
    clock_t const timer_start = clock();

    // Query max trials
    MAX_TRIALS = query_max_trials();

    // Value Iteration start
    VI();

    // Show the (hopefully) optimal policy
    cout << endl << "RUNNING WITH POLICY: " << endl << endl;
    run_with_policy();

    // Print the policy
    cout << endl << "COMPLETE POLICY: " << endl << endl;
    print_policy();

    // Stop the timer
    clock_t const timer_end = clock();
    clock_t time_elapsed = 1000000 * (timer_end - timer_start) / CLOCKS_PER_SEC;

    return time_elapsed;
}
