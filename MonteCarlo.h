#ifndef MONTECARLO_H
#define MONTECARLO_H

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

#endif // MONTECARLO_H
