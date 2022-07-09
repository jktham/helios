#include "ui.h"
#include "global.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

void Element::compileShader()
{
	const char *vert_source;

	std::ifstream vert_file(shader_path + ".vs");
	std::string vert_string((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>());
	vert_source = vert_string.c_str();

	unsigned int vert_shader;
	vert_shader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vert_shader, 1, &vert_source, NULL);
	glCompileShader(vert_shader);

	const char *frag_source;

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
	shader_path = "res/shaders/ui_quad";
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
		position.x,
		position.y,
		color.r,
		color.g,
		color.b,
		color.a,
		position.x,
		position.y + size.y,
		color.r,
		color.g,
		color.b,
		color.a,
		position.x + size.x,
		position.y,
		color.r,
		color.g,
		color.b,
		color.a,

		position.x + size.x,
		position.y,
		color.r,
		color.g,
		color.b,
		color.a,
		position.x,
		position.y + size.y,
		color.r,
		color.g,
		color.b,
		color.a,
		position.x + size.x,
		position.y + size.y,
		color.r,
		color.g,
		color.b,
		color.a,
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

TexturedQuad::TexturedQuad()
{
	shader_path = "res/shaders/ui_textured_quad";
}

void TexturedQuad::loadTexture()
{
	int width, height, channels;
	unsigned char *data;

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);
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
		position.x, position.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y + tex_size.y,
		position.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y,
		position.x + size.x, position.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y + tex_size.y,

		position.x + size.x, position.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y + tex_size.y,
		position.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y,
		position.x + size.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y};
}

void TexturedQuad::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texcoord
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(6 * sizeof(float)));
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
	shader_path = "res/shaders/ui_label";
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
		float y = (float)(17 - c / 16) * w;

		Glyph glyph = {
			glm::vec2(x, y),
			glm::vec2(w),
			widths[c],
		};

		glyphs.insert(std::pair<int, Glyph>(c, glyph));
	}
}

