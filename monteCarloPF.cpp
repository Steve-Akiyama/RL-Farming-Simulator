#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <ctime>
#include <tuple>

class PlantFarm {
    static const int TIME_FINAL;
    static const int WATER_MAX;
    static const int NITRO_MAX;
    static const int STATUS_MAX;
    static const int GROWTH_MAX;
    static const int YIELD_MAX;
    static const int WATER_START;
    static const int NITRO_START;
    static const int STATUS_START;
    static const int HIDDEN_STATUS;
    static const float WATER_CHANCE;
    static const float NITRO_CHANCE;
    static const int WATER_DECAY;
    static const int NITRO_DECAY;

    int _time;
    int _water;
    int _nitro;
    int _status;
    int _growth;
    int _yield;

public:
    PlantFarm() : _time(0), _water(WATER_START), _nitro(NITRO_START), _status(STATUS_START), _growth(0), _yield(0) {}

    int getTime() const { return _time; }
    int getWater() const { return _water; }
    int getNitro() const { return _nitro; }
    int getStatus() const { return _status; }
    int getGrowth() const { return _growth; }
    int getYield() const { return _yield; }

    void printStatus() const {
        std::cout << 
        "\nTime:     " << getTime() <<
        "\nWater:    " << getWater() <<
        "\nNitrogen: " << getNitro() <<
        "\nStatus:   " << getStatus() <<
        "\nGrowth:   " << getGrowth() <<
        "\nYield:    " << getYield() <<
        "\n";
    }

    bool transition(int waterInput, int nitroInput) {
        waterChange(waterInput);
        nitroChange(nitroInput);
        statusUpdate();
        growthUpdate();
        yieldUpdate();
        printStatus();
        _time += 1;
        return _time < TIME_FINAL;
    }

private:
    void waterChange(int waterInput) {
        _water += waterInput;
        _water -= WATER_DECAY;
        int random_number = std::rand() % 100;
        if (random_number < (100 * WATER_CHANCE)) {
            _water += (random_number % 2 == 0) ? 1 : -1;
        }
        _water = std::min(std::max(_water, 0), WATER_MAX);
    }

    void nitroChange(int nitroInput) {
        _nitro += nitroInput;
        _nitro -= NITRO_DECAY;
        int random_number = std::rand() % 100;
        if (random_number < (100 * NITRO_CHANCE)) {
            _nitro += (random_number % 2 == 0) ? 1 : -1;
        }
        _nitro = std::min(std::max(_nitro, 0), NITRO_MAX);
    }

    void statusUpdate() {
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
        _status = std::min(std::max(_status, 0), STATUS_MAX);
    }

    void growthUpdate() {
        if (_status == 1) {
            _growth -= 2;
        } else if (_status == 2) {
            _growth -= 1;
        } else if (_status == 4) {
            _growth += 1;
        } else if (_status == 5) {
            _growth += 2;
        }
        _growth = std::min(std::max(_growth, 0), GROWTH_MAX);
    }

    void yieldUpdate() {
        if (_growth != GROWTH_MAX) {
            _yield = 0;
        } else {
            _yield = (_status == 4) ? 1 : (_status == 5) ? 2 : 0;
        }
    }
};

const int PlantFarm::TIME_FINAL = 10;
const int PlantFarm::WATER_MAX = 4;
const int PlantFarm::NITRO_MAX = 4;
const int PlantFarm::STATUS_MAX = 5;
const int PlantFarm::GROWTH_MAX = 4;
const int PlantFarm::YIELD_MAX = 2;
const int PlantFarm::WATER_START = 3;
const int PlantFarm::NITRO_START = 3;
const int PlantFarm::STATUS_START = 3;
const int PlantFarm::HIDDEN_STATUS = 2;
const float PlantFarm::WATER_CHANCE = 0.2;
const float PlantFarm::NITRO_CHANCE = 0.2;
const int PlantFarm::WATER_DECAY = 2;
const int PlantFarm::NITRO_DECAY = 2;

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
    std::map<std::pair<State, Action>, std::pair<double, int>> qValues;
    std::map<std::pair<State, Action>, std::vector<double>> returns;
    std::vector<std::tuple<State, Action, double>> episode;
    PlantFarm farm;

public:
    MonteCarloMDP() {
        std::srand(std::time(nullptr)); // Seed for random number generation
    }

    void generateEpisode() {
        episode.clear();
        farm = PlantFarm();
        while (farm.getTime() < 10) {
            State currentState = {farm.getTime(), farm.getWater(), farm.getNitro(), farm.getStatus(), farm.getGrowth(), farm.getYield()};
            Action action = {std::rand() % 5, std::rand() % 5};
            double reward = performAction(action);
            episode.push_back(std::make_tuple(currentState, action, reward));
            if (reward == -100) {
                break;
            }
        }
    }

    double performAction(Action action) {
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

    void updateQValues() {
        double totalReward = 0;
        std::reverse(episode.begin(), episode.end());
        for (const auto &step : episode) {
            State state;
            Action action;
            double reward;
            std::tie(state, action, reward) = step;
            totalReward += reward;
            auto qValueKey = std::make_pair(state, action);
            returns[qValueKey].push_back(totalReward);
            qValues[qValueKey].first += totalReward;
            qValues[qValueKey].second += 1;
        }
    }

    void run(int iterations) {
        for (int i = 0; i < iterations; ++i) {
            generateEpisode();
            updateQValues();
        }
    }

    void printQValues() {
        for (const auto &entry : qValues) {
            const auto &stateAction = entry.first;
            const auto &qValuePair = entry.second;
            const auto &state = stateAction.first;
            const auto &action = stateAction.second;
            double qValue = qValuePair.first / qValuePair.second;
            std::cout << "State: [Time: " << state.time << ", Water: " << state.water << ", Nitrogen: " << state.nitrogen
                 << ", Status: " << state.status << ", Growth: " << state.growth << ", Yield: " << state.yield
                 << "] Action: [WaterInput: " << action.waterInput << ", NitroInput: " << action.nitroInput
                 << "] Q-Value: " << qValue << "\n";
        }
    }
};

int main() {
    MonteCarloMDP mc;
    mc.run(1000); // Run for 1000 episodes
    mc.printQValues();
    return 0;
}
