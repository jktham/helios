#include "planet.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <fstream>
#include <iostream>

void Planet::compileShader()
{
	// vertex shader
	const char* vert_source;

	std::ifstream vert_file("src/shader.vs");
	std::string vert_string((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>());
	vert_source = vert_string.c_str();

	unsigned int vert_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	// fragment shader
	const char* frag_source;

	std::ifstream frag_file("src/shader.fs");
	std::string frag_string((std::istreambuf_iterator<char>(frag_file)), std::istreambuf_iterator<char>());
	frag_source = frag_string.c_str();

	unsigned int frag_shader;
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(frag_shader, 1, &frag_source, NULL);
	glCompileShader(frag_shader);

	// shader program
	shader = glCreateProgram();

	glAttachShader(shader, vert_shader);
	glAttachShader(shader, frag_shader);
	glLinkProgram(shader);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}

void Planet::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Planet::generateMesh()
{
	int rings = 10;
	int points = 12;

	float pi = 3.1415f;
	float delta_theta = pi / (float)(rings + 1.0f);
	float delta_phi = 2 * pi / (float)(points);

	float theta = 0.0f;
	float phi = 0.0f;

	std::vector<float> vertex;

	vertex = { 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	vertices.insert(vertices.end(), vertex.begin(), vertex.end());

	for (int ring = 0; ring < rings; ring++)
	{
		theta += delta_theta;
		for (int point = 0; point < points; point++)
		{
			phi += delta_phi;
			float x = sin(theta) * cos(phi);
			float y = sin(theta) * sin(phi);
			float z = cos(theta);

			vertex = { x, y, z, 1.0f, 1.0f, 1.0f, 1.0f };
			vertices.insert(vertices.end(), vertex.begin(), vertex.end());
		}
	}

	vertex = { 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	vertices.insert(vertices.end(), vertex.begin(), vertex.end());

	std::vector<unsigned int> index;

	for (unsigned int r = 0; r < rings - 1; r++)
	{
		for (unsigned int p = 0; p < points; p++)
		{
			unsigned int i = r * (unsigned int)points + p;

			unsigned int i1 = i;
			unsigned int i2 = i + 1;
			unsigned int i3 = i + (unsigned int)points;
			unsigned int i4 = i + (unsigned int)points + 1;

			index = { i1, i3, i2 };
			indices.insert(indices.end(), index.begin(), index.end());
			index = { i4, i2, i3 };
			indices.insert(indices.end(), index.begin(), index.end());
		}
	}
}

void Planet::updateModelMatrix()
{
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(radius));
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, position);
}

void Planet::updateBuffers()
{
	// vertex array object
	glBindVertexArray(VAO);

	// vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (position)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (color)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Planet::draw()
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);

	glUseProgram(shader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}