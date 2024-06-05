#include <iostream>
#include <cstdlib>
#include "PlantFarm.h"

using namespace std;

/**
 * Default Constructor
 */
PlantFarm::PlantFarm() {
    //Initializes varialbes
    _time = 0;
    _growth = 0;
    _yield = 0;
    _water = WATER_START;
    _nitro = NITRO_START;
    _status = STATUS_START;
}

/**
 * Copy Constructor
 */
PlantFarm::PlantFarm(const PlantFarm& other) {
    //Initializes varialbes
    _time = other._time;
    _growth = other._growth;
    _yield = other._yield;
    _water = other._water;
    _nitro = other._nitro;
    _status = other._status;
}

int PlantFarm::getTime() {
    return _time;
}

int PlantFarm::getWater() {
    return _water;
}

int PlantFarm::getNitro() {
    return _nitro;
}

int PlantFarm::getStatus() {
    return _status;
}

int PlantFarm::getGrowth() {
    return _growth;
}

int PlantFarm::getYield() {
    return _yield;
}

float* PlantFarm::getProbabilities() {
    float* prob = new float[2];
    prob[0] = WATER_CHANCE;
    prob[1] = NITRO_CHANCE;

    return prob;
}

/**
 * Prints the working variables of PlantFarm.
 * Includes all status variables within the State portion of the MDP.
*/
void PlantFarm::printStatus() {
    cout <<
        "Time: " << getTime() << " | " <<
        "Wat: " << getWater() << " | " <<
        "Nit: " << getNitro() << " | " <<
        "Stat: " << getStatus() << " | " <<
        "Grw: " << getGrowth() << " | " <<
        "Yld: " << getYield() << " | " <<
        "R: " << reward() << endl;
}

/**
 * Transitions the PlantFarm forward by one time unit.
 *
 * Updates other variables depending on inputs.
*/
bool PlantFarm::transition(int waterInput, int nitroInput) {

    //Checks for correct inputs, and exits otherwise
    if (waterInput > 4 || waterInput < 0 ||
        nitroInput > 4 || nitroInput < 0) {
        cout << "\nPlease enter values within the proper range.";
        return 0;
    }


    //Increments the time
    _time += 1;

    //Updates water and nitrogen variables
    waterAndNitroChange(waterInput, nitroInput);

    //Updates the status to reflect the new nitrogen/water levels
    statusUpdate();

    //Updates the growth
    growthUpdate();

    //Returns the end signifier if the experiment is complete
    if (_time == TIME_FINAL) {
        yieldUpdate(); //Produce crops
        printStatus(); //Prints the stats
        return 1;
    }
    else if (_status == 0) {
        cout << "\nOh no! The plant died.\n";
        printStatus(); //Prints the stats
        return 1;
    }
    else {
        printStatus(); //Prints the stats
        return 0;
    }

}

/**
 * Updates the water and nitrogen with input, natural decay and variable change
*/
void PlantFarm::waterAndNitroChange(int waterInput, int nitroInput) {
    //Updates water and nitrogen as needed
    _water += waterInput;
    _nitro += nitroInput;

    //Updates water and nitrogen with passive decay
    _water -= WATER_DECAY;
    _nitro -= NITRO_DECAY;

    //Creates a random number to generate transition changes
    int random_number = rand() % 100 + 1;
    //cout << "\n\nRandom Number: " << random_number << "\n\n";

    //Determines if the transition function should give a water change
    if (random_number < (100 * WATER_CHANCE)) {
        if (random_number % 2) {
            _water += 1;
        }
        else {
            _water -= 1;
        }
    }

    //Determines if the transition function should give a nitrogen change
    else if (random_number < (100 * (NITRO_CHANCE + WATER_CHANCE))) {
        if (random_number % 2) {
            _nitro += 1;
        }
        else {
            _nitro -= 1;
        }
    }

    //Updates water to fit within bounds
    if (_water < 0) {
        _water = 0;
    }
    else if (_water > WATER_MAX) {
        _water = WATER_MAX;
    }

    //Updates nitrogen to fit within bounds
    if (_nitro < 0) {
        _nitro = 0;
    }
    else if (_nitro > NITRO_MAX) {
        _nitro = NITRO_MAX;
    }
}

/**
 * Updates the status to reflect the current water and nitrogen levels
*/
void PlantFarm::statusUpdate() {

    //Initializes status to STATUS_START on each cycle
    _status = STATUS_START;

    //Updates the status dependant on the water
    switch (_water) {
    case 0: _status -= 2; break;
    case 1: _status += 0; break;
    case 2: _status += 1; break;
    case 3: _status += 0; break;
    case 4: _status -= 2; break;
    }

    //Updates the status dependant on the nitrogen
    switch (_nitro) {
    case 0: //If the plant has no nitrogen, it cannot grow (But also is not damaged)
        _status -= 1;
        break;
    case 1: //If the plant has too little nitrogen, it only can grow a little
        if (_status > 3) {
            _status -= 1; //Still grows if and only if status was max beforehand
        }
        break;
    case 2: //If the plant has the right amount of nitrogen, lets the plant reach max growth IF it is healthy
        if (_status >= 3) {
            _status += 1;
        }
        break;
    case 3: //If the plant has a little too much nitrogen, it's OK, but no effect
        break;
    case 4: //If the plant has way too much nitrogen, it can hurt the plant!
        _status -= 1;
        break;
    }

    //Catch out of bounds statuses
    if (_status < 0) {
        _status = 0;
    }
    else if (_status > STATUS_MAX) {
        _status = STATUS_MAX;
    }

    return;
}

/**
 * Updates the growth based on the status
*/
void PlantFarm::growthUpdate() {
    switch (_status) {
    case 1: _growth -= 2; break;
    case 2: _growth -= 1; break;
    case 3: _growth += 0; break;
    case 4: _growth += 1; break;
    case 5: _growth += 2; break;
    }

    if (_growth < 0) {
        _growth = 0;
    }
    else if (_growth > GROWTH_MAX) {
        _growth = GROWTH_MAX;
    }

    return;
}

/**
 * Updates the plant yield based on the growth
*/
void PlantFarm::yieldUpdate() {
    if (_growth != GROWTH_MAX) {
        _yield = 0;
        return;
    }
    else {
        if (_status == 4) {
            _yield = 1;
        }
        else if (_status == 5) {
            _yield = 2;
        }
    }
}


/**
 * Returns a reward based on the status and yield of the plant.
*/
int PlantFarm::reward() {

    //Initializes reward (return value) to 0
    int reward = 0;

    //Adds reward based on status
    switch (_status) {
    case 0: reward += -1000; break; //Plant died :c
    case 1: reward += -100; break;  //Huge negative reward for a hugely negative status.
    case 2: reward += -10; break;   //Negative reward for a negative status.
    case 3: reward += 0; break;     //Here solely for readability, does nothing if the status will not grow or harm the plant.
    case 4: reward += 5; break;     //Small positive reward for a small positive effect.
    case 5: reward += 10; break;    //Decent positive reward for a positive effect
    }

    //Adds reward based on yield
    switch (_yield) {
    case 0: reward += 0; break;    //No yield, no reward.
    case 1: reward += 25; break;   //Regular yield, regular reward.
    case 2: reward += 50; break;   //Large yield, large reward.
    }

    return reward;
}