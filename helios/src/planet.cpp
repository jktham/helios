#include "planet.h"
#include "camera.h"
#include "global.h"

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

	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

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
	int rings = 63;
	int points = 128;

	double pi = 3.1415926;
	double delta_theta = pi / (float)(rings + 1);
	double delta_phi = 2 * pi / (float)(points);

	double theta = 0.0f;
	double phi = 0.0f;

	// generate vertices
	std::vector<float> vertex;
	int vertex_count = 0;

	// north pole vertices
	for (int i = 0; i < points; i++)
	{
		vertex = {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			i * 1.0f / (float)points, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};
		vertices.insert(vertices.end(), vertex.begin(), vertex.end());
		vertex_count += 1;

		// north pole seam vertex
		if (i == points - 1)
		{
			vertex = {
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			vertices.insert(vertices.end(), vertex.begin(), vertex.end());
			vertex_count += 1;
		}
	}

	// body vertices
	for (int r = 0; r < rings; r++)
	{
		phi = 0.0;
		theta += delta_theta;
		for (int p = 0; p < points; p++)
		{
			float x = (float)(sin(theta) * cos(phi));
			float y = (float)(sin(theta) * sin(phi));
			float z = (float)(cos(theta));
			float u = (float)(phi / (2.0f * pi));
			float v = (float)(theta / pi);

			vertex = {
				x, y, z,
				x, y, z,
				u, v,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			vertices.insert(vertices.end(), vertex.begin(), vertex.end());
			vertex_count += 1;

			phi += delta_phi;

			// body seam vertex
			if (p == points - 1)
			{
				float x = (float)(sin(theta) * cos(phi));
				float y = (float)(sin(theta) * sin(phi));
				float z = (float)(cos(theta));
				float u = (float)(phi / (2.0f * pi));
				float v = (float)(theta / pi);

				vertex = {
					x, y, z,
					x, y, z,
					u, v,
					1.0f, 1.0f, 1.0f, 1.0f
				};
				vertices.insert(vertices.end(), vertex.begin(), vertex.end());
				vertex_count++;
			}
		}
	}

	// south pole vertices
	for (int i = 0; i < points; i++)
	{
		vertex = {
			0.0f, 0.0f, -1.0f,
			0.0f, 0.0f, -1.0f,
			i * 1.0f / (float)points, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};
		vertices.insert(vertices.end(), vertex.begin(), vertex.end());
		vertex_count++;

		// south pole seam vertex
		if (i == points - 1)
		{
			vertex = {
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			vertices.insert(vertices.end(), vertex.begin(), vertex.end());
			vertex_count++;
		}
	}

	// generate indices
	std::vector<unsigned int> index;
	int index_count = 0;

	// pole indices
	//      A
	//     . .
	//    .   .
	//   .     .
	//  .       .
	// B.........C

	for (unsigned int i = 0; i < (unsigned int)points; i++)
	{
		unsigned int P = i + 1;
		unsigned int A = P;
		unsigned int B = P + points;
		unsigned int C = P + points + 1;

		index = { A, B, C };
		indices.insert(indices.end(), index.begin(), index.end());
		index_count++;
	}

	for (unsigned int i = 0; i < (unsigned int)points; i++)
	{
		unsigned int P = vertex_count - i - 2;
		unsigned int A = P;
		unsigned int B = P - points;
		unsigned int C = P - points - 1;

		index = { A, B, C };
		indices.insert(indices.end(), index.begin(), index.end());
		index_count++;
	}

	// body indices
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
			unsigned int i = r * (points + 1) + p + points + 1;

			unsigned int A = i;
			unsigned int D = i + 1;
			unsigned int B = i + points + 1;
			unsigned int C = i + points + 2;

			index = { A, B, C };
			indices.insert(indices.end(), index.begin(), index.end());
			index_count++;
			index = { A, C, D };
			indices.insert(indices.end(), index.begin(), index.end());
			index_count++;
		}
	}

	//std::cout << vertex_count << "\n"; // (rings + 2) * (points + 1)
	//std::cout << index_count << "\n"; // rings * points * 2
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
	glUniform3f(glGetUniformLocation(shader, "light.position"), -100.0f, -100.0f, 100.0f);
	glUniform3f(glGetUniformLocation(shader, "light.ambient"), 0.02f, 0.02f, 0.02f);
	glUniform3f(glGetUniformLocation(shader, "light.diffuse"), 0.6f, 0.6f, 0.6f);
	glUniform3f(glGetUniformLocation(shader, "light.specular"), 0.2f, 0.2f, 0.2f);

	glUniform3f(glGetUniformLocation(shader, "material.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "material.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader, "material.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), 8.0f);

	glUniform3f(glGetUniformLocation(shader, "view_pos"), camera.position.x, camera.position.y, camera.position.z);
	glUniform1i(glGetUniformLocation(shader, "texture1"), 0);

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);

	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glUseProgram(0);
}