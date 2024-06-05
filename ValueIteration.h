#include "PlantFarm.h"

#ifndef VALUE_ITERATION_H
#define VALUE_ITERATION_H

#include <iostream>
#include <cstdlib>

class ValueIteration
{
    /**
     * Defines constants for the class. Includes upper bounds, starting values, transition function chances, and decay rates.
     */

    // Variables
    PlantFarm *plant_farm;

public:
    // Default constructor. Initializes values.
    ValueIteration();

    // Getters for variables within the class.

    // Calculates and returns the reward based on the status and yield.

    // Prints variable information about the current state, as well as the current reward.

    /**
     * The one and only public class other than the constructor that affects the values within the class.
     * Allows the class to progress one time unit, with inputs for water and nitrogen.
     */

private:
};

#endif // PLANT_FARM_H