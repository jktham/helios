#include "planet.h"
#include "camera.h"
#include "global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>

void Planet::compileShader()
{
	// body vertex shader
	const char* body_vert_source;

	std::ifstream body_vert_file(body_shader_path + ".vs");
	std::string body_vert_string((std::istreambuf_iterator<char>(body_vert_file)), std::istreambuf_iterator<char>());
	body_vert_source = body_vert_string.c_str();

	unsigned int body_vert_shader;
	body_vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(body_vert_shader, 1, &body_vert_source, NULL);
	glCompileShader(body_vert_shader);

	// body fragment shader
	const char* body_frag_source;

	std::ifstream body_frag_file(body_shader_path + ".fs");
	std::string body_frag_string((std::istreambuf_iterator<char>(body_frag_file)), std::istreambuf_iterator<char>());
	body_frag_source = body_frag_string.c_str();

	unsigned int body_frag_shader;
	body_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(body_frag_shader, 1, &body_frag_source, NULL);
	glCompileShader(body_frag_shader);

	// body shader program
	body_shader = glCreateProgram();

	glAttachShader(body_shader, body_vert_shader);
	glAttachShader(body_shader, body_frag_shader);
	glLinkProgram(body_shader);

	glDeleteShader(body_vert_shader);
	glDeleteShader(body_frag_shader);

	// orbit vertex shader
	const char* orbit_vert_source;

	std::ifstream orbit_vert_file(orbit_shader_path + ".vs");
	std::string orbit_vert_string((std::istreambuf_iterator<char>(orbit_vert_file)), std::istreambuf_iterator<char>());
	orbit_vert_source = orbit_vert_string.c_str();

	unsigned int orbit_vert_shader;
	orbit_vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(orbit_vert_shader, 1, &orbit_vert_source, NULL);
	glCompileShader(orbit_vert_shader);

	// orbit fragment shader
	const char* orbit_frag_source;

	std::ifstream orbit_frag_file(orbit_shader_path + ".fs");
	std::string orbit_frag_string((std::istreambuf_iterator<char>(orbit_frag_file)), std::istreambuf_iterator<char>());
	orbit_frag_source = orbit_frag_string.c_str();

	unsigned int orbit_frag_shader;
	orbit_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(orbit_frag_shader, 1, &orbit_frag_source, NULL);
	glCompileShader(orbit_frag_shader);

	// orbit shader program
	orbit_shader = glCreateProgram();

	glAttachShader(orbit_shader, orbit_vert_shader);
	glAttachShader(orbit_shader, orbit_frag_shader);
	glLinkProgram(orbit_shader);

	glDeleteShader(orbit_vert_shader);
	glDeleteShader(orbit_frag_shader);

	// axis vertex shader
	const char* axis_vert_source;

	std::ifstream axis_vert_file(axis_shader_path + ".vs");
	std::string axis_vert_string((std::istreambuf_iterator<char>(axis_vert_file)), std::istreambuf_iterator<char>());
	axis_vert_source = axis_vert_string.c_str();

	unsigned int axis_vert_shader;
	axis_vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(axis_vert_shader, 1, &axis_vert_source, NULL);
	glCompileShader(axis_vert_shader);

	// axis fragment shader
	const char* axis_frag_source;

	std::ifstream axis_frag_file(axis_shader_path + ".fs");
	std::string axis_frag_string((std::istreambuf_iterator<char>(axis_frag_file)), std::istreambuf_iterator<char>());
	axis_frag_source = axis_frag_string.c_str();

	unsigned int axis_frag_shader;
	axis_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(axis_frag_shader, 1, &axis_frag_source, NULL);
	glCompileShader(axis_frag_shader);

	// axis shader program
	axis_shader = glCreateProgram();

	glAttachShader(axis_shader, axis_vert_shader);
	glAttachShader(axis_shader, axis_frag_shader);
	glLinkProgram(axis_shader);

	glDeleteShader(axis_vert_shader);
	glDeleteShader(axis_frag_shader);
}

