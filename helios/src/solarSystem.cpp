#include "solarSystem.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

void SolarSystem::initializePlanets()
{
	Planet* planet = new Planet;
	planets.push_back(planet);
	planets.back()->name = "S1";
	planets.back()->orbit_center = glm::vec3(0.0f, 0.0f, 10.0f);
	planets.back()->radius = 8.0f;
	planets.back()->rotation_speed = -0.3f;
	planets.back()->shader_name = "default_sun";

	planet = new Planet;
	planets.push_back(planet);
	planets.back()->name = "S1-P1";
	planets.back()->radius = 1.0f;
	planets.back()->rotation_speed = 1.5f;
	planets.back()->relative_orbit = true;
	planets.back()->orbit_anchor = planets[0];
	planets.back()->orbit_radius = 15.0f;
	planets.back()->orbit_speed = 1.5f;
	planets.back()->orbit_offset = 2.0f * 3.14f / 2.0f;

	planet = new Planet;
	planets.push_back(planet);
	planets.back()->name = "S1-P2";
	planets.back()->radius = 2.0f;
	planets.back()->rotation_speed = 1.0f;
	planets.back()->relative_orbit = true;
	planets.back()->orbit_anchor = planets[0];
	planets.back()->orbit_radius = 30.0f;
	planets.back()->orbit_speed = 1.0f;
	planets.back()->orbit_offset = 2.0f * 3.14f / 6.0f;

	planet = new Planet;
	planets.push_back(planet);
	planets.back()->name = "S1-P3";
	planets.back()->radius = 5.0f;
	planets.back()->rotation_speed = 0.6f;
	planets.back()->relative_orbit = true;
	planets.back()->orbit_anchor = planets[0];
	planets.back()->orbit_radius = 40.0f;
	planets.back()->orbit_speed = 0.6f;
	planets.back()->orbit_offset = 2.0f * 3.14f / 1.0f;

	planet = new Planet;
	planets.push_back(planet);
	planets.back()->name = "S1-P3-M1";
	planets.back()->radius = 0.5f;
	planets.back()->rotation_speed = 2.0f;
	planets.back()->relative_orbit = true;
	planets.back()->orbit_anchor = planets[3];
	planets.back()->orbit_radius = 7.0f;
	planets.back()->orbit_speed = 2.0f;
	planets.back()->orbit_offset = 2.0f * 3.14f / 2.0f;
}

void SolarSystem::generatePlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->compileShader();
		planets[i]->loadTextures();
		planets[i]->generateMesh();
		planets[i]->generateBuffers();
		planets[i]->updateBuffers();
	}
}

void SolarSystem::updatePlanets(float delta_time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->updatePosition(delta_time);
		planets[i]->updateRotation(delta_time);
	}
}

void SolarSystem::drawPlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->updateModelMatrix();
		planets[i]->draw();
	}
}