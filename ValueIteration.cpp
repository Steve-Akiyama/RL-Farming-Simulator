#include <vector>
#include <map>
#include <tuple>
#include <limits>
#include "ValueIteration.h"

using namespace std;

ValueIteration::ValueIteration() {
    //Initializes varialbes
    plant_farm = new PlantFarm();
}