#include "ActorCriticController.h"

ActorCriticController::ActorCriticController() : _actorCritic(0.1, 0.2, {0, 0, 0, 0, 0, 0}, {0, 0, 0}) {
    // No need to initialize PlantFarm here if it's already done in its constructor
}

void ActorCriticController::runExperiment() {
    while (!_plantFarm.getTime() != 10) {
        // Get current state from PlantFarm
        std::vector<double> state = {
            static_cast<double>(_plantFarm.getWater()),
            static_cast<double>(_plantFarm.getNitro()),
            static_cast<double>(_plantFarm.getStatus())
        };

        // Select action using actor
        std::pair<int, int> action = _actorCritic.selectAction(state);

        // Transition using PlantFarm
        _plantFarm.transition(action.first, action.second);

        // Get reward from PlantFarm
        double reward = _plantFarm.reward();

        // Get next state from PlantFarm
        std::vector<double> nextState = {
            static_cast<double>(_plantFarm.getWater()),
            static_cast<double>(_plantFarm.getNitro()),
            static_cast<double>(_plantFarm.getStatus())
        };

        // Update actor-critic weights
        _actorCritic.updateWeights(state, action, reward, nextState);
    }
}
