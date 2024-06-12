#include "Reinforce.h"
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

Reinforce::Reinforce(double alpha, double gamma) : alpha(alpha), gamma(gamma), debugMode(false), distribution(0.0, 1.0) {
    theta = std::vector<double>(5, 0.0);  // Initialize policy parameters
    std::random_device rd;
    generator = std::default_random_engine(rd());
}

void Reinforce::run(int episodes) {
    std::vector<double> averageRewards;  // Declare averageRewards

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
            done = env.transition(action, action);  // Simplification: same action for water and nitrogen
            rewards.push_back(env.reward());
            if (debugMode) {
                std::cout << "Episode " << episode << " Time " << env.getTime() << " Action: " << action << " Reward: " << rewards.back() << std::endl;
            }
        }

        updatePolicy(actions, rewards, states);

        double totalReward = std::accumulate(rewards.begin(), rewards.end(), 0.0);
        averageRewards.push_back(totalReward / rewards.size());
    }

    printAverageRewards(averageRewards);  // Print the average rewards after training
}

void Reinforce::setDebug(bool debugMode) {
    this->debugMode = debugMode;
}

double Reinforce::policy(int action, const PlantFarm& env) {
    // Softmax policy calculation
    double sum_exp = 0.0;
    for (int i = 0; i < theta.size(); ++i) {
        sum_exp += std::exp(theta[i]);
    }
    return std::exp(theta[action]) / sum_exp;
}

int Reinforce::selectAction(const PlantFarm& env) {
    std::vector<double> probabilities(5);
    for (int i = 0; i < 5; ++i) {
        probabilities[i] = policy(i, env);
    }

    double random_value = distribution(generator);
    double cumulative_probability = 0.0;
    for (int i = 0; i < 5; ++i) {
        cumulative_probability += probabilities[i];
        if (random_value <= cumulative_probability) {
            return i;
        }
    }
    return 4;  // Return the last action if none selected
}

void Reinforce::updatePolicy(const std::vector<int>& actions, const std::vector<int>& rewards, const std::vector<PlantFarm>& states) {
    for (size_t t = 0; t < actions.size(); ++t) {
        double G = 0.0;
        for (size_t k = t; k < rewards.size(); ++k) {
            G += std::pow(gamma, k - t) * rewards[k];
        }
        double policy_prob = policy(actions[t], states[t]);
        theta[actions[t]] += alpha * std::pow(gamma, t) * G * (1 - policy_prob);
        if (debugMode) {
            std::cout << "Episode " << t << " Action: " << actions[t] << " Reward: " << rewards[t] << " G: " << G << " Theta: " << theta[actions[t]] << std::endl;
        }
    }
}

void Reinforce::printAverageRewards(const std::vector<double>& averageRewards) {
    std::cout << "Episode,AverageReward" << std::endl;  // Print CSV header
    for (size_t i = 0; i < averageRewards.size(); ++i) {
        std::cout << i << "," << averageRewards[i] << std::endl;
    }
}
