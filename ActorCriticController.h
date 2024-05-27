#ifndef ACTOR_CRITIC_CONTROLLER_H
#define ACTOR_CRITIC_CONTROLLER_H

#include "ActorCritic.h"
#include "PlantFarm.h"

class ActorCriticController {
private:
    PlantFarm _plantFarm;
    ActorCritic _actorCritic;

public:
    ActorCriticController();

    void runExperiment();
};

#endif // ACTOR_CRITIC_CONTROLLER_H
