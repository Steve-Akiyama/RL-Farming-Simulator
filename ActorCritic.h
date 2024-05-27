#ifndef ACTOR_CRITIC_H
#define ACTOR_CRITIC_H

#include <vector>

class ActorCritic {
private:
    double _actorAlpha; // Learning rate for the actor
    double _criticAlpha; // Learning rate for the critic
    std::vector<double> _actorWeights; // Weights for the actor
    std::vector<double> _criticWeights; // Weights for the critic

    double softmax(double x);
    int sampleAction(double probability);
    double dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2);

public:
    ActorCritic(double actorAlpha, double criticAlpha, const std::vector<double>& actorInitialWeights, const std::vector<double>& criticInitialWeights);
    std::pair<int, int> selectAction(const std::vector<double>& state);
    double estimateValue(const std::vector<double>& state);
    void updateWeights(const std::vector<double>& state, const std::pair<int, int>& action, double reward, const std::vector<double>& nextState);
};

#endif // ACTOR_CRITIC_H
