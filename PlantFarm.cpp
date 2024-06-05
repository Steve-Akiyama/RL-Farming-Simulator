#include "PlantFarm.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

PlantFarm::PlantFarm() {
    reset();
}

void PlantFarm::reset() {
    _time = 0;
    _water = WATER_START;
    _nitro = NITRO_START;
    _status = STATUS_START;
    _growth = 0;
    _yield = 0;
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

int PlantFarm::getFinalTime() {
    return TIME_FINAL;
}

void PlantFarm::printStatus() {
    cout << 
       "Time: " << std::setw(2) << std::setfill('0') << getTime() <<
    "\nWater: " << getWater() <<
    "\nNitrogen: " << getNitro() <<
    "\nStatus: " << getStatus() <<
    "\nGrowth: " << getGrowth() <<
    "\nYield: " << getYield() <<
    "\nReward: " << reward() <<
    "\n\n";
}

bool PlantFarm::transition(int waterInput, int nitroInput) {
    _time += 1;
    waterChange(waterInput);
    nitroChange(nitroInput);
    statusUpdate();
    growthUpdate();
    yieldUpdate();

    if (_time == TIME_FINAL || _status == 0) {
        printStatus();
        return 1;
    } else {
        printStatus();
        return 0;
    }
}

void PlantFarm::waterChange(int waterInput) {
    _water += waterInput;
    _water -= WATER_DECAY;

    int random_number = rand() % 100 + 1;

    if (random_number < (100 * WATER_CHANCE)) {
        if (random_number % 2) {
            _water += 1;
        } else {
            _water -= 1;
        }
    }

    if (_water < 0) {
        _water = 0;
    } else if (_water > WATER_MAX) {
        _water = WATER_MAX;
    }
}

void PlantFarm::nitroChange(int nitroInput) {
    _nitro += nitroInput;
    _nitro -= NITRO_DECAY;

    int random_number = rand() % 100;

    if (random_number < (100 * NITRO_CHANCE)) {
        if (random_number % 2) {
            _nitro += 1;
        } else {
            _nitro -= 1;
        }
    }

    if (_nitro < 0) {
        _nitro = 0;
    } else if (_nitro > NITRO_MAX) {
        _nitro = NITRO_MAX;
    }
}

void PlantFarm::statusUpdate() {
    _status = STATUS_START;

    switch (_water) {
        case 0: _status -= 2; break;
        case 1: _status += 0; break;
        case 2: _status += 1; break;
        case 3: _status += 0; break;
        case 4: _status -= 2; break;
    }

    switch (_nitro) {
        case 0: _status -= 1; break;
        case 1: if (_status > 3) { _status -= 1; } break;
        case 2: if (_status >= 3) { _status += 1; } break;
        case 3: break;
        case 4: _status -= 1; break;
    }

    if (_status < 0) {
        _status = 0;
    } else if (_status > STATUS_MAX) {
        _status = STATUS_MAX;
    }
}

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
    } else if (_growth > GROWTH_MAX) {
        _growth = GROWTH_MAX;
    }
}

void PlantFarm::yieldUpdate() {
    if (_growth != GROWTH_MAX) {
        _yield = 0;
        return;
    } else {
        if (_status == 4) {
            _yield = 1;
        } else if (_status == 5) {
            _yield = 2;
        }
    }
}

int PlantFarm::reward() {
    int reward = 0;

    switch (_status) {
        case 0: reward += -200; break;
        case 1: reward += -50; break;
        case 2: reward += -10; break;
        case 3: reward += 0; break;
        case 4: reward += 5; break;
        case 5: reward += 10; break;
    }

    switch (_yield) {
        case 0: reward += 0; break;
        case 1: reward += 50; break;
        case 2: reward += 100; break;
    }

    return reward;
}
