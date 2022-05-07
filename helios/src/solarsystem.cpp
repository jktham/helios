#include "solarsystem.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

void Solarsystem::initializePlanets()
{
	for (int i = 0; i < 5; i++)
	{
		Planet* planet = new Planet;
		planets.push_back(planet);
	}

	planets[0]->name = "S1";
	planets[0]->orbit_center = glm::vec3(0.0f, 0.0f, 0.0f);
	planets[0]->radius = 8.0f;
	planets[0]->rotation_speed = -0.2f;
	planets[0]->shader_name = "default_sun";

	planets[1]->name = "S1-P1";
	planets[1]->radius = 1.0f;
	planets[1]->rotation_speed = 1.0f;
	planets[1]->relative_orbit = true;
	planets[1]->orbit_anchor = planets[0];
	planets[1]->orbit_radius = 15.0f;
	planets[1]->orbit_speed = 1.0f;
	planets[1]->orbit_offset = 2.0f * 3.14f / 2.0f;
	planets[1]->light_source = planets[0];

	planets[2]->name = "S1-P2";
	planets[2]->radius = 2.0f;
	planets[2]->rotation_speed = 0.6f;
	planets[2]->relative_orbit = true;
	planets[2]->orbit_anchor = planets[0];
	planets[2]->orbit_radius = 30.0f;
	planets[2]->orbit_speed = 0.6f;
	planets[2]->orbit_offset = 2.0f * 3.14f / 6.0f;
	planets[2]->light_source = planets[0];

	planets[3]->name = "S1-P3";
	planets[3]->radius = 5.0f;
	planets[3]->rotation_speed = 0.1f;
	planets[3]->relative_orbit = true;
	planets[3]->orbit_anchor = planets[0];
	planets[3]->orbit_radius = 40.0f;
	planets[3]->orbit_speed = 0.1f;
	planets[3]->orbit_offset = 2.0f * 3.14f / 1.0f;
	planets[3]->light_source = planets[0];

	planets[4]->name = "S1-P3-M1";
	planets[4]->radius = 0.5f;
	planets[4]->rotation_speed = 2.0f;
	planets[4]->relative_orbit = true;
	planets[4]->orbit_anchor = planets[3];
	planets[4]->orbit_axis = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
	planets[4]->orbit_radius = 7.0f;
	planets[4]->orbit_speed = 2.0f;
	planets[4]->orbit_offset = 2.0f * 3.14f / 2.0f;
	planets[4]->light_source = planets[0];
	planets[4]->rotation_axis = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
	planets[4]->pole_axis = glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f));
}

void Solarsystem::generatePlanets()
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

void Solarsystem::updatePlanets(float delta_time)
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->updatePosition(delta_time);
		planets[i]->updateRotation(delta_time);
	}
}

void Solarsystem::drawPlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->updateModelMatrix();
		planets[i]->draw();
	}
}