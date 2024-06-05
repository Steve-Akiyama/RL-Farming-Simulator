all: PlantFarmApp

PlantFarmApp:
	g++ -o PlantFarmApp *.cpp
	./PlantFarmApp

clean:
	rm *.exe


