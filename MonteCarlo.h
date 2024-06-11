#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "PlantFarm.h"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <ctime>
#include <utility>  // for std::pair and std::make_pair
#include <tuple>  // for std::tie

using namespace std;

class MonteCarloMDP 
{
    double discountFactor;
    double learningRate;

    struct State 
    {
        int time;   //[0, 10] where 0 is the starting value and 10 is the end of the simulation
        int water;  //[0, WATER_MAX]
        int nitro;  //[0, NITRO_MAX]
        int status; //[0, 5] where 1 is heavy plant decay and 5 is plant flourishing. Growth requires a status of 4 or 5, decay requires a status of 1 or 2.
        int growth; //[0, 4] where 0 is a seedling and 4 is fully grown.
        int yield;  //[0, 2] where 0 is no yield and 2 is max yield. Yield > 0 requires a fully grown plant, and max yield requires max status.

        bool operator<(const State& state) const {
            return tie(time, water, nitro, status, growth, yield) < tie(state.time, state.water, state.nitro, state.status, state.growth, state.yield);
        }
    };

    struct Action 
    {
        int waterInput;
        int nitroInput;

        bool operator<(const Action &other) const {
            if (waterInput != other.waterInput) return waterInput < other.waterInput;
            return nitroInput < other.nitroInput;
        }
    };

    vector<Action> actions;     // Action space
    Action best_action;

    float* probabilities;       // Transition % chances

    // Value function (V(s,a))
    map<pair<State, Action>, double> value_function; // the double is the V of that s,a pair

    // Policy 
    map<State, Action> policy;  // List of mapped state, action pairs
    PlantFarm farm; // Instance of the plant farm problem

    double getReward(const State& state, const Action& action);

public:
    MonteCarloMDP();
    void runEpisode();
    State* init_current_state(PlantFarm& plant_farm);
    int get_best_action(State& state);
    double performAction(const Action& action);
    void updateQValues();
    double qvalue(PlantFarm& plant_farm, State& state, Action& action);
    void runMonteCarlo(int iterations);
    void printQValues();
    void run_with_policy();
    void print_policy();
    void outputQValues(const string& filename);
};

#endif // MONTECARLO_H
