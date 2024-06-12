#include "Reinforce.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

Reinforce::Reinforce(double alpha, double gamma, int numActions) 
    : alpha(alpha), gamma(gamma), debugMode(false), distribution(0.0, 1.0) {
    theta = std::vector<double>(numActions, 0.0);  // Initialize policy parameters
    std::random_device rd;
    generator = std::default_random_engine(rd());

    // Initialize possible actions
    actions = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
        {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4},
        {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4},
        {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}
    };
}

void Reinforce::run(int episodes) {
    std::vector<double> averageRewards;

    for (int episode = 0; episode < episodes; ++episode) {
        PlantFarm env;
        std::vector<int> actions;
        std::vector<int> rewards;
        std::vector<PlantFarm> states;
        bool done = false;

        while (!done) {
            int action = selectAction(env);
            actions.push_back(action);
            states.push_back(env);
            done = env.transition(this->actions[action].first, this->actions[action].second);
            rewards.push_back(env.reward());
        }

        updatePolicy(actions, rewards, states);

        double totalReward = std::accumulate(rewards.begin(), rewards.end(), 0.0);
        averageRewards.push_back(totalReward / rewards.size());
    }

    printAverageRewards(averageRewards);
}

void Reinforce::setDebug(bool debugMode) {
    this->debugMode = debugMode;
}

double Reinforce::policy(int action, const PlantFarm& env) {
    double sum_exp = 0.0;
    for (double theta_val : theta) {
        sum_exp += std::exp(theta_val);
    }
    return std::exp(theta[action]) / sum_exp;
}

int Reinforce::selectAction(const PlantFarm& env) {
    std::vector<double> probabilities(actions.size());
    for (int i = 0; i < actions.size(); ++i) {
        probabilities[i] = policy(i, env);
    }

    double random_value = distribution(generator);
    double cumulative_probability = 0.0;
    for (int i = 0; i < actions.size(); ++i) {
        cumulative_probability += probabilities[i];
        if (random_value <= cumulative_probability) {
            return i;
        }
    }
    return actions.size() - 1;  // Return the last action if none selected
}

void Reinforce::updatePolicy(const std::vector<int>& actions, const std::vector<int>& rewards, const std::vector<PlantFarm>& states) {
    for (size_t t = 0; t < actions.size(); ++t) {
        double G = computeReturn(rewards, t);
        double policy_prob = policy(actions[t], states[t]);
        theta[actions[t]] += alpha * G * (1 - policy_prob); // Policy gradient update
    }
}

double Reinforce::computeReturn(const std::vector<int>& rewards, size_t t) {
    double G = 0.0;
    for (size_t k = t; k < rewards.size(); ++k) {
        G += std::pow(gamma, k - t) * rewards[k];
    }
    return G;
}

void Reinforce::printAverageRewards(const std::vector<double>& averageRewards) {
    std::cout << "Episode,AverageReward" << std::endl;  // Print CSV header
    for (size_t i = 0; i < averageRewards.size(); ++i) {
        std::cout << i << "," << averageRewards[i] << std::endl;
    }
}
