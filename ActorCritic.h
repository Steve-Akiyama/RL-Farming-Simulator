#include "PlantFarm.h"
#include "DataPlot.cpp"
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

    //Hyperparameters
    double gamma;
    double epsilon;
    double alpha;

    //Hyperparameter adaptivity
    double epsilonDecay; //Decays epsilon after every episode (0.95 means epsilon loses 5% of its current value)
    double epsilonStop;  //Sets epsilon to zero after a percent of the episodes (0.9 means the last 10% will have epsilon = 0)

    //Debug prints out additional info, waits for user input between episodes
    bool debug = false;
    
    //Data constructs for Actor-Critic
    double * V;
    double * theta;

    //Data file output
    std::string outputFile = "episode_rewards.dat";

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