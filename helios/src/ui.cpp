#include "ui.h"
#include "global.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

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
	glm::vec2 position = this->position;

	if (parent)
	{
		position += parent->position;
	}

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

	stbi_set_flip_vertically_on_load(false);
	data = stbi_load(texture_path.c_str(), &width, &height, &channels, 0);
	if (data)
	{
		if (transparency)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
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
	glm::vec2 position = this->position;

	if (parent)
	{
		position += parent->position;
	}

	vert_stride = 8;
	mesh = {
		position.x,			 position.y,		  color.r, color.g, color.b, color.a, tex_position.x,			   tex_position.y,
		position.x,			 position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x,			   tex_position.y + tex_size.y,
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

Label::Label()
{
	shader_path = "src/ui_textured_quad";
}

void Label::generateFont()
{
	std::fstream file(font_path + ".csv", std::fstream::in);
	std::string line;
	std::vector<std::string> lines;
	std::vector<std::string> values;
	std::vector<float> widths;

	while (std::getline(file, line, '\n'))
	{
		lines.push_back(line);
	}

	file.close();

	for (int i = 0; i < lines.size(); i++)
	{
		values.push_back(lines[i].substr(lines[i].find(",") + 1));

		if (i > 7 && i < 264)
		{
			widths.push_back(std::stof(values[i]) / std::stof(values[2]));
		}
	}

	for (unsigned int c = 0; c < 256; c++)
	{
		float w = 1.0f / 16.0f;
		float x = (c % 16) * w;
		float y = (float)(c / 16) * w - 2 * w;

		Glyph glyph = {
			glm::vec2(x, y),
			glm::vec2(w),
			widths[c],
		};

		glyphs.insert(std::pair<int, Glyph>(c, glyph));
	}
}

void Label::generateQuads()
{
	glyph_quads = {};
	float offset = 0.0f;

	for (int i = 0; i < text.length(); i++)
	{
		TexturedQuad* glyph_quad = new TexturedQuad;
		glyph_quad->parent = this;
		glyph_quad->texture_path = font_path + ".png";
		glyph_quad->shader_path = shader_path;
		glyph_quad->transparency = true;

		Glyph glyph = glyphs[(int)(char)text[i]];
		glyph_quad->position = glm::vec2(offset * scale.x, 0.0f);
		glyph_quad->size = scale;
		glyph_quad->tex_position = glyph.tex_position;
		glyph_quad->tex_size = glyph.tex_size;

		glyph_quads.push_back(glyph_quad);
		//std::reverse(glyph_quads.begin(), glyph_quads.end());
		offset += glyph.width;
	}
}

void Label::compileShader()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->compileShader();
	}
}

void Label::generateBuffers()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->generateBuffers();
	}
}

void Label::loadTexture()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->loadTexture();
	}
}

void Label::generateMesh()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->generateMesh();
	}
}

void Label::updateBuffers()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->updateBuffers();
	}
}

void Label::setUniforms()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->setUniforms();
	}
}

void Label::draw()
{
	for (int i = 0; i < glyph_quads.size(); i++)
	{
		glyph_quads[i]->draw();
	}
}

void Page::updateElements()
{
	float time = (float)glfwGetTime();
	glm::dvec2 cursor = glm::vec2(0.0f, 0.0f);

	if (cursor_enabled)
	{
		glfwSetInputMode(ui.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwGetCursorPos(ui.window, &(cursor.x), &(cursor.y));
	}
	else
	{
		glfwSetInputMode(ui.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	ui.pages[0]->elements[0]->position.x = 100.0f + sin(time) * 100.0f;
	ui.pages[1]->elements[0]->position.y = 100.0f + sin(time) * 100.0f;
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
	for (int i = 0; i < 2; i++)
	{
		Page* page = new Page;
		page->id = i;
		pages.push_back(page);
	}

	Quad* quad;
	TexturedQuad* tex_quad;
	Label* label;

	quad = new Quad;
	quad->position = glm::vec2(100.0f, 100.0f);
	quad->size = glm::vec2(200.0f, 200.0f);
	quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	pages[0]->elements.push_back(quad);

	tex_quad = new TexturedQuad;
	tex_quad->position = glm::vec2(0.0f, 400.0f);
	tex_quad->size = glm::vec2(200.0f, 200.0f);
	tex_quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	tex_quad->parent = quad;
	pages[0]->elements.push_back(tex_quad);

	label = new Label;
	label->position = glm::vec2(600.0f, 100.0f);
	label->text = "ABCijk.,-?|:>@°§";
	label->generateFont();
	label->generateQuads();
	pages[0]->elements.push_back(label);

	quad = new Quad;
	quad->position = glm::vec2(500.0f, 500.0f);
	quad->size = glm::vec2(200.0f, 200.0f);
	quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	pages[1]->elements.push_back(quad);

	tex_quad = new TexturedQuad;
	tex_quad->position = glm::vec2(400.0f, 0.0f);
	tex_quad->size = glm::vec2(200.0f, 200.0f);
	tex_quad->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);
	tex_quad->parent = quad;
	pages[1]->elements.push_back(tex_quad);
	pages[1]->cursor_enabled = true;

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
	page->updateElements();
	page->generateElements();
}

void UI::drawPage(Page* page)
{
	page->drawElements();
}