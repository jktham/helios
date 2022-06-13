#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <cmath>

struct Material
{
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(1.0f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);
	float shininess = 8.0f;
};

struct Light
{
	glm::vec3 color = glm::vec3(1.0f);
	glm::vec3 ambient = glm::vec3(0.01f);
	glm::vec3 diffuse = glm::vec3(0.6f);
	glm::vec3 specular = glm::vec3(0.2f);
};

class Planet
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

	float radius = 1.0f;
	unsigned long int mass = 0;
	std::string name = "";
	int id = 0;
	bool lines_enabled = true;

	glm::vec3 pole_axis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 rotation_axis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
	float rotation_speed = 0.0f;
	float rotation_offset = 0.0f;

	Planet* orbit_anchor = nullptr;
	glm::vec3 orbit_center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 orbit_axis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
	float orbit_radius = 0.0f;
	float orbit_speed = 0.0f;
	float orbit_offset = 0.0f;

	Material material;
	Light light;
	Planet* light_source = this;

	glm::mat4 body_model = glm::mat4(1.0f);
	glm::mat4 orbit_model = glm::mat4(1.0f);
	glm::mat4 axis_model = glm::mat4(1.0f);

	std::vector<float> body_vertices;
	std::vector<unsigned int> body_indices;

	std::vector<float> orbit_vertices;
	std::vector<float> axis_vertices;

	GLuint body_vao = 0;
	GLuint body_vbo = 0;
	GLuint body_ebo = 0;
	GLuint body_shader = 0;
	GLuint body_texture = 0;

	GLuint orbit_vao = 0;
	GLuint orbit_vbo = 0;
	GLuint orbit_shader = 0;
	GLuint axis_vao = 0;
	GLuint axis_vbo = 0;
	GLuint axis_shader = 0;

	std::string body_shader_path = "src/planet_body";
	std::string texture_path = "res/textures/test.png";

	std::string orbit_shader_path = "src/planet_orbit";
	std::string axis_shader_path = "src/planet_axis";

	void compileShader();
	void loadTextures();
	void generateMesh();
	void updatePosition(float delta_time);
	void updateRotation(float delta_time);
	void updateModelMatrix();
	void generateBuffers();
	void updateBuffers();
	void drawBody();
	void drawOrbit();
	void drawAxis();
};