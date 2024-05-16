all: PlantFarmApp

PlantFarmApp:
	g++ -o PlantFarmApp TerminalUI.cpp PlantFarm.cpp Main.cpp
	./PlantFarmApp

clean:
	rm PlantFarmApp.exe