#ifndef VALUE_ITERATION_H
#define VALUE_ITERATION_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include "PlantFarm.h"

using namespace std;

class ValueIteration
{
    /**
     * Defines constants for the class. Includes upper bounds, starting values, transition function chances, and decay rates.
     */
    const bool DEBUG = false;        // Set to true to enable debug messages

    const int MAX_TRIALS = 1000;
    const int S_PRIME_POP = 10;

    const double GAMMA = 0.90;      // Discount factor (0 < x < 1)
    const double EPSILON = 20;

    // State space
    struct State
    {
        int time;   //[0, 10] where 0 is the starting value and 10 is the end of the simulation
        int water;  //[0, WATER_MAX]
        int nitro;  //[0, NITRO_MAX]
        int status; //[0, 5] where 1 is heavy plant decay and 5 is plant flourishing. Growth requires a status of 4 or 5, decay requires a status of 1 or 2.
        int growth; //[0, 4] where 0 is a seedling and 4 is fully grown.
        int yield;  //[0, 2] where 0 is no yield and 2 is max yield. Yield > 0 requires a fully grown plant, and max yield requires max status.

        // Adapted from https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
        bool operator<(const State& state) const {
            return tie(time, water, nitro, status, growth, yield) < tie(state.time, state.water, state.nitro, state.status, state.growth, state.yield);
        }
    };

    // Actions
    // Some of the below code adapted from https://www.geeksforgeeks.org/pair-in-cpp-stl/
    typedef pair<int, int> Action;

    vector<Action> actions;     // Action space
    Action best_action;

    float* probabilities;       // Transition % chances

    // Value function (V(s,a))
    map<pair<State, Action>, double> value_function; // the double is the V of that s,a pair

    // Policy 
    map<State, Action> policy;  // List of mapped state, action pairs

public:
    ValueIteration();           // Default constructor. Initializes values.

    State* init_current_state(PlantFarm& plant_farm);

    void print_value_function(State& state);

    void print_policy();

    int get_best_action(State& state);

    double qvalue(PlantFarm& plant_farm, State& state, Action& action);

    void VI();

    void run_with_policy();

    clock_t run();      // Runs the algorithm. This is what should be called from Main.cpp.

private:
};

#endif // VALUE_ITERATION_H
