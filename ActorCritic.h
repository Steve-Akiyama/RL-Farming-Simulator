#include "PlantFarm.h"
#include <iostream>
#include <cstdlib>  // For rand() function
#include <ctime>    // For time() function
#include <string.h>
#include <cmath>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstdio>
#include <string>
#include <limits>



class ActorCritic {

    
    double gamma;
    double epsilon;
    double alpha;
    bool debug = false;
    double * V;
    double * theta;

    public:

        ActorCritic();

        void runActorCritic(int);

        int runEpisode();

        void setDebug(bool);

        void setEpsilon(double);

        double getEpsilon();

        void setAlpha(double);

        double getAlpha();

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
        void updateValue(int stateWater, int stateNitro, double TD);

        //Actor policy update
        void updatePolicy(int currentWater, int currentNitro, int waterInput, int nitroInput, double TD);

    PlantFarm farm;

};