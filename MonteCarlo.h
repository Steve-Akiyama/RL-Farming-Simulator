#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <iostream>
#include <vector>
#include <map>
#include "PlantFarm.h"

using namespace std;

class MonteCarloMDP
{
    const bool DEBUG = false;

    double GAMMA;
    double EPSILON;
    double ALPHA;
    int MAX_EPISODES;
    double EPSILON_DECAY;
    double EPSILON_STOP;

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

    typedef pair<int, int> Action;

    vector<Action> actions;
    Action best_action;

    float* probabilities;

    map<pair<State, Action>, double> value_function;
    map<State, Action> policy;
    map<pair<State, Action>, vector<double>> returns;

    PlantFarm farm;

public:
    MonteCarloMDP();

    State* init_current_state(PlantFarm& plant_farm);
    void print_policy();
    int get_best_action(State& state);
    double qvalue(PlantFarm& plant_farm, State& state, Action& action);
    int get_reward(const State& state);
    void runEpisode();
    void runMonteCarlo(int episodes);
    void run_with_policy();
    void printBestEpisode(const vector<pair<State, Action>>& best_episode);
};

#endif // MONTECARLO_H
