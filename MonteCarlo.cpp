#include "MonteCarlo.h"
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>
#include <ctime>
#include <limits>
#include <cstdlib>

using namespace std;

MonteCarloMDP::MonteCarloMDP() {
    srand(time(nullptr)); // Seed for random number generation
}

void MonteCarloMDP::runEpisode() {
    episode.clear();
    farm.reset();
    while (farm.getTime() < farm.getFinalTime()) {
        State currentState = {farm.getTime(), farm.getWater(), farm.getNitro(), farm.getStatus(), farm.getGrowth(), farm.getYield()};
        Action action = {rand() % 5, rand() % 5};
        double reward = performAction(action);
        episode.push_back(make_tuple(currentState, action, reward));
        
        farm.printStatus();
        if (reward == -100) {
            break;
        }
    }
}

double MonteCarloMDP::performAction(const Action& action) {
    bool continueExperiment = farm.transition(action.waterInput, action.nitroInput);
    State newState = {farm.getTime(), farm.getWater(), farm.getNitro(), farm.getStatus(), farm.getGrowth(), farm.getYield()};
    double reward = getReward(newState, action);
    return reward;
}

void MonteCarloMDP::updateQValues() {
    double totalReward = 0;
    reverse(episode.begin(), episode.end());
    for (const auto &step : episode) {
        State state;
        Action action;
        double reward;
        tie(state, action, reward) = step;
        totalReward += reward;
        auto qValueKey = make_pair(state, action);
        returns[qValueKey].push_back(totalReward);
        qValues[qValueKey].first += totalReward;
        qValues[qValueKey].second += 1;
    }
}

void MonteCarloMDP::runMonteCarlo(int iterations = 5) {
    for (int i = 0; i < iterations; ++i) {
        runEpisode();
        updateQValues();
    }
}

void MonteCarloMDP::printQValues() {
    for (const auto &entry : qValues) {
        const auto &stateAction = entry.first;
        const auto &qValuePair = entry.second;
        const auto &state = stateAction.first;
        const auto &action = stateAction.second;
        double qValue = qValuePair.first / qValuePair.second;
        cout << "State: [Time: " << state.time << ", Water: " << state.water << ", Nitrogen: " << state.nitrogen
             << ", Status: " << state.status << ", Growth: " << state.growth << ", Yield: " << state.yield
             << "] Action: [WaterInput: " << action.waterInput << ", NitroInput: " << action.nitroInput
             << "] Q-Value: " << qValue << "\n";
    }
}

double MonteCarloMDP::getReward(const State& state, const Action& action) {
    double reward = 0;
    int status = state.status;
    int growth = state.growth;
    int yield = state.yield;

    if (status == 1) {
        reward = -100;
    } else if (status == 4 || status == 5) {
        reward = status + growth + yield * 4;
    }

    if (state.water >= 1 && state.water <= 3) {
        reward += 1;
    } else if (state.water == 0 || state.water == 4) {
        reward -= 2;
    }

    if (state.nitrogen >= 2 && state.nitrogen <= 3) {
        reward += 1;
    } else if (state.nitrogen == 4) {
        reward -= 1;
    } else if (state.nitrogen == 1 && state.status >= 4) {
        reward -= 1;
    } else if (state.nitrogen == 0 && state.status >= 4) {
        reward = 3;
    }

    return reward;
}
