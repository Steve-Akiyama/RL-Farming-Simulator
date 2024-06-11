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
        if (value_function.find(make_pair(*current_state, actions[i])) == value_function.end()) {
            value_function[make_pair(*current_state, actions[i])] = (double)0;
        }
    }

    return current_state;
}

/**
 * print_value_function(struct State& state)
 * Print the value function for all possible actions from a state
 */
void ValueIteration::print_value_function(struct State& state)
{
    // Print state info
    cout << "State: (time: " << state.time
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
            ", V(s,a): " << value_function[pair] << endl;
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
 * qvalue(PlantFarm& plant_farm, struct State& S, pair<int, int>& A)
 * Inputs:
 *  plant_farm - used purely to find the probability function
 *  S - the state from the State, Action pair
 *  A - the action from the State, Action pair
 *
 * Calculates a State, Action pair's Q value, and updates the value function with it
 */
double ValueIteration::qvalue(PlantFarm& plant_farm, struct State& S, pair<int, int>& A)
{
    double Q = 0;

    // Initialize the probability function, 5 possible next states (s')
    map<State, double> P;    // The probability that each state has to be s'

    struct State state1 = S;    // Water + Input - 2, Nitro + Input - 2
    state1.water += A.first - 2;
    state1.nitro += A.second - 2;
    P[state1] = 1.0 - probabilities[1] - probabilities[0];  // 60% by default

    struct State state2 = S;    // Water + Input - 3, Nitro + Input - 2
    state2.water += A.first - 3;
    state2.nitro += A.second - 2;
    P[state2] = probabilities[0] / 2;   // 10% by default

    struct State state3 = S;    // Water + Input - 1, Nitro + Input - 2
    state3.water += A.first - 1;
    state3.nitro += A.second - 2;
    P[state3] = probabilities[0] / 2;   // 10% by default

    struct State state4 = S;    // Water + Input - 2, Nitro + Input - 3
    state4.water += A.first - 2;
    state4.nitro += A.second - 3;
    P[state4] = probabilities[1] / 2;   // 10% by default

    struct State state5 = S;    // Water + Input - 2, Nitro + Input - 1
    state5.water += A.first - 2;
    state5.nitro += A.second - 1;
    P[state5] = probabilities[1] / 2;   // 10% by default

    for (const auto& Sprime : P) {
        struct State curr_state = Sprime.first;  // denotes s'
        double prob = Sprime.second; // denotes the transition probability to s'
        int best_action_id = get_best_action(curr_state);

        Q += prob * (plant_farm.reward() + GAMMA * value_function[make_pair(curr_state, actions[best_action_id])]); // Current equation: sum(T(s,a,s') * V(s'))
    }

    //Q *= GAMMA; // Current equation: gamma * sum(T(s,a,s') * V(s'))
    //Q += plant_farm.reward(); // Current equation: R(s) + gamma * sum(T(s,a,s') * V(s'))

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

        int best_action = 0;
        // double best_action_value = 0.0;

        while (!trial_over) {
            State* S = init_current_state(*plant_farm);
            double Q = 0.0;

            // Iterate over all possible actions for this state
            for (int action_id = 0; action_id < actions.size(); action_id++) {
                Action A = actions[action_id];  // Action to be tested

                // Test an action using a temporary PlantFarm without progressing to the next state on the real PlantFarm
                PlantFarm* temp_farm = new PlantFarm(*plant_farm);
                temp_farm->transition(A.first, A.second);
                cout << "Input {<Water>, <Nitrogen>}: " << "{" << A.first << ", " << A.second << "}" << endl;

                // Calculate the Q-value for the action
                Q = qvalue(*temp_farm, *S, A);

                // Update the value function
                value_function[make_pair(*S, A)] = Q;
            }

            // Update residual
            Action best_A = actions[get_best_action(*S)];
            double residual = fabs(Q - value_function[make_pair(*S, best_A)]);
            cout << "Q: " << Q << endl;
            cout << "V(s'): " << value_function[make_pair(*S, best_A)] << endl;
            cout << "Residual: " << residual << endl;
            if (residual > max_residual) {
                max_residual = residual;
            }

            // Update the policy
            int best_action_id = get_best_action(*S);
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
