#ifndef PLANT_FARM_H
#define PLANT_FARM_H

#include <iostream>
#include <cstdlib>

class PlantFarm {
    /**
     * Defines constants for the class. Includes upper bounds, starting values, transition function chances, and decay rates.
    */
    //Final time period
    const int TIME_FINAL = 10; 

    //Max values
    const int WATER_MAX = 4;
    const int NITRO_MAX = 4;
    const int STATUS_MAX = 5;
    const int GROWTH_MAX = 4;
    const int YIELD_MAX = 2;

    //Starting values
    const int WATER_START = 3;
    const int NITRO_START = 3;
    const int STATUS_START = 3;

    //How long the status of the plants should stay hidden to the agent
    const int HIDDEN_STATUS = 2;

    //Transition function probabilities. Defines the chance that the values vary, so chances/2 is the chance of the values moving up or down.
    const float WATER_CHANCE = 0.2;
    const float NITRO_CHANCE = 0.2;

    //Decay rates for water and nitrogen.
    const int WATER_DECAY = 2;
    const int NITRO_DECAY = 2;

    int _time;      //[0, 10] where 0 is the starting value and 10 is the end of the simulation
    int _water;     //[0, WATER_MAX] where 0 is the lowest possible value and WATER_MAX is the highest possible value
    int _nitro;     //[0, NITRO_MAX] where 0 is the lowest possible value and NITRO_MAX is the highest possible value
    int _status;    //[0, 5] where 1 is heavy plant decay and 5 is plant flourishing. Growth requires a status of 4 or 5, decay requires a status of 1 or 2.
    int _growth;    //[0, 4] where 0 is a seedling and 4 is fully grown.
    int _yield;     //[0, 2] where 0 is no yield and 2 is max yield. Yield > 0 requires a fully grown plant, and max yield requires max status.

public:
    //Default constructor. Initializes values.
    PlantFarm();

    //Getters for variables within the class.
    int getTime();
    int getWater();
    int getNitro();
    int getStatus();
    int getGrowth();
    int getYield();
    int getFinalTime(); // Added this method declaration
    void reset();

    //Calculates and returns the reward based on the status and yield.
    int reward();

    //Prints variable information about the current state, as well as the current reward.
    void printStatus();
    
    /**
     * The one and only public class other than the constructor that affects the values within the class. 
     * Allows the class to progress one time unit, with inputs for water and nitrogen.
    */
    bool transition(int waterInput, int nitroInput);

private:
    //Update function for water
    void waterChange(int waterInput);

    //Update function for nitrogen
    void nitroChange(int nitroInput);

    //Status update function. Uses nitrogen and water status to generate an entirely new status.
    void statusUpdate();

    //Growth update function. Uses status and previously existing growth to determine how grown the plant is.
    void growthUpdate();

    //Yield update function. Uses growth and status to determine an entirely new yield for the plant.
    void yieldUpdate();
};

#endif // PLANT_FARM_H