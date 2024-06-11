#ifndef REINFORCE_H
#define REINFORCE_H

#include <vector>
#include <random>
#include "PlantFarm.h"

class Reinforce {
public:
    Reinforce(double alpha, double gamma);
    void run(int episodes);
    void setDebug(bool debugMode);
    void printPolicy();  // New method to print the policy

private:
    double alpha;  // Learning rate
    double gamma;  // Discount factor
    std::vector<double> theta;  // Policy parameters
    bool debugMode;

    double policy(int action, const PlantFarm& env);
    int selectAction(const PlantFarm& env);
    void updatePolicy(const std::vector<int>& actions, const std::vector<int>& rewards, const std::vector<PlantFarm>& states);
};

#endif // REINFORCE_H
