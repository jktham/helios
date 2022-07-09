#pragma once

#include "planet.h"

#include <vector>

class Solarsystem
{
public:
	std::vector<Planet*> planets;
	float time_scale = 1.0f;
	bool paused = false;

	void initializePlanets();
	void generatePlanets();
	void updatePlanets(float delta_time);
	void drawPlanets();
};