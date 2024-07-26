PlantFarm project

CS 499
Spring 2024
Oregon State University

Participants:
Steven Akiyama - ActorCritic, PlantFarm, and TerminalUI implementation
Greggory Hickman - Value Iteration and TerminalUI implementation
Joseph Murche - MonteCarlo implementation
Jamie Parker - REINFORCE implementation

Compiling/Running code:

- Requires GNUplot

Compilation script:

g++ -o PlantFarmApp TerminalUI.cpp PlantFarm.cpp DataPlot.cpp ValueIteration.cpp ActorCritic.cpp Main.cpp

Run:

./PlantFarmApp

Project to solve a "PlantFarm" MDP using value iteration.
Problem specifications and class implementation can be found in PlantFarm.cpp
