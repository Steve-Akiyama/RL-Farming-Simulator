#ifndef REINFORCE_H
#define REINFORCE_H

#include "PlantFarm.h"
#include <vector>
#include <random>

class Reinforce {
public:
    Reinforce(double alpha, double gamma, int numActions);
    void run(int episodes);
    void setDebug(bool debugMode);

private:
    double alpha;
    double gamma;
    bool debugMode;
    std::vector<double> theta;  // Policy parameters
    std::vector<std::pair<int, int>> actions;  // List of possible actions
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;

    double policy(int action, const PlantFarm& env);
    int selectAction(const PlantFarm& env);
    void updatePolicy(const std::vector<int>& actions, const std::vector<int>& rewards, const std::vector<PlantFarm>& states);
    void printAverageRewards(const std::vector<double>& averageRewards);
    double computeReturn(const std::vector<int>& rewards, size_t t);
};

#endif // REINFORCE_H
