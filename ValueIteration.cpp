#include <ctime>
#include <limits>
#include <map>
#include <tuple>
#include <vector>
#include "ValueIteration.h"

using namespace std;

ValueIteration::ValueIteration()
{
    // Initializes varialbes
    plant_farm = new PlantFarm();
    actions = {
        {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, 
        {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, 
        {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, 
        {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, 
        {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}
    };
}

/**
 * Set up the first state so that the algorithm can begin
 */
void ValueIteration::setup(PlantFarm &farm)
{
    State initialState = {
        farm.getTime(), 
        farm.getWater(), 
        farm.getNitro(), 
        farm.getStatus(), 
        farm.getGrowth(), 
        farm.getYield()
    };
    valueFunction[initialState] = 0.0;
}

clock_t ValueIteration::run() {
    // Start the timer
    clock_t const timer_start = clock();

    // Value Iteration start
    setup(*plant_farm);

    // Stop the timer (code adapted from https://stackoverflow.com/questions/2297829/what-is-the-best-way-to-time-how-long-functions-take-in-c)
    clock_t const timer_end = clock();
    clock_t time_elapsed = 1000000 * (timer_end - timer_start) / CLOCKS_PER_SEC;

    return time_elapsed;
}