void Planet::loadTextures()
{
	int width, height, channels;
	unsigned char* data;

	glGenTextures(1, &texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
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

	// north pole vertices
	for (int i = 0; i < points; i++)
	{
		vertex = {
			0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f,
			i * 1.0f / (float)points, 0.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		};
		body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());

		// north pole seam vertex
		if (i == points - 1)
		{
			vertex = {
				0.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 1.0f,
				1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());
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
			body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());

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
				body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());
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
		body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());

		// south pole seam vertex
		if (i == points - 1)
		{
			vertex = {
				0.0f, 0.0f, -1.0f,
				0.0f, 0.0f, -1.0f,
				1.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f
			};
			body_vertices.insert(body_vertices.end(), vertex.begin(), vertex.end());
		}
	}

	// generate body_indices
	std::vector<unsigned int> index;

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
		body_indices.insert(body_indices.end(), index.begin(), index.end());
	}

	for (unsigned int i = 0; i < (unsigned int)points; i++)
	{
		unsigned int P = (int)body_vertices.size() / 12 - i - 2;
		unsigned int A = P;
		unsigned int B = P - points;
		unsigned int C = P - points - 1;

		index = { A, B, C };
		body_indices.insert(body_indices.end(), index.begin(), index.end());
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
			body_indices.insert(body_indices.end(), index.begin(), index.end());
			index = { A, C, D };
			body_indices.insert(body_indices.end(), index.begin(), index.end());
		}
	}

	// orbit vertices
	points = 360;
	delta_phi = 2 * pi / (float)points;
	phi = 0.0f;

	for (int i = 0; i < points; i++)
	{
		vertex = {
			cos((float)phi), sin((float)phi), 0.0f
		};
		orbit_vertices.insert(orbit_vertices.end(), vertex.begin(), vertex.end());
		phi += delta_phi;
	}

	// axis vertices
	float length = 1.5f;
	vertex = {
			0.0f, 0.0f, length
	};
	axis_vertices.insert(axis_vertices.end(), vertex.begin(), vertex.end());
	vertex = {
			0.0f, 0.0f, -length
	};
	axis_vertices.insert(axis_vertices.end(), vertex.begin(), vertex.end());
}

void Planet::updatePosition(float delta_time)
{
	if (relative_orbit)
	{
		orbit_center = orbit_anchor->position;
	}

	orbit_offset += orbit_speed * delta_time * solarsystem.time_scale * !solarsystem.paused;

	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 orbit_plane_i = glm::vec3(glm::cross(up, orbit_axis));
	glm::vec3 orbit_plane_j = glm::vec3(glm::cross(orbit_axis, orbit_plane_i));

	float orbit_x = cos(orbit_offset) * orbit_radius;
	float orbit_y = sin(orbit_offset) * orbit_radius;

	if ((glm::length(orbit_plane_i) != 0.0f) && (glm::length(orbit_plane_j) != 0.0f))
	{
		position = orbit_center + orbit_x * glm::normalize(orbit_plane_i) + orbit_y * glm::normalize(orbit_plane_j);
	}
	else
	{
		position = orbit_center + glm::vec3(orbit_x, orbit_y, 0.0f);
	}
}

void Planet::updateRotation(float delta_time)
{
	rotation_offset += rotation_speed * delta_time * solarsystem.time_scale * !solarsystem.paused;
}

void Planet::updateModelMatrix()
{
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 pole_plane_i = glm::cross(up, pole_axis);
	glm::vec3 pole_plane_j = glm::cross(pole_axis, pole_plane_i);

	float pole_rotation_offset = acos(glm::dot(glm::normalize(up), glm::normalize(pole_axis)));
	glm::vec3 pole_rotation_axis = glm::cross(glm::normalize(up), glm::normalize(pole_axis));

	body_model = glm::mat4(1.0f);
	body_model = glm::translate(body_model, position);
	body_model = glm::rotate(body_model, rotation_offset, rotation_axis);
	if (glm::length(pole_rotation_axis) != 0.0f)
		body_model = glm::rotate(body_model, pole_rotation_offset, glm::normalize(pole_rotation_axis));
	body_model = glm::scale(body_model, glm::vec3(radius));

	float orbit_rotation_offset = acos(glm::dot(glm::normalize(up), glm::normalize(orbit_axis)));
	glm::vec3 orbit_rotation_axis = glm::cross(glm::normalize(up), glm::normalize(orbit_axis));

	orbit_model = glm::mat4(1.0f);
	orbit_model = glm::translate(orbit_model, orbit_center);
	if (glm::length(orbit_rotation_axis) != 0.0f)
		orbit_model = glm::rotate(orbit_model, orbit_rotation_offset, glm::normalize(orbit_rotation_axis));
	orbit_model = glm::scale(orbit_model, glm::vec3(orbit_radius));

	axis_model = glm::mat4(1.0f);
	axis_model = glm::translate(axis_model, position);
	if (glm::length(pole_rotation_offset) != 0.0f)
		axis_model = glm::rotate(axis_model, pole_rotation_offset, glm::normalize(pole_rotation_axis));
	axis_model = glm::scale(axis_model, glm::vec3(radius));
}

