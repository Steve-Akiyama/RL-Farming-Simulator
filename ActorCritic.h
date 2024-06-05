#include "PlantFarm.h"
#include <iostream>
#include <cstdlib>
#include <cstdlib>  // For rand() function
#include <ctime>    // For time() function
#include <string.h>
#include <cmath>
#include <iomanip>


class ActorCritic {

    
    double gamma = 0.99;
    double epsilon = 0.2;
    double alpha = 0.2;
    bool debug = false;
    double * V;
    double * theta;

    public:

        ActorCritic();

        int runEpisode();

        void setDebug(bool);

    private:

        //Initialize value function
        void initValueFunct();

        //initialize theta
        void initTheta();

        //small random value function to init theta
        double smallRandomValue();

        //Actor action method
        void getAction(int * action);

        //Critic TD update
        double TDError(int stateWater, int stateNitro, int reward, int nextWater, int nextNitro);
        void updateValue(int stateWater, int stateNitro, int TD);

        //Actor policy update
        void updatePolicy(int currentWater, int currentNitro, int waterInput, int nitroInput, int TD);

    PlantFarm farm;

};