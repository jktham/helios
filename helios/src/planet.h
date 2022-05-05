#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

class Planet
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);

	float radius = 1.0f;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLuint shader = 0;
	GLuint texture1 = 0;

	std::string shader_name = "default_planet";
	std::string texture_name = "test";

	void compileShader();
	void loadTextures();
	void generateBuffers();
	void generateMesh();
	void updateModelMatrix();
	void updateBuffers();
	void draw();
};