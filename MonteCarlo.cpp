To adapt your existing C++ code to use a header file, we will separate the class and struct declarations into a header file (`PlantFarm.h`) and keep the definitions in the source file (`monteCarloPF.cpp`). Here's how you can structure the files:

### PlantFarm.h

```cpp
#ifndef PLANTFARM_H
#define PLANTFARM_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <ctime>
#include <tuple>

using namespace std;

class PlantFarm {
    const int TIME_FINAL = 10;
    const int WATER_MAX = 4;
    const int NITRO_MAX = 4;
    const int STATUS_MAX = 5;
    const int GROWTH_MAX = 4;
    const int YIELD_MAX = 2;

    const int WATER_START = 3;
    const int NITRO_START = 3;
    const int STATUS_START = 3;

    const int HIDDEN_STATUS = 2;
    const float WATER_CHANCE = 0.2;
    const float NITRO_CHANCE = 0.2;
    const int WATER_DECAY = 2;
    const int NITRO_DECAY = 2;

    int _time; 
    int _water;
    int _nitro;
    int _status;
    int _growth;
    int _yield;

public:
    PlantFarm();

    int getTime();
    int getWater();
    int getNitro();
    int getStatus();
    int getGrowth();
    int getYield();

    void printStatus();
    bool transition(int waterInput, int nitroInput);
    void reset();

private:
    void waterChange(int waterInput);
    void nitroChange(int nitroInput);
    void statusUpdate();
    void growthUpdate();
    void yieldUpdate();
};

struct State {
    int time;
    int water;
    int nitrogen;
    int status;
    int growth;
    int yield;

    bool operator<(const State &other) const;
};

struct Action {
    int waterInput;
    int nitroInput;

    bool operator<(const Action &other) const;
};

class MonteCarloMDP {
private:
    map<pair<State, Action>, pair<double, int>> qValues;
    map<pair<State, Action>, vector<double>> returns;
    vector<tuple<State, Action, double>> episode;
    PlantFarm farm;

public:
    MonteCarloMDP();
    void generateEpisode();
    double performAction(Action action);
    void updateQValues();
    void run(int iterations);
    void printQValues();
};

#endif // PLANTFARM_H
```

### monteCarloPF.cpp

```cpp
#include "PlantFarm.h"

PlantFarm::PlantFarm() : _time(0), _water(WATER_START), _nitro(NITRO_START), _status(STATUS_START), _growth(0), _yield(0) {}

int PlantFarm::getTime() { return _time; }
int PlantFarm::getWater() { return _water; }
int PlantFarm::getNitro() { return _nitro; }
int PlantFarm::getStatus() { return _status; }
int PlantFarm::getGrowth() { return _growth; }
int PlantFarm::getYield() { return _yield; }

void PlantFarm::printStatus() {
    cout << 
    "\nTime:     " << getTime() <<
    "\nWater:    " << getWater() <<
    "\nNitrogen: " << getNitro() <<
    "\nStatus:   " << getStatus() <<
    "\nGrowth:   " << getGrowth() <<
    "\nYield:    " << getYield() <<
    "\n";
}

bool PlantFarm::transition(int waterInput, int nitroInput) {
    waterChange(waterInput);
    nitroChange(nitroInput);
    statusUpdate();
    growthUpdate();
    yieldUpdate();
    printStatus();
    _time += 1;
    return _time < TIME_FINAL;
}

void PlantFarm::reset() {
    _time = 0;
    _water = WATER_START;
    _nitro = NITRO_START;
    _status = STATUS_START;
    _growth = 0;
    _yield = 0;
}

void PlantFarm::waterChange(int waterInput) {
    _water += waterInput;
    _water -= WATER_DECAY;
    int random_number = rand() % 100;
    if (random_number < (100 * WATER_CHANCE)) {
        _water += (random_number % 2 == 0) ? 1 : -1;
    }
    _water = min(max(_water, 0), WATER_MAX);
}

void PlantFarm::nitroChange(int nitroInput) {
    _nitro += nitroInput;
    _nitro -= NITRO_DECAY;
    int random_number = rand() % 100;
    if (random_number < (100 * NITRO_CHANCE)) {
        _nitro += (random_number % 2 == 0) ? 1 : -1;
    }
    _nitro = min(max(_nitro, 0), NITRO_MAX);
}

void PlantFarm::statusUpdate() {
    if (_water == 0 || _water == WATER_MAX) {
        _status -= 2;
    } else {
        _status += 1;
    }
    if (_nitro == 4) {
        _status -= 1;
    } else if (_nitro == 1 && _status > 3) {
        _status -= 1;
    } else if (_nitro == 0 && _status > 3) {
        _status = 3;
    } else {
        _status += 1;
    }
    _status = min(max(_status, 0), STATUS_MAX);
}

void PlantFarm::growthUpdate() {
    if (_status == 1) {
        _growth -= 2;
    } else if (_status == 2) {
        _growth -= 1;
    } else if (_status == 4) {
        _growth += 1;
    } else if (_status == 5) {
        _growth += 2;
    }
    _growth = min(max(_growth, 0), GROWTH_MAX);
}

void PlantFarm::yieldUpdate() {
    if (_growth != GROWTH_MAX) {
        _yield = 0;
    } else {
        _yield = (_status == 4) ? 1 : (_status == 5) ? 2 : 0;
    }
}

bool State::operator<(const State &other) const {
    return tie(time, water, nitrogen, status, growth, yield) < tie(other.time, other.water, other.nitrogen, other.status, other.growth, other.yield);
}

bool Action::operator<(const Action &other) const {
    return tie(waterInput, nitroInput) < tie(other.waterInput, other.nitroInput);
}

MonteCarloMDP::MonteCarloMDP() {
    srand(time(nullptr)); // Seed for random number generation
}

void MonteCarloMDP::generateEpisode() {
    episode.clear();
    farm.reset();
    while (farm.getTime() < 10) {
        State currentState = {farm.getTime(), farm.getWater(), farm.getNitro(), farm.getStatus(), farm.getGrowth(), farm.getYield()};
        Action action = {rand() % 5, rand() % 5};
        double reward = performAction(action);
        episode.push_back(make_tuple(currentState, action, reward));
        if (reward == -100) {
            break;
        }
    }
}

double MonteCarloMDP::performAction(Action action) {
    bool continueExperiment = farm.transition(action.waterInput, action.nitroInput);
    double reward = 0;
    int status = farm.getStatus();
    int growth = farm.getGrowth();
    int yield = farm.getYield();

    if (status == 1) {
        reward = -100;
    } else if (status == 4 || status == 5) {
        reward = status + growth + yield * 4;
    }

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

void MonteCarloMDP::run(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        generateEpisode();
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
             << "] Action: [WaterInput: " << action
             << "State: [Time: " << state.time << ", Water: " << state.water << ", Nitrogen: " << state.nitrogen
             << ", Status: " << state.status << ", Growth: " << state.growth << ", Yield: " << state.yield
             << "] Action: [WaterInput: " << action.waterInput << ", NitroInput: " << action.nitroInput
             << "] Q-Value: " << qValue << "\n";
    }
}

int main() {
    MonteCarloMDP mc;
    mc.run(1000); // Run for 1000 episodes
    mc.printQValues();
    return 0;
}
