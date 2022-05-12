#include "ui.h"
#include "global.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <vector>
#include <string>
#include <fstream>

void Element::compileShader()
{
	const char* vert_source;

	std::ifstream vert_file(shader_path + ".vs");
	std::string vert_string((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>());
	vert_source = vert_string.c_str();

	unsigned int vert_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	const char* frag_source;

	std::ifstream frag_file(shader_path + ".fs");
	std::string frag_string((std::istreambuf_iterator<char>(frag_file)), std::istreambuf_iterator<char>());
	frag_source = frag_string.c_str();

	unsigned int frag_shader;
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(frag_shader, 1, &frag_source, NULL);
	glCompileShader(frag_shader);

	shader = glCreateProgram();

	glAttachShader(shader, vert_shader);
	glAttachShader(shader, frag_shader);
	glLinkProgram(shader);

	glDeleteShader(vert_shader);
	glDeleteShader(frag_shader);
}

void Element::generateBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
}

void Element::loadTexture()
{
}

void Element::generateMesh()
{
}

void Element::updateBuffers()
{
}

void Element::setUniforms()
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(ui.projection));
	glUseProgram(0);
}

void Element::draw()
{
	glUseProgram(shader);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.size() / vert_stride);

	glBindVertexArray(0);
	glUseProgram(0);
}

Quad::Quad()
{
	shader_path = "src/ui_quad";
}

void Quad::generateMesh()
{
	vert_stride = 6;
	mesh = {
		position.x,			 position.y,		  color.r, color.g, color.b, color.a,
		position.x,			 position.y + size.y, color.r, color.g, color.b, color.a,
		position.x + size.x, position.y,		  color.r, color.g, color.b, color.a,

		position.x + size.x, position.y,		  color.r, color.g, color.b, color.a,
		position.x,			 position.y + size.y, color.r, color.g, color.b, color.a,
		position.x + size.x, position.y + size.y, color.r, color.g, color.b, color.a,
	};
}

void Quad::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

TexturedQuad::TexturedQuad()
{
	shader_path = "src/ui_textured_quad";
}

void TexturedQuad::loadTexture()
{
	int width, height, channels;
	unsigned char* data;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

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

void TexturedQuad::generateMesh()
{
	vert_stride = 8;
	mesh = {
		position.x,			 position.y,		  color.r, color.g, color.b, color.a, tex_position.x			 , tex_position.y,
		position.x,			 position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x			 , tex_position.y + tex_size.y,
		position.x + size.x, position.y,		  color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y,

		position.x + size.x, position.y,		  color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y,
		position.x,			 position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x,			   tex_position.y + tex_size.y,
		position.x + size.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y + tex_size.y,
	};
}

void TexturedQuad::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texcoord
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void TexturedQuad::setUniforms()
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(ui.projection));
	glUniform1i(glGetUniformLocation(shader, "quad_texture"), 0);
	glUseProgram(0);
}

void TexturedQuad::draw()
{
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.size() / vert_stride);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Page::generateElements()
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->generateMesh();
		elements[i]->updateBuffers();
	}
}

void Page::drawElements()
{
	for (int i = 0; i < elements.size(); i++)
	{
		elements[i]->setUniforms();
		elements[i]->draw();
	}
}

void UI::initializePages()
{
	for (int i = 0; i < 1; i++)
	{
		Page* page = new Page;
		pages.push_back(page);
	}

	Quad* quad = new Quad;
	quad->position = glm::vec2(100.0f, 100.0f);
	quad->size = glm::vec2(200.0f, 200.0f);
	quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	pages[0]->elements.push_back(quad);

	TexturedQuad* tex_quad = new TexturedQuad;
	tex_quad->position = glm::vec2(100.0f, 400.0f);
	tex_quad->size = glm::vec2(200.0f, 200.0f);
	tex_quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	pages[0]->elements.push_back(tex_quad);

	for (int i = 0; i < pages.size(); i++)
		for (int j = 0; j < pages[i]->elements.size(); j++)
		{
			pages[i]->elements[j]->compileShader();
			pages[i]->elements[j]->generateBuffers();
			pages[i]->elements[j]->loadTexture();
		}
}

void UI::updatePage(Page* page)
{
	page->generateElements();
}

void UI::drawPage(Page* page)
{
	page->drawElements();
}