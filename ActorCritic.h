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

        //Class Constructor
        ActorCritic();

        //Runs N episodes of Actor-Critic, prints out a graph of results
        void runActorCritic(int N);

        //Runs a singular episode of Actor-Critic
        int runEpisode();

        //Sets the class in or out of debug mode. Debug mode: Gives additional data
        void setDebug(bool);

        //Getters and Setters
        void setEpsilon(double);
        double getEpsilon();

        void setAlpha(double);
        double getAlpha();

    private:

        //Initialize value function
        void initValueFunct();

        //Initialize theta
        void initTheta();

        //Small random value function to init theta
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