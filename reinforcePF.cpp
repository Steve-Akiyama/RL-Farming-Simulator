#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <map>
#include <tuple>
#include <ctime>
#include <iomanip>

using namespace std;

// Define constants
const int INPUT_SIZE = 6;  // Size of the state vector
const int HIDDEN_SIZE = 10;
const int OUTPUT_SIZE = 25;  // 5x5 possible actions (water, nitrogen combinations)
const double LEARNING_RATE = 0.01;
const double DISCOUNT_FACTOR = 0.99;
const int EPISODES = 1000;
const double EPSILON = 0.1;  // Exploration rate

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
    PlantFarm() {
        reset();
    }

    void reset() {
        _time = 0;
        _water = WATER_START;
        _nitro = NITRO_START;
        _status = STATUS_START;
        _growth = 0;
        _yield = 0;
    }

    int getTime() { return _time; }
    int getWater() { return _water; }
    int getNitro() { return _nitro; }
    int getStatus() { return _status; }
    int getGrowth() { return _growth; }
    int getYield() { return _yield; }
    int getFinalTime() { return TIME_FINAL; }

    void printStatus() {
        cout << 
        "T: " << std::setw(2) << std::setfill('0') << getTime() <<
        " | W: " << getWater() <<
        " | N: " << getNitro() <<
        " | S: " << getStatus() <<
        " | G: " << getGrowth() <<
        " | Y: " << getYield() <<
        " | R: " << reward() <<
        "\n";
    }

    bool transition(int waterInput, int nitroInput) {
        if (waterInput > 4 || waterInput < 0 || nitroInput > 4 || nitroInput < 0) {
            cout << "\nPlease enter values within the proper range.";
            return 0;
        }

        _time += 1;
        waterChange(waterInput);
        nitroChange(nitroInput);
        statusUpdate();
        growthUpdate();

        if (_time == TIME_FINAL) {
            yieldUpdate();
            printStatus();
            return 1;
        } else if (_status == 0) {
            printStatus();
            return 1;
        } else {
            printStatus();
            return 0;
        }
    }

    int reward() {
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

private:
    void waterChange(int waterInput) {
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

    void nitroChange(int nitroInput) {
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

    void statusUpdate() {
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

    void growthUpdate() {
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

    void yieldUpdate() {
        if (_growth != GROWTH_MAX) {
            _yield = 0;
        } else {
            if (_status == 4) {
                _yield = 1;
            } else if (_status == 5) {
                _yield = 2;
            }
        }
    }
};

// Utility function to generate random normal number
double random_normal() {
    static default_random_engine generator;
    static normal_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Utility function to generate random double between 0 and 1
double random_double() {
    static default_random_engine generator;
    static uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// REINFORCE Class Definition ######################################################
class REINFORCE {
public:
    REINFORCE();
    vector<double> forward(const vector<double>& state);
    void update(const vector<vector<double>>& states, const vector<int>& actions, const vector<double>& returns);

private:
    vector<vector<double>> weights_input_hidden;
    vector<vector<double>> weights_hidden_output;
    vector<double> sigmoid(const vector<double>& x);
    vector<double> softmax(const vector<double>& x);
};

REINFORCE::REINFORCE() {
    weights_input_hidden.resize(INPUT_SIZE, vector<double>(HIDDEN_SIZE));
    weights_hidden_output.resize(HIDDEN_SIZE, vector<double>(OUTPUT_SIZE));
    for (int i = 0; i < INPUT_SIZE; ++i)
        for (int j = 0; j < HIDDEN_SIZE; ++j)
            weights_input_hidden[i][j] = random_normal();

    for (int i = 0; i < HIDDEN_SIZE; ++i)
        for (int j = 0; j < OUTPUT_SIZE; ++j)
            weights_hidden_output[i][j] = random_normal();
}

vector<double> REINFORCE::sigmoid(const vector<double>& x) {
    vector<double> result(x.size());
    for (size_t i = 0; i < x.size(); ++i) {
        result[i] = 1 / (1 + exp(-x[i]));
    }
    return result;
}

vector<double> REINFORCE::softmax(const vector<double>& x) {
    vector<double> exp_x(x.size());
    double sum_exp_x = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        exp_x[i] = exp(x[i]);
        sum_exp_x += exp_x[i];
    }
    for (size_t i = 0; i < x.size(); ++i) {
        exp_x[i] /= sum_exp_x;
    }
    return exp_x;
}

vector<double> REINFORCE::forward(const vector<double>& state) {
    vector<double> hidden(HIDDEN_SIZE, 0.0);
    for (int i = 0; i < HIDDEN_SIZE; ++i) {
        for (int j = 0; j < INPUT_SIZE; ++j) {
            hidden[i] += state[j] * weights_input_hidden[j][i];
        }
        hidden[i] = 1 / (1 + exp(-hidden[i]));  // Sigmoid activation
    }

    vector<double> output(OUTPUT_SIZE, 0.0);
    for (int i = 0; i < OUTPUT_SIZE; ++i) {
        for (int j = 0; j < HIDDEN_SIZE; ++j) {
            output[i] += hidden[j] * weights_hidden_output[j][i];
        }
    }

    return softmax(output);  // Softmax activation
}

void REINFORCE::update(const vector<vector<double>>& states, const vector<int>& actions, const vector<double>& returns) {
    for (size_t i = 0; i < states.size(); ++i) {
        vector<double> state = states[i];
        int action = actions[i];
        double G = returns[i];

        // Forward pass
        vector<double> hidden(HIDDEN_SIZE, 0.0);
        for (int j = 0; j < HIDDEN_SIZE; ++j) {
            for (int k = 0; k < INPUT_SIZE; ++k) {
                hidden[j] += state[k] * weights_input_hidden[k][j];
            }
            hidden[j] = 1 / (1 + exp(-hidden[j]));  // Sigmoid activation
        }

        vector<double> action_probs(OUTPUT_SIZE, 0.0);
        for (int j = 0; j < OUTPUT_SIZE; ++j) {
            for (int k = 0; k < HIDDEN_SIZE; ++k) {
                action_probs[j] += hidden[k] * weights_hidden_output[k][j];
            }
        }

        action_probs = softmax(action_probs);  // Softmax activation

        // Compute gradients
        vector<double> dlogp = action_probs;
        dlogp[action] -= 1;  // Gradient of log softmax

        // Update weights
        for (int j = 0; j < HIDDEN_SIZE; ++j) {
            for (int k = 0; k < OUTPUT_SIZE; ++k) {
                weights_hidden_output[j][k] -= LEARNING_RATE * hidden[j] * dlogp[k] * G;
            }
        }

        for (int j = 0; j < INPUT_SIZE; ++j) {
            for (int k = 0; k < HIDDEN_SIZE; ++k) {
                double dhidden = 0;
                for (int l = 0; l < OUTPUT_SIZE; ++l) {
                    dhidden += weights_hidden_output[k][l] * dlogp[l];
                }
                weights_input_hidden[j][k] -= LEARNING_RATE * state[j] * dhidden * hidden[k] * (1 - hidden[k]) * G;
            }
        }
    }
}

// Helper Functions
vector<double> compute_returns(const vector<double>& rewards) {
    vector<double> returns(rewards.size());
    double G = 0;
    for (int t = rewards.size() - 1; t >= 0; --t) {
        G = rewards[t] + DISCOUNT_FACTOR * G;
        returns[t] = G;
    }
    return returns;
}

int sample_action(const vector<double>& probabilities) {
    random_device rd;
    mt19937 gen(rd());
    discrete_distribution<> dist(probabilities.begin(), probabilities.end());
    return dist(gen);
}

// Main Training Loop
int main() {
    REINFORCE policy;
    PlantFarm farm;
    vector<double> episode_rewards;  // To store cumulative rewards for each episode

    for (int episode = 0; episode < EPISODES; ++episode) {
        vector<vector<double>> states;
        vector<int> actions;
        vector<double> rewards;
        double cumulative_reward = 0.0;

        farm.reset();
        bool done = false;
        while (!done) {
            vector<double> state = {
                double(farm.getTime()), double(farm.getWater()), double(farm.getNitro()),
                double(farm.getStatus()), double(farm.getGrowth()), double(farm.getYield())
            };

            vector<double> action_probs = policy.forward(state);
            int action;

            // Epsilon-greedy action selection for exploration
            if (random_double() < EPSILON) {
                action = rand() % OUTPUT_SIZE;
            } else {
                action = sample_action(action_probs);
            }

            int water_input = action / 5;
            int nitro_input = action % 5;

            done = farm.transition(water_input, nitro_input);

            states.push_back(state);
            actions.push_back(action);
            double reward = farm.reward();
            rewards.push_back(reward);
            cumulative_reward += reward;  // Accumulate the reward
        }

        vector<double> returns = compute_returns(rewards);
        policy.update(states, actions, returns);

        episode_rewards.push_back(cumulative_reward);  // Store the cumulative reward for this episode

        // Print cumulative reward for the current episode for debugging
        cout << "Episode " << episode << " cumulative reward: " << cumulative_reward << endl;
    }

    // Print cumulative rewards for each episode
    for (int episode = 0; episode < EPISODES; ++episode) {
        cout << episode << "," << episode_rewards[episode] << endl;
    }

    return 0;
}
