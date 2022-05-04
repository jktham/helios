#include "planet.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <vector>
#include <fstream>
#include <iostream>
#include <math.h>

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

void Planet::loadTextures()
{
	int width, height, channels;
	unsigned char* data;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("res/textures/test.png", &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
}

void Planet::generateBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void Planet::generateMesh()
{
	int rings = 50;
	int points = 50;

	float pi = 3.1415f;
	float delta_theta = pi / (float)(rings + 1.0f);
	float delta_phi = 2 * pi / (float)(points);

	float theta = 0.0f;
	float phi = 0.0f;

	// generate vertices
	std::vector<float> vertex;
	int vertex_count = 0;

	vertex = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.5f + atan2(0.0f, 0.0f) / (2.0f * pi), 0.5f + asin(-1.0f) / pi,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	vertices.insert(vertices.end(), vertex.begin(), vertex.end());
	vertex_count += 1;

	for (int r = 0; r < rings; r++)
	{
		theta += delta_theta;
		for (int p = 0; p < points; p++)
		{
			phi += delta_phi;
			float x = sin(theta) * cos(phi);
			float y = sin(theta) * sin(phi);
			float z = cos(theta);
			float u = 0.5f + atan2(-x, -y) / (2.0f * pi);
			float v = 0.5f + asin(-z) / pi;

			vertex = {
				x, y, z,
				x, y, z,
				u, v,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			vertices.insert(vertices.end(), vertex.begin(), vertex.end());
			vertex_count += 1;
		}
	}

	vertex = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.5f + atan2(0.0f, 0.0f) / (2.0f * pi), 0.5f + asin(1.0f) / pi,
		1.0f, 1.0f, 1.0f, 1.0f
	};
	vertices.insert(vertices.end(), vertex.begin(), vertex.end());
	vertex_count += 1;

	// generate indices
	std::vector<unsigned int> index;
	int index_count = 0;

	//      A
	//     . .
	//    .   .
	//   .     .
	//  .       .
	// B.........C

	for (unsigned int i = 1; i < (unsigned int)points + 1; i++)
	{
		unsigned int P = 0;
		unsigned int A = P;
		unsigned int B = P + i;
		unsigned int C = P + i % points + 1;

		index = { A, B, C };
		indices.insert(indices.end(), index.begin(), index.end());
		index_count += 1;
	}

	for (unsigned int i = 1; i < (unsigned int)points + 1; i++)
	{
		unsigned int P = vertex_count - 1;
		unsigned int A = P;
		unsigned int B = P - i;
		unsigned int C = P - i % points - 1;

		index = { A, B, C };
		indices.insert(indices.end(), index.begin(), index.end());
		index_count += 1;
	}

	// A..........D
	// ..         .
	// .   .      .
	// .      .   .
	// .         ..
	// B..........C

	for (unsigned int r = 0; r < (unsigned int)rings - 1; r++)
	{
		for (unsigned int p = 0; p < (unsigned int)points; p++)
		{
			unsigned int i = r * points + p + 1;

			unsigned int A = i;
			unsigned int D = i + 1;
			unsigned int B = i + points;
			unsigned int C = i + points + 1;

			if (p == points - 1)
			{
				D = i - points + 1;
				C = i + 1;
			}

			index = { A, B, C };
			indices.insert(indices.end(), index.begin(), index.end());
			index_count += 1;
			index = { A, C, D };
			indices.insert(indices.end(), index.begin(), index.end());
			index_count += 1;
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (normal)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (texcoord)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (color)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Planet::draw()
{
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "tex"), 0);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glUseProgram(0);
}