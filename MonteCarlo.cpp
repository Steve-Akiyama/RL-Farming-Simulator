#include "MonteCarlo.h"
#include "PlantFarm.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>
#include <fstream>
#include <utility>
#include <tuple>
#include <iterator>
#include <numeric>
#include <iomanip>

using namespace std;

MonteCarloMDP::MonteCarloMDP() : GAMMA(0.9), EPSILON(0.2), ALPHA(0.1), MAX_EPISODES(10000) {
    actions = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},
        {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4},
        {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}
    };

    PlantFarm plant_farm;
    probabilities = plant_farm.getProbabilities();
}

struct MonteCarloMDP::State* MonteCarloMDP::init_current_state(PlantFarm& plant_farm) {
    State* current_state = new State {
        plant_farm.getTime(),
        plant_farm.getWater(),
        plant_farm.getNitro(),
        plant_farm.getStatus(),
        plant_farm.getGrowth(),
        plant_farm.getYield()
    };

    for (int i = 0; i < actions.size(); i++) {
        if (value_function.find(make_pair(*current_state, actions[i])) == value_function.end()) {
            value_function[make_pair(*current_state, actions[i])] = 0.0;
        }
    }

    return current_state;
}

int MonteCarloMDP::get_best_action(struct State& state) {
    int best_action_id = 0;
    double best_action_value = -numeric_limits<double>::infinity();

    for (int i = 0; i < actions.size(); i++) {
        if (best_action_value < value_function[make_pair(state, actions[i])]) {
            best_action_value = value_function[make_pair(state, actions[i])];
            best_action_id = i;
        }
    }

    return best_action_id;
}

double MonteCarloMDP::qvalue(PlantFarm& plant_farm, struct State& S, Action& A) {
    double Q = 0;
    map<State, double> P;

    struct State state1 = S;
    state1.water += A.first - 2;
    state1.nitro += A.second - 2;
    P[state1] = 1.0 - probabilities[1] - probabilities[0];

    struct State state2 = S;
    state2.water += A.first - 3;
    state2.nitro += A.second - 2;
    P[state2] = probabilities[0] / 2;

    struct State state3 = S;
    state3.water += A.first - 1;
    state3.nitro += A.second - 2;
    P[state3] = probabilities[0] / 2;

    struct State state4 = S;
    state4.water += A.first - 2;
    state4.nitro += A.second - 3;
    P[state4] = probabilities[1] / 2;

    struct State state5 = S;
    state5.water += A.first - 2;
    state5.nitro += A.second - 1;
    P[state5] = probabilities[1] / 2;

    for (const auto& Sprime : P) {
        struct State curr_state = Sprime.first;
        double prob = Sprime.second;
        int best_action_id = get_best_action(curr_state);

        Q += prob * (plant_farm.reward() + GAMMA * value_function[make_pair(curr_state, actions[best_action_id])]);
    }

    return Q;
}

int MonteCarloMDP::get_reward(const State& state) {
    int reward = 0;

    switch (state.status) {
        case 0: reward += -100; break;
        case 1: reward += -20; break;
        case 2: reward += -10; break;
        case 3: reward += 0; break;
        case 4: reward += 10; break;
        case 5: reward += 20; break;
    }

    switch (state.yield) {
        case 0: reward += 0; break;
        case 1: reward += 25; break;
        case 2: reward += 50; break;
    }

    return reward;
}

void MonteCarloMDP::runEpisode() {
    vector<pair<State, Action>> episode;
    State current_state = *init_current_state(farm);

    while (farm.getTime() < 10) {
        int actionIdx = get_best_action(current_state);
        Action action = actions[actionIdx];

        farm.transition(action.first, action.second);
        episode.push_back(make_pair(current_state, action));

        current_state.time = farm.getTime();
        current_state.water = farm.getWater();
        current_state.nitro = farm.getNitro();
        current_state.status = farm.getStatus();
        current_state.growth = farm.getGrowth();
        current_state.yield = farm.getYield();
    }

    double G = 0.0;
    for (vector<pair<State, Action>>::reverse_iterator it = episode.rbegin(); it != episode.rend(); ++it) {
        const State& state = it->first;
        const Action& action = it->second;
        G = get_reward(state) + GAMMA * G;

        pair<State, Action> stateActionPair = make_pair(state, action);
        returns[stateActionPair].push_back(G);
        value_function[stateActionPair] = (1 - ALPHA) * value_function[stateActionPair] + ALPHA * G;
    }
}

