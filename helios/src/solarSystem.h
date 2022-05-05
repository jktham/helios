#pragma once

#include "planet.h"

#include <vector>

class SolarSystem
{
public:
	std::vector<Planet> planets;

	void initializePlanets();
	void generatePlanets();
	void updatePlanets();
	void drawPlanets();
};