void Label::loadTexture()
{
	generateFont();

	int width, height, channels;
	unsigned char *data;

	glGenTextures(1, &font_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font_texture);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load((font_path + ".png").c_str(), &width, &height, &channels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Label::generateMesh()
{
	mesh = {};
	glm::vec2 offset = glm::vec2(0.0f);

	for (int i = 0; i < text.length(); i++)
	{
		int c = (int)(char)text[i];
		if (c == 10)
		{
			offset.x = 0.0f;
			offset.y += 1.2f;
			continue;
		}

		Glyph glyph = glyphs[c];
		glm::vec2 size = scale;
		glm::vec2 tex_position = glyph.tex_position;
		glm::vec2 tex_size = glyph.tex_size;

		glm::vec2 position = this->position;

		if (parent)
		{
			position += parent->position;
		}

		position += offset * scale;

		vert_stride = 8;
		std::vector<float> verts = {
			position.x, position.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y + tex_size.y,
			position.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y,
			position.x + size.x, position.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y + tex_size.y,

			position.x + size.x, position.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y + tex_size.y,
			position.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x, tex_position.y,
			position.x + size.x, position.y + size.y, color.r, color.g, color.b, color.a, tex_position.x + tex_size.x, tex_position.y};
		mesh.insert(mesh.end(), verts.begin(), verts.end());

		offset.x += glyph.width;
	}
}

void Label::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// position
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// texcoord
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vert_stride * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Label::setUniforms()
{
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(ui.projection));
	glUniform1i(glGetUniformLocation(shader, "font_texture"), 0);
	glUseProgram(0);
}

void Label::draw()
{
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font_texture);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mesh.size() / vert_stride);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
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

	Label *info_label = (Label *)ui.pages[0]->elements[0];
	info_label->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f * ui.enabled);
	info_label->text = "salat\n";
	info_label->text += "timescale: " + std::to_string(solarsystem.time_scale * !solarsystem.paused) + "\n";
	info_label->text += "movespeed: " + std::to_string(camera.speed) + "\n";
	info_label->text += "position: " + std::to_string(camera.position.x) + ", " + std::to_string(camera.position.y) + ", " + std::to_string(camera.position.z) + "\n";
	info_label->text += "offset: " + std::to_string(camera.offset.x) + ", " + std::to_string(camera.offset.y) + ", " + std::to_string(camera.offset.z) + "\n";
	info_label->text += "anchor: " + camera.anchor->name + "\n";

	glm::vec4 planet_world_pos = glm::vec4(camera.anchor->position, 1.0f);
	glm::vec4 planet_clip_pos = camera.projection * (camera.view * planet_world_pos);
	glm::vec2 planet_window_pos = glm::vec2(-9999.0f);
	if (planet_clip_pos.w > 0.0f)
	{
		glm::vec2 planet_screen_pos = glm::vec3(planet_clip_pos) / planet_clip_pos.w;
		planet_window_pos = ((planet_screen_pos + glm::vec2(1.0f)) / 2.0f) * glm::vec2(1920.0f, -1080.0f) + glm::vec2(0.0f, 1080.0f);
	}

	Label *planet_label = (Label *)ui.pages[0]->elements[1];
	planet_label->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f * ui.enabled);
	planet_label->position = planet_window_pos + glm::vec2(0.0f, 0.0f);
	planet_label->text = "";
	planet_label->text += "name: " + camera.anchor->name + "\n";
	planet_label->text += "id: " + std::to_string(camera.anchor->id) + "\n";
	planet_label->text += "mass: " + std::to_string(camera.anchor->mass) + "\n";
	planet_label->text += "radius: " + std::to_string(camera.anchor->radius) + "\n";
	planet_label->text += "orbit radius: " + std::to_string(camera.anchor->orbit_radius) + "\n";
	if (camera.anchor->orbit_anchor)
		planet_label->text += "orbit anchor: " + camera.anchor->orbit_anchor->name + "\n";
	else
		planet_label->text += "orbit center: " + std::to_string(camera.anchor->orbit_center.x) + ", " + std::to_string(camera.anchor->orbit_center.y) + ", " + std::to_string(camera.anchor->orbit_center.z) + "\n";
	planet_label->text += "orbit speed: " + std::to_string(camera.anchor->orbit_speed) + "\n";
	planet_label->text += "orbit offset: " + std::to_string(camera.anchor->orbit_offset) + "\n";
	planet_label->text += "orbit axis: " + std::to_string(camera.anchor->orbit_axis.x) + ", " + std::to_string(camera.anchor->orbit_axis.y) + ", " + std::to_string(camera.anchor->orbit_axis.z) + "\n";
	planet_label->text += "rotation speed: " + std::to_string(camera.anchor->rotation_speed) + "\n";
	planet_label->text += "rotation offset: " + std::to_string(camera.anchor->rotation_offset) + "\n";
	planet_label->text += "rotation axis: " + std::to_string(camera.anchor->rotation_axis.x) + ", " + std::to_string(camera.anchor->rotation_axis.y) + ", " + std::to_string(camera.anchor->rotation_axis.z) + "\n";
	planet_label->text += "pole axis: " + std::to_string(camera.anchor->pole_axis.x) + ", " + std::to_string(camera.anchor->pole_axis.y) + ", " + std::to_string(camera.anchor->pole_axis.z) + "\n";
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
		Page *page = new Page;
		page->id = i;
		pages.push_back(page);
	}

	Label *info_label = new Label;
	info_label->position = glm::vec2(10.0f, 10.0f);
	info_label->scale = glm::vec2(24.0f);
	info_label->color = glm::vec4(1.0f);
	info_label->text = "";
	pages[0]->elements.push_back(info_label);

	Label *planet_label = new Label;
	planet_label->position = glm::vec2(10.0f, 200.0f);
	planet_label->scale = glm::vec2(24.0f);
	planet_label->color = glm::vec4(1.0f);
	planet_label->text = "";
	pages[0]->elements.push_back(planet_label);

	Label *menu_label = new Label;
	menu_label->position = glm::vec2(10.0f, 10.0f);
	menu_label->scale = glm::vec2(24.0f);
	menu_label->color = glm::vec4(1.0f);
	menu_label->text = "keybinds\nWASD: movement\nUP/DOWN: camera speed\nLEFT/RIGHT: timescale\nSHFIT: sprint\nSPACE: pause\nQ: toggle ui\nTAB: toggle wireframe\n1-9: change anchor\nENTER: menu\nESCAPE: exit";
	pages[1]->elements.push_back(menu_label);
	pages[1]->cursor_enabled = true;

	for (int i = 0; i < pages.size(); i++)
		for (int j = 0; j < pages[i]->elements.size(); j++)
		{
			pages[i]->elements[j]->compileShader();
			pages[i]->elements[j]->generateBuffers();
			pages[i]->elements[j]->loadTexture();
		}
}

void UI::updatePage(Page *page)
{
	page->updateElements();
	page->generateElements();
}

void UI::drawPage(Page *page)
{
	page->drawElements();
}