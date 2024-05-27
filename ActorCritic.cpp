#include "ActorCritic.h"
#include <cmath>
#include <cstdlib>

ActorCritic::ActorCritic(double actorAlpha, double criticAlpha, const std::vector<double>& actorInitialWeights, const std::vector<double>& criticInitialWeights) :
    _actorAlpha(actorAlpha),
    _criticAlpha(criticAlpha),
    _actorWeights(actorInitialWeights),
    _criticWeights(criticInitialWeights) {}

std::pair<int, int> ActorCritic::selectAction(const std::vector<double>& state) {
    // Calculate action probabilities using softmax
    double waterProb = softmax(_actorWeights[0] * state[0] + _actorWeights[1] * state[1] + _actorWeights[2] * state[2]);
    double nitroProb = softmax(_actorWeights[3] * state[0] + _actorWeights[4] * state[1] + _actorWeights[5] * state[2]);
    
    // Sample actions based on probabilities
    int waterInput = sampleAction(waterProb);
    int nitroInput = sampleAction(nitroProb);
    
    return {waterInput, nitroInput};
}

double ActorCritic::estimateValue(const std::vector<double>& state) {
    // Calculate estimated value using linear approximation
    double value = dotProduct(_criticWeights, state);
    
    return value;
}

void ActorCritic::updateWeights(const std::vector<double>& state, const std::pair<int, int>& action, double reward, const std::vector<double>& nextState) {
    // Get TD error (temporal difference error)
    double tdError = reward + estimateValue(nextState) - estimateValue(state);
    
    // Update actor weights
    for (int i = 0; i < _actorWeights.size(); ++i) {
        _actorWeights[i] += _actorAlpha * tdError * state[i];
    }
    
    // Update critic weights
    for (int i = 0; i < _criticWeights.size(); ++i) {
        _criticWeights[i] += _criticAlpha * tdError * state[i];
    }
}

double ActorCritic::softmax(double x) {
    return exp(x) / (exp(0) + exp(x));
}

int ActorCritic::sampleAction(double probability) {
    double randNum = (double)rand() / RAND_MAX;
    return randNum < probability ? 1 : 0; // Binary action: 1 or 0
}

double ActorCritic::dotProduct(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    double result = 0;
    for (int i = 0; i < vec1.size(); ++i) {
        result += vec1[i] * vec2[i];
    }
    return result;
}
