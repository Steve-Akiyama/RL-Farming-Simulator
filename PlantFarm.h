#ifndef PLANT_FARM_H
#define PLANT_FARM_H

#include <iostream>
#include <cstdlib>

class PlantFarm {
    const int TIME_FINAL = 10;
    const int WATER_MAX = 4;
    const int NITRO_MAX = 4;
    const int STATUS_MAX = 5;
    const int GROWTH_MAX = 4;
    const int YIELD_MAX = 2;
    const int WATER_START = 3;
    const int NITRO_START = 3;
    const int STATUS_START = 3;
    const int HIDDEN_STATUS = 2;
    const float WATER_CHANCE = 0.2;
    const float NITRO_CHANCE = 0.2;
    const int WATER_DECAY = 2;
    const int NITRO_DECAY = 2;
    int _time;
    int _water;
    int _nitro;
    int _status;
    int _growth;
    int _yield;

public:
    PlantFarm();
    int getTime();
    int getWater();
    int getNitro();
    int getStatus();
    int getGrowth();
    int getYield();
    void printStatus();
    bool transition(int waterInput, int nitroInput);

private:
    void waterChange(int waterInput);
    void nitroChange(int nitroInput);
    void statusUpdate();
    void growthUpdate();
    void yieldUpdate();
};

#endif // PLANT_FARM_H
