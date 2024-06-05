#ifndef MONTECARLO_H
#define MONTECARLO_H

#include "PlantFarm.h"
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <ctime>

using namespace std;

class MonteCarloMDP {

    struct State 
    {
        int time;
        int water;
        int nitrogen;
        int status;
        int growth;
        int yield;

        bool operator<(const State &other) const {
            if (time != other.time) return time < other.time;
            if (water != other.water) return water < other.water;
            if (nitrogen != other.nitrogen) return nitrogen < other.nitrogen;
            if (status != other.status) return status < other.status;
            if (growth != other.growth) return growth < other.growth;
            return yield < other.yield;
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

    map<pair<State, Action>, pair<double, int> > qValues; // Q-values with visit count
    map<pair<State, Action>, vector<double> > returns; // Returns for each state-action pair
    vector<tuple<State, Action, double> > episode; // To store the episode
    PlantFarm farm; // Instance of the plant farm problem

    double getReward(const State& state, const Action& action);

public:
    MonteCarloMDP();
    void runEpisode();
    double performAction(const Action& action);
    void updateQValues();
    void runMonteCarlo(int iterations);
    void printQValues();
};

#endif // MONTECARLO_H

