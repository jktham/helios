#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Element
{
public:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 width = glm::vec2(0.0f);

	std::vector<float> mesh;

	virtual void generateMesh();
};

class Quad : public Element
{
public:
	glm::vec4 color = glm::vec4(1.0f);

	void generateMesh();
};

class Page
{
public:
	std::vector<Element*> elements;
	std::vector<float> mesh;

	void generateMesh();
};

class UI
{
public:
	std::vector<Page*> pages;
	Page* current_page;

	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);

	GLuint vao;
	GLuint vbo;
	GLuint shader;

	void compileShader();
	void generateBuffers();
	void updateBuffers();
	void initializePages();
	void updatePage();
	void drawPage();
};