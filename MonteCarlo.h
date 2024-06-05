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

struct State {
    int time;
    int water;
    int nitrogen;
    int status;
    int growth;
    int yield;

    bool operator<(const State &other) const {
        return std::tie(time, water, nitrogen, status, growth, yield) < std::tie(other.time, other.water, other.nitrogen, other.status, other.growth, other.yield);
    }
};

struct Action {
    int waterInput;
    int nitroInput;

    bool operator<(const Action &other) const {
        return std::tie(waterInput, nitroInput) < std::tie(other.waterInput, other.nitroInput);
    }
};

class MonteCarloMDP {
private:
    map<pair<State, Action>, pair<double, int>> qValues; // Q-values with visit count
    map<pair<State, Action>, vector<double>> returns; // Returns for each state-action pair
    vector<tuple<State, Action, double>> episode; // To store the episode
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
