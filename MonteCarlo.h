#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <vector>
#include <map>
#include "PlantFarm.h"

using namespace std;

class MonteCarloMDP
{
    // Constants for the class
    const bool DEBUG = false;

    double GAMMA;       // Discount factor (0 < x < 1)
    double EPSILON;     // Greedy epsilon
    int MAX_EPISODES;   // Number of episodes for training

    // State space
    struct State
    {
        int time;
        int water;
        int nitro;
        int status;
        int growth;
        int yield;

        bool operator==(const State& state) const {
            return time == state.time && 
                water == state.water && 
                nitro == state.nitro &&
                status == state.status && 
                growth == state.growth && 
                yield == state.yield;
        }
        
        bool operator<(const State& state) const {
            return tie(time, water, nitro, status, growth, yield) < tie(state.time, state.water, state.nitro, state.status, state.growth, state.yield);
        }
    };

    // Actions
    typedef pair<int, int> Action;

    vector<Action> actions;
    Action best_action;

    float* probabilities;

    // Value function (Q(s,a))
    map<pair<State, Action>, double> value_function;

    // Policy
    map<State, Action> policy;

    // Returns
    map<pair<State, Action>, vector<double>> returns;

    PlantFarm farm;  // Declare farm as a member variable

public:
    MonteCarloMDP();           // Default constructor. Initializes values.

    State* init_current_state(PlantFarm& plant_farm);

    void print_policy();

    int get_best_action(State& state);

    double qvalue(PlantFarm& plant_farm, State& state, Action& action);

    void runEpisode();

    void runMonteCarlo(int episodes);

    void run_with_policy();

    void printBestPolicy();
};

#endif // MONTECARLO_H
