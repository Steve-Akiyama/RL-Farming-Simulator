#include <iostream>
#include <cstdlib>


/**
 * Creates an instance of the problem we're solving
 * 
 * Public Methods:
 * - printStatus()
 * - transition()
 * 
 * Getters:
 * - getTime()
 * - getWater()
 * - getNitro()
 * - getStatus()
 * - getGrowth()
 * - getYield()
 * 
*/
class PlantFarm {

    //Initializes upper bounds for variables
    const int TIME_FINAL = 10;
    const int WATER_MAX = 4;
    const int NITRO_MAX = 4;
    const int STATUS_MAX = 5;
    const int GROWTH_MAX = 4;
    const int YIELD_MAX = 2;

    //Initializes starting values for variables (All others start at 0)
    const int WATER_START = 3;
    const int NITRO_START = 3;
    const int STATUS_START = 3;

    //Hidden status threshold
    const int HIDDEN_STATUS = 2;

    //Abnormal water chance
    const float WATER_CHANCE = 0.2;

    //Abnormal nitrogen chance
    const float NITRO_CHANCE = 0.2;

    //Passive decay
    const int WATER_DECAY = 2;
    const int NITRO_DECAY = 2;

    //Variables to track plant statistics
    
    int _time; 
    int _water;
    int _nitro;
    /**
     * 0: Plant death
     * 1: Plant severe damage
     * 2: Plant damage
     * 3: Plant neutral, no growth
     * 4: Plant growth
     * 5: Plant fast growth
    */
    int _status;
    int _growth;
    int _yield;

    public:

        //Constructor for a PlantFarm. Initializes variables to const.
        PlantFarm() {
            
            //Initializes varialbes
            _time = 0;
            _growth = 0;
            _yield = 0;
            _water = WATER_START;
            _nitro = NITRO_START;
            _status = STATUS_START;
        }

        int getTime() {
            return _time;
        }

        int getWater() {
            return _water;
        }

        int getNitro() {
            return _nitro;
        }

        int getStatus() {
            return _status;
        }

        int getGrowth() {
            return _growth;
        }

        int getYield() {
            return _yield;
        }

        /**
         * Prints the working variables of PlantFarm. 
         * Includes all status variables within the State portion of the MDP.
        */
        void printStatus() {
            std::cout << 
            "\nTime:     " << getTime() <<
            "\nWater:    " << getWater() <<
            "\nNitrogen: " << getNitro() <<
            "\nStatus:   " << getStatus() <<
            "\nGrowth:   " << getGrowth() <<
            "\nYield:    " << getYield() <<
            "\n";
        }

        /**
         * Transitions the PlantFarm forward by one time unit.
         * 
         * Updates other variables depending on inputs.
        */
        bool transition(int waterInput, int nitroInput) {
            
            //Updates water and nitrogen variables
            waterChange(waterInput);
            nitroChange(nitroInput);

            //Updates the status to reflect the new nitrogen/water levels
            statusUpdate();

            //Updates the growth and yield
            growthUpdate();
            yieldUpdate();

            //Prints the stats
            printStatus();

            //Increments the time
            _time += 1;

            //Returns the end signifier if the experiment is complete
            if (_time == TIME_FINAL) {
                return 0;
            } else {
                return 1;
            }

        }

    private:

        /**
         * Updates the water with input, natural decay and variable change
        */
        void waterChange(int waterInput) {
            //Updates water as needed
            _water += waterInput;

            //Updates water with passive decay
            _water -= WATER_DECAY;

            //Creates a random number to generate transition changes
            int random_number = rand() % 100;

            //Determines if the transition function should give a water change
            if (random_number < (100 * WATER_CHANCE)) {
                if (random_number % 2) {
                    _water += 1;
                } else {
                    _water -= 1;
                }
            }

            //Updates water to fit within bounds
            if (_water < 0) {
                _water = 0;
            } else if (_water > WATER_MAX) {
                _water = WATER_MAX;
            }
        }

        /**
         * Updates the nitrogen with input, natural decay and variable change
        */
        void nitroChange(int nitroInput) {
            //Updates nitrogen as needed
            _nitro += nitroInput;

            //Updates nitrogen with passive decay
            _nitro -= NITRO_DECAY;

            //Creates a random number to generate transition changes
            int random_number = rand() % 100;
            std::cout << random_number;

            //Determines if the transition function should give a nitrogen change
            if (random_number < (100 * NITRO_CHANCE)) {
                if (random_number % 2) {
                    _nitro += 1;
                } else {
                    _nitro -= 1;
                }
            }

            //Updates water to fit within bounds
            if (_nitro < 0) {
                _nitro = 0;
            } else if (_water > WATER_MAX) {
                _nitro = NITRO_MAX;
            }
        }

        /**
         * Updates the status to reflect the current water and nitrogen levels
        */
        void statusUpdate() {
            //If the water is out of range, reduces status
            if (_water == 0 || _water == WATER_MAX) {
                _status -= 2;
            } else {
                //Otherwise, increases status slightly
                _status += 1;
            }

            //If the water is out of range, reduces status
            if (_nitro == 4) {
                _status -= 1; //Too much nitrogen reduces growth
            } else if (_nitro == 1 && _status > 3) {
                _status -= 1; //Too little nitrogen reduces growth
            } else if (_nitro == 0 && _status > 3) {
                _status = 3; //Far too little nitrogen sets growth to 0
            } else {
                _status += 1; //In any other case, slightly increase status
            }

            //Catch out of bounds statuses
            if (_status < 0) {
                _status = 0;
            } else if (_status > STATUS_MAX) {
                _status = STATUS_MAX;
            }

            return;
        }

        /**
         * Updates the growth based on the status
        */
        void growthUpdate() {
            switch (_status) {
                case 1: _growth -= 2;
                case 2: _growth -= 1;
                case 4: _growth += 1;
                case 5: _growth += 2;

                if (_growth < 0) {
                    _growth = 0;
                } else if (_growth > GROWTH_MAX) {
                    _growth = GROWTH_MAX;
                }

                return;
            }
        }

        /**
         * Updates the plant yield based on the growth
        */
        void yieldUpdate() {
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

};