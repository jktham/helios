#include "ui.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <fstream>

void Element::generateMesh()
{
}

void Quad::generateMesh()
{
	mesh = {
		position.x,			  position.y,			color.r, color.g, color.b, color.a,
		position.x,			  position.y + width.y, color.r, color.g, color.b, color.a,
		position.x + width.x, position.y,			color.r, color.g, color.b, color.a,

		position.x + width.x, position.y,			color.r, color.g, color.b, color.a,
		position.x,			  position.y + width.y, color.r, color.g, color.b, color.a,
		position.x + width.x, position.y + width.y, color.r, color.g, color.b, color.a,
	};
}

void Page::generateMesh()
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->generateMesh();
		mesh.insert(mesh.end(), elements[i]->mesh.begin(), elements[i]->mesh.end());
	}
}

void UI::compileShader()
{
	// vertex shader
	const char* vert_source;

	std::ifstream vert_file("src/ui.vs");
	std::string vert_string((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>());
	vert_source = vert_string.c_str();

	unsigned int vert_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	// fragment shader
	const char* frag_source;

	std::ifstream frag_file("src/ui.fs");
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

void UI::generateBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

void UI::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * current_page->mesh.size(), current_page->mesh.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void UI::initializePages()
{
	for (int i = 0; i < 1; i++)
	{
		Page* page = new Page;
		pages.push_back(page);
	}

	/*Quad* test_quad = new Quad;
	test_quad->position = glm::vec2(100.0f, 100.0f);
	test_quad->width = glm::vec2(100.0f, 100.0f);
	test_quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	pages[0]->elements.push_back(test_quad);*/
}

void UI::updatePage()
{
	current_page->generateMesh();
	updateBuffers();
}

void UI::drawPage()
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUseProgram(0);

	glUseProgram(shader);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)current_page->mesh.size() / 6);

	glBindVertexArray(0);
	glUseProgram(0);
}