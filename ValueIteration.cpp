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
        value_function[make_pair(*current_state, actions[i])] = (double)0;
    }

    if (DEBUG) print_value_function(*current_state);

    return current_state;
}

/**
 * print_value_function(struct State& state)
 * Print the value function for all possible actions from a state
 */
void ValueIteration::print_value_function(struct State& state)
{
    // Print state info
    cout << "State: "
        << "(time: " << state.time
        << ", water: " << state.water
        << ", nitrogen: " << state.nitro
        << ", status: " << state.time
        << ", growth: " << state.time
        << ", yield: " << state.time << ")" << endl;

    // Print value function of all actions from states
    cout << "Actions: " << endl;
    for (int i = 0; i < actions.size(); i++)
    {
        cout << "{" << actions[i].first << ", " << actions[i].second << "}"
            << " V(s,a): " << value_function[make_pair(state, actions[i])] << endl;
    }

    cout << endl;

    return;
}

/**
 * get_best_action(struct State& state)
 * Return the action id with the highest V(s,a) for a state.
 * Can access this action from other places with actions[get_best_action(state)].
 * This is basically the program's way of accessing the policy.
 */
int ValueIteration::get_best_action(struct State& state)
{
    int best_action_id = 0;
    double best_action_value = -10000.0;    // Set to very low number (this number can only improve)

    // best_action_id = argmax(actions)
    for (int i = 0; i < actions.size(); i++)
    {
        // Replace best_action_value when a better action is found
        if (best_action_value < value_function[make_pair(state, actions[i])])
        {
            best_action_value = value_function[make_pair(state, actions[i])];
            best_action_id = i;
        }
    }

    return best_action_id;
}

/**
 * VI()
 * Run value iteration learning algorithm
 * Based on the grid_VI_basecode.py homework 1 project
 */
double ValueIteration::qvalue(PlantFarm& plant_farm, struct State& state, pair<int, int>& action)
{
    double Q = 0;

    float* probabilities = plant_farm.getProbabilities();

    int net_water_change;

    vector<pair<int, double>> succ_list = P[s][a]; // Assuming P is a global variable

    for (const auto& succ : succ_list) {
        int succ_state_id = find(states.begin(), states.end(), succ.first) - states.begin(); // denotes s'
        double prob = succ.second; // denotes the transition probability

        // Complete the following line of code to calculate Q-value.
        Q += prob * V[succ_state_id]; // Current equation: sum(T(s,a,s') * V*(s'))
    }

    Q *= gamma; // Current equation: gamma * sum(T(s,a,s') * V*(s'))
    Q += R[s]; // Current equation: R(s) + gamma * sum(T(s,a,s') * V*(s'))

    return Q;
}

/**
 * VI()
 * Run value iteration learning algorithm
 * Based on the grid_VI_basecode.py homework 1 project
 */
void ValueIteration::VI()
{
    // Value Iteration start
    for (int trial = 0; trial < MAX_TRIALS; trial++) {
        PlantFarm* plant_farm = new PlantFarm();    // Fresh new PlantFarm each trial

        double max_residual = 0.0;
        bool trial_over = false;

        while (!trial_over) {
            // struct State* current_state = init_current_state(*plant_farm);

            // Iterate over all possible actions for this state
            for (auto action : actions) {
                PlantFarm* temp_farm = new PlantFarm(*plant_farm);  // Temporary PlantFarm, in order to test an action without progressing to the next state on the real PlantFarm
                trial_over = temp_farm->transition(action.first, action.second);

                cout << "Input {<Water>, <Nitrogen>}: " << "{" << action.first << ", " << action.second << "}" << endl;

                // Calculate the Q-value for the action
                struct State* state = init_current_state(*temp_farm);
                double Q = qvalue(*temp_farm, *state, action);
                

                // To do: Update value function based on the next state
                

                double residual = fabs(value_function[make_pair(*state, action)] - Q);
                max_residual = max(max_residual, residual);

                value_function[make_pair(*state, action)] = Q;
            }

            // Update the policy for the current state
            struct State* state = init_current_state(*plant_farm);
            int best_action_id = get_best_action(*state);
            policy[*state] = actions[best_action_id];

            // Check for convergence
            if (max_residual < EPSILON) {
                break;
            }

            // Perform the best action
            trial_over = plant_farm->transition(actions[best_action_id].first, actions[best_action_id].second);
            cout << "Input {<Water>, <Nitrogen>}: " << "{" << actions[best_action_id].first << ", " << actions[best_action_id].second << "}" << endl;
        }
    }

    return;
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

    // Value Iteration start
    VI();

    // Stop the timer
    clock_t const timer_end = clock();
    clock_t time_elapsed = 1000000 * (timer_end - timer_start) / CLOCKS_PER_SEC;

    return time_elapsed;
}