void MonteCarloMDP::runMonteCarlo(int episodes) {
    ofstream outFile("output.dat");
    if (!outFile.is_open()) {
        cerr << "Unable to open file for writing" << endl;
        return;
    }

    double best_reward = -numeric_limits<double>::infinity();
    vector<pair<State, Action>> best_episode;

    for (int i = 0; i < episodes; ++i) {
        farm.reset();
        vector<pair<State, Action>> episode;
        State current_state = *init_current_state(farm);

        while (farm.getTime() < 10) {
            int actionIdx = get_best_action(current_state);
            Action action = actions[actionIdx];

            farm.transition(action.first, action.second);
            episode.push_back(make_pair(current_state, action));

            current_state.time = farm.getTime();
            current_state.water = farm.getWater();
            current_state.nitro = farm.getNitro();
            current_state.status = farm.getStatus();
            current_state.growth = farm.getGrowth();
            current_state.yield = farm.getYield();
        }

        double G = 0.0;
        for (vector<pair<State, Action>>::reverse_iterator it = episode.rbegin(); it != episode.rend(); ++it) {
            const State& state = it->first;
            const Action& action = it->second;
            G = get_reward(state) + GAMMA * G;

            pair<State, Action> stateActionPair = make_pair(state, action);
            returns[stateActionPair].push_back(G);
            value_function[stateActionPair] = (1 - ALPHA) * value_function[stateActionPair] + ALPHA * G;
        }

        if (G > best_reward) {
            best_reward = G;
            best_episode = episode;
        }

        // Output all training data to output.dat
        for (const auto& entry : value_function) {
            const State& state = entry.first.first;
            const Action& action = entry.first.second;
            double value = entry.second;

            outFile << "Time: " << state.time
                    << " | Water: " << state.water
                    << " | Nitrogen: " << state.nitro
                    << " | Status: " << state.status
                    << " | Growth: " << state.growth
                    << " | Yield: " << state.yield
                    << " | Action: (water: " << action.first
                    << ", nitrogen: " << action.second << ")"
                    << " | Q-value: " << value
                    << " | Reward: " << get_reward(state) << endl;
        }

        // Update epsilon
        EPSILON = max(EPSILON * EPSILON_DECAY, EPSILON_STOP);
    }
    outFile.close();

    // Print the best episode
    printBestEpisode(best_episode);
}

void MonteCarloMDP::printBestEpisode(const vector<pair<State, Action>>& best_episode) {
    for (const auto& step : best_episode) {
        const State& state = step.first;
        const Action& action = step.second;

        cout << "T: " << std::setw(2) << std::setfill('0') << state.time
             << " | W: " << state.water
             << " | N: " << state.nitro
             << " | S: " << state.status
             << " | G: " << state.growth
             << " | Y: " << state.yield
             << " | R: " << get_reward(state) << endl;
        cout << "<Water> <Nitrogen> Input: " << action.first << " " << action.second << endl;
    }
}

void MonteCarloMDP::run_with_policy() {
    PlantFarm plant_farm;
    plant_farm.reset();
    State* current_state = init_current_state(plant_farm);

    while (plant_farm.getTime() < 10) {
        int actionIdx = get_best_action(*current_state);
        Action action = actions[actionIdx];

        plant_farm.transition(action.first, action.second);

        cout << "T: " << std::setw(2) << std::setfill('0') << current_state->time
             << " | W: " << current_state->water
             << " | N: " << current_state->nitro
             << " | S: " << current_state->status
             << " | G: " << current_state->growth
             << " | Y: " << current_state->yield
             << " | R: " << get_reward(*current_state) << endl;
        cout << "<Water> <Nitrogen> Input: " << action.first << " " << action.second << endl;

        current_state->time = plant_farm.getTime();
        current_state->water = plant_farm.getWater();
        current_state->nitro = plant_farm.getNitro();
        current_state->status = plant_farm.getStatus();
        current_state->growth = plant_farm.getGrowth();
        current_state->yield = plant_farm.getYield();
    }
}
