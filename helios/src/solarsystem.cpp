#include "solarsystem.h"

#include <glm/glm.hpp>

#include <vector>
#include <string>

void Solarsystem::initializePlanets()
{
	for (int i = 0; i < 8; i++)
	{
		Planet* planet = new Planet;
		planets.push_back(planet);
	}

	planets[0]->name = "S1";
	planets[0]->id = 0;
	planets[0]->orbit_center = glm::vec3(0.0f, 0.0f, 0.0f);
	planets[0]->radius = 8.0f;
	planets[0]->rotation_speed = -0.2f;
	planets[0]->body_shader_path = "src/sun_body";

	planets[1]->name = "S1-P1";
	planets[1]->id = 1;
	planets[1]->radius = 1.0f;
	planets[1]->rotation_speed = 1.0f;
	planets[1]->orbit_anchor = planets[0];
	planets[1]->orbit_radius = 15.0f;
	planets[1]->orbit_speed = 1.0f;
	planets[1]->orbit_offset = 3.0f;
	planets[1]->light_source = planets[0];
	planets[1]->rotation_axis = glm::normalize(glm::vec3(0.1f, -0.2f, 1.0f));
	planets[1]->pole_axis = glm::normalize(glm::vec3(0.1f, -0.2f, 1.0f));
	planets[1]->orbit_axis = glm::normalize(glm::vec3(0.1f, -0.2f, 1.0f));

	planets[2]->name = "S1-P2";
	planets[2]->id = 2;
	planets[2]->radius = 2.0f;
	planets[2]->rotation_speed = -0.6f;
	planets[2]->orbit_anchor = planets[0];
	planets[2]->orbit_radius = 30.0f;
	planets[2]->orbit_speed = -0.6f;
	planets[2]->orbit_offset = 1.0f;
	planets[2]->light_source = planets[0];
	planets[2]->rotation_axis = glm::normalize(glm::vec3(0.0f, 0.1f, 1.0f));
	planets[2]->pole_axis = glm::normalize(glm::vec3(0.0f, 0.1f, 1.0f));
	planets[2]->orbit_axis = glm::normalize(glm::vec3(0.0f, 0.1f, 1.0f));

	planets[3]->name = "S1-P3";
	planets[3]->id = 3;
	planets[3]->radius = 5.0f;
	planets[3]->rotation_speed = 0.1f;
	planets[3]->orbit_anchor = planets[0];
	planets[3]->orbit_radius = 60.0f;
	planets[3]->orbit_speed = 0.1f;
	planets[3]->orbit_offset = 2.0f;
	planets[3]->light_source = planets[0];

	planets[4]->name = "S1-P3-M1";
	planets[4]->id = 4;
	planets[4]->radius = 0.5f;
	planets[4]->rotation_speed = 2.0f;
	planets[4]->orbit_anchor = planets[3];
	planets[4]->orbit_axis = glm::normalize(glm::vec3(0.8f, 0.0f, 1.0f));
	planets[4]->orbit_radius = 10.0f;
	planets[4]->orbit_speed = 2.0f;
	planets[4]->orbit_offset = 0.0f;
	planets[4]->light_source = planets[0];
	planets[4]->rotation_axis = glm::normalize(glm::vec3(0.8f, 0.0f, 1.0f));
	planets[4]->pole_axis = glm::normalize(glm::vec3(0.8f, 0.0f, 1.0f));

	planets[5]->name = "S1-P4";
	planets[5]->id = 5;
	planets[5]->radius = 2.0f;
	planets[5]->rotation_speed = 5.0f;
	planets[5]->orbit_anchor = planets[0];
	planets[5]->orbit_radius = 200.0f;
	planets[5]->orbit_speed = 0.01f;
	planets[5]->orbit_offset = 4.0f;
	planets[5]->light_source = planets[0];
	planets[5]->rotation_axis = glm::normalize(glm::vec3(0.0f, 0.8f, 1.0f));
	planets[5]->pole_axis = glm::normalize(glm::vec3(0.0f, 0.8f, 1.0f));
	planets[5]->orbit_axis = glm::normalize(glm::vec3(0.0f, 0.8f, 1.0f));

	planets[6]->name = "S1-P4-M1";
	planets[6]->id = 6;
	planets[6]->radius = 1.0f;
	planets[6]->rotation_speed = 0.2f;
	planets[6]->orbit_anchor = planets[5];
	planets[6]->orbit_axis = glm::normalize(glm::vec3(2.0f, 0.0f, 1.0f));
	planets[6]->orbit_radius = 40.0f;
	planets[6]->orbit_speed = 0.2f;
	planets[6]->orbit_offset = 5.0f;
	planets[6]->light_source = planets[0];
	planets[6]->rotation_axis = glm::normalize(glm::vec3(2.0f, 0.0f, 1.0f));
	planets[6]->pole_axis = glm::normalize(glm::vec3(2.0f, 0.0f, 1.0f));

	planets[7]->name = "S1-P4-M1-M1";
	planets[7]->id = 7;
	planets[7]->radius = 0.1f;
	planets[7]->rotation_speed = -0.8f;
	planets[7]->orbit_anchor = planets[6];
	planets[7]->orbit_axis = glm::normalize(glm::vec3(0.0f, 0.2f, 1.0f));
	planets[7]->orbit_radius = 4.0f;
	planets[7]->orbit_speed = 0.8f;
	planets[7]->orbit_offset = 1.0f;
	planets[7]->light_source = planets[0];
	planets[7]->rotation_axis = glm::normalize(glm::vec3(0.0f, 0.2f, 1.0f));
	planets[7]->pole_axis = glm::normalize(glm::vec3(0.0f, 0.2f, 1.0f));
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
		planets[i]->updateModelMatrix();
	}
}

void Solarsystem::drawPlanets()
{
	for (int i = 0; i < planets.size(); i++)
	{
		planets[i]->drawBody();
		planets[i]->drawOrbit();
		planets[i]->drawAxis();
	}
}