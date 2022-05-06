#pragma once

#include "planet.h"

#include <vector>

class SolarSystem
{
public:
	std::vector<Planet*> planets;
	float time_scale = 0.5f;

	void initializePlanets();
	void generatePlanets();
	void updatePlanets(float delta_time);
	void drawPlanets();
};