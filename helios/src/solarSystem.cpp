#include "solarSystem.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

void SolarSystem::initializePlanets()
{
	planets.push_back(Planet());
	planets.back().orbit_center = glm::vec3(0.0f, 0.0f, 0.0f);
	planets.back().radius = 8.0f;
	planets.back().rotation_speed = -0.3f;
	planets.back().shader_name = "default_sun";

	planets.push_back(Planet());
	planets.back().radius = 1.0f;
	planets.back().rotation_speed = 1.5f;
	planets.back().orbit_center = planets[0].orbit_center;
	planets.back().orbit_radius = 15.0f;
	planets.back().orbit_speed = 1.5f;
	planets.back().orbit_amount = 2.0f * 3.14f / 2.0f;

	planets.push_back(Planet());
	planets.back().radius = 2.0f;
	planets.back().rotation_speed = 1.0f;
	planets.back().orbit_center = planets[0].orbit_center;
	planets.back().orbit_radius = 30.0f;
	planets.back().orbit_speed = 1.0f;
	planets.back().orbit_amount = 2.0f * 3.14f / 6.0f;

	planets.push_back(Planet());
	planets.back().radius = 5.0f;
	planets.back().rotation_speed = 0.6f;
	planets.back().orbit_center = planets[0].orbit_center;
	planets.back().orbit_radius = 40.0f;
	planets.back().orbit_speed = 0.6f;
	planets.back().orbit_amount = 2.0f * 3.14f / 1.0f;
}

void SolarSystem::generatePlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].compileShader();
		planets[i].loadTextures();
		planets[i].generateMesh();
		planets[i].generateBuffers();
		planets[i].updateBuffers();
	}
}

void SolarSystem::updatePlanets(float delta_time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].updatePosition(delta_time);
		planets[i].updateRotation(delta_time);
	}
}

void SolarSystem::drawPlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].updateModelMatrix();
		planets[i].draw();
	}
}