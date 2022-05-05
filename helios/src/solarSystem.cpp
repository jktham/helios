#include "solarSystem.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

void SolarSystem::initializePlanets()
{
	planets.push_back(Planet());
	planets.back().position = glm::vec3(0.0f);
	planets.back().radius = 8.0f;
	planets.back().shader_name = "default_sun";

	planets.push_back(Planet());
	planets.back().position = glm::vec3(15.0f, 0.0f, 5.0f);
	planets.back().radius = 1.0f;

	planets.push_back(Planet());
	planets.back().position = glm::vec3(-25.0f, 15.0f, 0.0f);
	planets.back().radius = 5.0f;

	planets.push_back(Planet());
	planets.back().position = glm::vec3(0.0f, -20.0f, -10.0f);
	planets.back().radius = 2.0f;
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

void SolarSystem::updatePlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].updateModelMatrix();
	}
}

void SolarSystem::drawPlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i].draw();
	}
}