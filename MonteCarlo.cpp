#include "MonteCarlo.h"
#include "PlantFarm.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <limits>
#include <fstream>
#include <utility>  // for std::pair
#include <tuple>  // for std::tie

using namespace std;

/**
 * Constructor
 * Initializes variables and action space
 */
MonteCarloMDP::MonteCarloMDP() {
    discountFactor = 0.9;
    learningRate = 0.1;

    // Initializes variables
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

/**
 * setup(PlantFarm &farm)
 * Set up the first state so that the algorithm can begin
 */
struct MonteCarloMDP::State* MonteCarloMDP::init_current_state(PlantFarm& plant_farm) {
    // Setup state by getting the initial values from plant_farm
    State* current_state = new State;
    current_state->time = plant_farm.getTime();
    current_state->water = plant_farm.getWater();
    current_state->nitro = plant_farm.getNitro();
    current_state->status = plant_farm.getStatus();
    current_state->growth = plant_farm.getGrowth();
    current_state->yield = plant_farm.getYield();

    // Set Q(s,a) for all actions in state to 0
    for (size_t i = 0; i < actions.size(); i++) {
        if (value_function.find(make_pair(*current_state, actions[i])) == value_function.end()) {
            value_function[make_pair(*current_state, actions[i])] = 0.0;
        }
    }

    return current_state;
}

/**
 * print_value_function(struct State& state)
 * Print the value function for all possible actions from a state
 */
void MonteCarloMDP::print_value_function(struct State& state) {
    // Print state info
    cout << "State: (time: " << state.time
        << ", water: " << state.water
        << ", nitrogen: " << state.nitro
        << ", status: " << state.status
        << ", growth: " << state.growth
        << ", yield: " << state.yield << ")" << endl;

    // Print value function of all actions from states
    cout << "Actions: " << endl;
    for (size_t i = 0; i < actions.size(); i++) {
        cout << "{" << actions[i].waterInput << ", " << actions[i].nitroInput << "}"
            << " Q(s,a): " << value_function[make_pair(state, actions[i])] << endl;
    }

    cout << endl;
}

/**
 * print_policy()
 * Print the full policy
 */
void MonteCarloMDP::print_policy() {
    for (map<State, Action>::const_iterator it = policy.begin(); it != policy.end(); ++it) {
        cout << "State: {" <<
            "T: " << it->first.time << " | " <<
            "Wat: " << it->first.water << " | " <<
            "Nit: " << it->first.nitro << " | " <<
            "Sta: " << it->first.status << " | " <<
            "Grw: " << it->first.growth << " | " <<
            "Yld: " << it->first.yield <<
            "} Action: {" << it->second.waterInput << ", " << it->second.nitroInput << "}" <<
            ", Q(s,a): " << value_function[make_pair(it->first, it->second)] << endl;
    }
}

/**
 * get_best_action(struct State& state)
 * Return the action id with the highest Q(s,a) for a state.
 */
int MonteCarloMDP::get_best_action(struct State& state) {
    int best_action_id = 0;
    double best_action_value = -numeric_limits<double>::infinity();    // Set to very low number (this number can only improve)

    // best_action_id = argmax(actions)
    for (size_t i = 0; i < actions.size(); i++) {
        // Replace best_action_value when a better action is found
        if (best_action_value < value_function[make_pair(state, actions[i])]) {
            best_action_value = value_function[make_pair(state, actions[i])];
            best_action_id = i;
        }
    }

    return best_action_id;
}

/**
 * qvalue(PlantFarm& plant_farm, struct State& S, struct Action& A)
 * Inputs:
 *  plant_farm - used purely to find the probability function
 *  S - the state from the State, Action pair
 *  A - the action from the State, Action pair
 *
 * Calculates a State, Action pair's Q value, and updates the value function with it
 */
double MonteCarloMDP::qvalue(PlantFarm& plant_farm, struct State& S, struct Action& A) {
    double Q = 0;

    // Initialize the probability function, 5 possible next states (s')
    map<State, double> P;    // The probability that each state has to be s'

    struct State state1 = S;    // Water + Input - 2, Nitro + Input - 2
    state1.water += A.waterInput - 2;
    state1.nitro += A.nitroInput - 2;
    P[state1] = 1.0 - probabilities[1] - probabilities[0];  // 60% by default

    struct State state2 = S;    // Water + Input - 3, Nitro + Input - 2
    state2.water += A.waterInput - 3;
    state2.nitro += A.nitroInput - 2;
    P[state2] = probabilities[0] / 2;   // 10% by default

    struct State state3 = S;    // Water + Input - 1, Nitro + Input - 2
    state3.water += A.waterInput - 1;
    state3.nitro += A.nitroInput - 2;
    P[state3] = probabilities[0] / 2;   // 10% by default

    struct State state4 = S;    // Water + Input - 2, Nitro + Input - 3
    state4.water += A.waterInput - 2;
    state4.nitro += A.nitroInput - 3;
    P[state4] = probabilities[1] / 2;   // 10% by default

    struct State state5 = S;    // Water + Input - 2, Nitro + Input - 1
    state5.water += A.waterInput - 2;
    state5.nitro += A.nitroInput - 1;
    P[state5] = probabilities[1] / 2;   // 10% by default

    for (map<State, double>::const_iterator it = P.begin(); it != P.end(); ++it) {
        struct State curr_state = it->first;  // denotes s'
        double prob = it->second; // denotes the transition probability to s'
        int best_action_id = get_best_action(curr_state);

        Q += prob * value_function[make_pair(curr_state, actions[best_action_id])]; // Current equation: sum(T(s,a,s') * V(s'))
    }

    Q *= discountFactor; // Current equation: gamma * sum(T(s,a,s') * V(s'))
    Q += getReward(S, A); // Current equation: R(s) + gamma * sum(T(s,a,s') * V(s'))

    return Q;
}

/**
 * runEpisode()
 * Run an episode of Monte Carlo learning
 */
void MonteCarloMDP::runEpisode() {
    vector<pair<State, Action> > episode;
    State current_state = *init_current_state(farm);

    while (farm.getTime() < 10) {  // Use a constant value as a substitute for TIME_FINAL
        int actionIdx = get_best_action(current_state);
        Action action = actions[actionIdx];

        double reward = performAction(action);
        episode.push_back(make_pair(current_state, action));

        current_state.time = farm.getTime();
        current_state.water = farm.getWater();
        current_state.nitro = farm.getNitro();
        current_state.status = farm.getStatus();
        current_state.growth = farm.getGrowth();
        current_state.yield = farm.getYield();
    }

    double G = 0.0;
    for (vector<pair<State, Action> >::reverse_iterator it = episode.rbegin(); it != episode.rend(); ++it) {
        const State& state = it->first;
        const Action& action = it->second;
        G = 0.0 + discountFactor * G;  // Fix the use of reward

        pair<State, Action> stateActionPair = make_pair(state, action);
        if (find(episode.begin(), it.base(), stateActionPair) == it.base()) {
            value_function[stateActionPair] += learningRate * (G - value_function[stateActionPair]);
        }
    }
}

/**
 * performAction()
 * Perform an action in the environment and get the reward
 */
double MonteCarloMDP::performAction(const Action& action) {
    farm.transition(action.waterInput, action.nitroInput);
    return getReward(State{ farm.getTime(), farm.getWater(), farm.getNitro(), farm.getStatus(), farm.getGrowth(), farm.getYield() }, action);
}

/**
 * runMonteCarlo(int iterations)
 * Train the model for a specified number of iterations
 */
void MonteCarloMDP::runMonteCarlo(int iterations) {
    for (int i = 0; i < iterations; ++i) {
        farm.reset();
        runEpisode();
    }
    outputQValues("output.dat");
}

/**
 * run_with_policy()
 * Run the policy in the environment
 */
void MonteCarloMDP::run_with_policy() {
    PlantFarm plant_farm;
    plant_farm.reset();
    State current_state = *init_current_state(plant_farm);

    while (plant_farm.getTime() < 10) {  // Use a constant value as a substitute for TIME_FINAL
        int actionIdx = get_best_action(current_state);
        Action action = actions[actionIdx];

        plant_farm.transition(action.waterInput, action.nitroInput);

        current_state.time = plant_farm.getTime();
        current_state.water = plant_farm.getWater();
        current_state.nitro = plant_farm.getNitro();
        current_state.status = plant_farm.getStatus();
        current_state.growth = plant_farm.getGrowth();
        current_state.yield = plant_farm.getYield();

        cout << "State: (time: " << current_state.time
            << ", water: " << current_state.water
            << ", nitrogen: " << current_state.nitro
            << ", status: " << current_state.status
            << ", growth: " << current_state.growth
            << ", yield: " << current_state.yield << ") Action: ("
            << action.waterInput << ", " << action.nitroInput << ")" << endl;
    }
}

/**
 * getReward()
 * Get the reward for the given state and action
 */
double MonteCarloMDP::getReward(const State& state, const Action& action) {
    // Implement reward logic
    int plantStatus = state.status;
    int plantGrowth = state.growth;
    int cropYield = state.yield;

    double reward = 0.0;
    if (plantStatus == 4 || plantStatus == 5) reward += plantStatus;
    if (plantStatus == 1) reward -= 100;
    reward += plantGrowth;
    reward += cropYield * 4;
    return reward;
}

/**
 * outputQValues()
 * Output Q-values to a file
 */
void MonteCarloMDP::outputQValues(const string& filename) {
    ofstream outFile(filename.c_str());
    if (outFile.is_open()) {
        for (map<pair<State, Action>, double>::const_iterator it = value_function.begin(); it != value_function.end(); ++it) {
            const State& state = it->first.first;
            const Action& action = it->first.second;
            double value = it->second;
            outFile << "State: (time: " << state.time
                    << ", water: " << state.water
                    << ", nitrogen: " << state.nitro
                    << ", status: " << state.status
                    << ", growth: " << state.growth
                    << ", yield: " << state.yield << ") "
                    << "Action: (water: " << action.waterInput
                    << ", nitrogen: " << action.nitroInput << ") "
                    << "Q-value: " << value << endl;
        }
        outFile.close();
    } else {
        cerr << "Unable to open file for writing" << endl;
    }
}