void Planet::generateBuffers()
{
	glGenVertexArrays(1, &body_VAO);
	glGenBuffers(1, &body_VBO);
	glGenBuffers(1, &body_EBO);

	glGenVertexArrays(1, &orbit_VAO);
	glGenBuffers(1, &orbit_VBO);
	glGenVertexArrays(1, &axis_VAO);
	glGenBuffers(1, &axis_VBO);
}

void Planet::updateBuffers()
{
	// vertex array object
	glBindVertexArray(body_VAO);

	// vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, body_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * body_vertices.size(), body_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (position)
	glBindBuffer(GL_ARRAY_BUFFER, body_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (normal)
	glBindBuffer(GL_ARRAY_BUFFER, body_VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (texcoord)
	glBindBuffer(GL_ARRAY_BUFFER, body_VBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (color)
	glBindBuffer(GL_ARRAY_BUFFER, body_VBO);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, body_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, body_indices.size() * sizeof(unsigned int), &body_indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// vertex array object
	glBindVertexArray(orbit_VAO);

	// vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, orbit_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_vertices.size(), orbit_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (position)
	glBindBuffer(GL_ARRAY_BUFFER, orbit_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// vertex array object
	glBindVertexArray(axis_VAO);

	// vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * axis_vertices.size(), axis_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vertex attribute (position)
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Planet::drawBody()
{
	glUseProgram(body_shader);
	glUniform3f(glGetUniformLocation(body_shader, "material.color"), material.color.r, material.color.g, material.color.b);
	glUniform3f(glGetUniformLocation(body_shader, "material.ambient"), material.ambient.r, material.ambient.g, material.ambient.b);
	glUniform3f(glGetUniformLocation(body_shader, "material.diffuse"), material.diffuse.r, material.diffuse.g, material.diffuse.b);
	glUniform3f(glGetUniformLocation(body_shader, "material.specular"), material.specular.r, material.specular.g, material.specular.b);
	glUniform1f(glGetUniformLocation(body_shader, "material.shininess"), material.shininess);

	glUniform3f(glGetUniformLocation(body_shader, "light.position"), light_source->position.x, light_source->position.y, light_source->position.z);
	glUniform3f(glGetUniformLocation(body_shader, "light.color"), light_source->light.color.r, light_source->light.color.g, light_source->light.color.b);
	glUniform3f(glGetUniformLocation(body_shader, "light.ambient"), light_source->light.ambient.r, light_source->light.ambient.g, light_source->light.ambient.b);
	glUniform3f(glGetUniformLocation(body_shader, "light.diffuse"), light_source->light.diffuse.r, light_source->light.diffuse.g, light_source->light.diffuse.b);
	glUniform3f(glGetUniformLocation(body_shader, "light.specular"), light_source->light.specular.r, light_source->light.specular.g, light_source->light.specular.b);

	glUniform3f(glGetUniformLocation(body_shader, "view_pos"), camera.position.x, camera.position.y, camera.position.z);
	glUniform1i(glGetUniformLocation(body_shader, "texture1"), 0);

	glUniformMatrix4fv(glGetUniformLocation(body_shader, "model"), 1, GL_FALSE, glm::value_ptr(body_model));
	glUniformMatrix4fv(glGetUniformLocation(body_shader, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(body_shader, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUseProgram(0);

	glUseProgram(body_shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(body_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, body_EBO);

	glDrawElements(GL_TRIANGLES, (GLsizei)body_indices.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Planet::drawOrbit()
{
	glUseProgram(orbit_shader);
	glUniformMatrix4fv(glGetUniformLocation(orbit_shader, "model"), 1, GL_FALSE, glm::value_ptr(orbit_model));
	glUniformMatrix4fv(glGetUniformLocation(orbit_shader, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(orbit_shader, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUseProgram(0);

	glUseProgram(orbit_shader);
	glBindVertexArray(orbit_VAO);

	glDrawArrays(GL_LINE_LOOP, 0, (GLsizei)orbit_vertices.size() / 3);

	glBindVertexArray(0);
	glUseProgram(0);
}

void Planet::drawAxis()
{
	glUseProgram(axis_shader);
	glUniformMatrix4fv(glGetUniformLocation(axis_shader, "model"), 1, GL_FALSE, glm::value_ptr(axis_model));
	glUniformMatrix4fv(glGetUniformLocation(axis_shader, "view"), 1, GL_FALSE, glm::value_ptr(camera.view));
	glUniformMatrix4fv(glGetUniformLocation(axis_shader, "projection"), 1, GL_FALSE, glm::value_ptr(camera.projection));
	glUseProgram(0);

	glUseProgram(axis_shader);
	glBindVertexArray(axis_VAO);

	glDrawArrays(GL_LINES, 0, (GLsizei)axis_vertices.size() / 3);

	glBindVertexArray(0);
	glUseProgram(0);
}