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

    // Variables
    PlantFarm *plant_farm;

    const double GAMMA = 0.9;       // Discount factor (0 < x < 1)
    const double EPSILON = 0.01;    // Convergence threshold (0 < x < 1, lower value = higher chance of optimal policy, but longer runtime)

    // State space
    struct State
    {
        int time;   //[0, 10] where 0 is the starting value and 10 is the end of the simulation
        int water;  //[0, WATER_MAX] where 0 is the lowest possible value and WATER_MAX is the highest possible value
        int nitro;  //[0, NITRO_MAX] where 0 is the lowest possible value and NITRO_MAX is the highest possible value
        int status; //[0, 5] where 1 is heavy plant decay and 5 is plant flourishing. Growth requires a status of 4 or 5, decay requires a status of 1 or 2.
        int growth; //[0, 4] where 0 is a seedling and 4 is fully grown.
        int yield;  //[0, 2] where 0 is no yield and 2 is max yield. Yield > 0 requires a fully grown plant, and max yield requires max status.

        // Adapted from https://stackoverflow.com/questions/3882467/defining-operator-for-a-struct
        bool operator<(const State& state) const {
            return tie(time, water, nitro, status, growth, yield) < tie(state.time, state.water, state.nitro, state.status, state.growth, state.yield);
        }
    };

    // Action space
    typedef pair<int, int> Action;
    vector<Action> actions;

    // Value function
    map<State, double> valueFunction;

    // Policy
    map<State, Action> policy;

public:
    ValueIteration();   // Default constructor. Initializes values.

    void setup(PlantFarm &farm);

    clock_t run();     // Runs the algorithm. This is what should be called from Main.cpp.

private:
};

#endif // VALUE_ITERATION_H
