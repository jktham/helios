#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

class Element
{
public:
	glm::vec2 position = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);

	std::vector<float> mesh;
	int vert_stride = 1;

	std::string shader_path = "";

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint shader = 0;

	virtual void compileShader();
	virtual void generateBuffers();
	virtual void loadTexture();

	virtual void generateMesh();
	virtual void updateBuffers();

	virtual void setUniforms();
	virtual void draw();
};

class Quad : public Element
{
public:
	glm::vec4 color = glm::vec4(1.0f);

	Quad();
	void generateMesh();
	void updateBuffers();
};

class TexturedQuad : public Element
{
public:
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec2 tex_position = glm::vec2(0.0f);
	glm::vec2 tex_size = glm::vec2(1.0f);

	std::string texture_path = "res/textures/test.png";

	GLuint texture = 0;

	TexturedQuad();
	void loadTexture();
	void generateMesh();
	void updateBuffers();
	void setUniforms();
	void draw();
};

class Page
{
public:
	std::vector<Element*> elements;

	void generateElements();
	void drawElements();
};

class UI
{
public:
	std::vector<Page*> pages;
	Page* current_page;

	glm::mat4 projection = glm::ortho(0.0f, 1920.0f, 1080.0f, 0.0f, -1.0f, 1.0f);

	void initializePages();
	void updatePage(Page* page);
	void drawPage(Page* page);
};