#include "camera.h"
#include "planet.h"
#include "solarsystem.h"
#include "ui.h"
#include "global.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

void processInputState(GLFWwindow* window, float delta_time);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void APIENTRY debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "helios", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	ui.window = window;

	glViewport(0, 0, 1920, 1080);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debug_callback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);
	glPointSize(1.0f);

	float current_frame = 0.0f;
	float last_frame = 0.0f;
	float delta_time = 0.0f;

	solarsystem.initializePlanets();
	solarsystem.generatePlanets();

	camera.offset = glm::vec3(-40.0f, 0.0f, 0.0f);
	camera.anchor = solarsystem.planets[0];

	ui.initializePages();

	while (!glfwWindowShouldClose(window))
	{
		if ((float)glfwGetTime() - last_frame < 1.0f / 120.0f)
			continue;

		current_frame = (float)glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		std::cout << std::fixed;
		std::cout << std::setprecision(4);
		std::cout << "delta: " << delta_time << ", fps: " << 1.0f/delta_time << ", time: " << (float)glfwGetTime() << "\n";

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInputState(window, delta_time);

		solarsystem.updatePlanets(delta_time);

		camera.updatePosition();
		camera.updateViewMatrix();
		camera.updateProjectionMatrix();

		solarsystem.drawPlanets();

		ui.updatePage(ui.pages[ui.current_page]);
		glDisable(GL_DEPTH_TEST);
		ui.drawPage(ui.pages[ui.current_page]);
		glEnable(GL_DEPTH_TEST);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInputState(GLFWwindow* window, float delta_time)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.applyMovement(Movement::FRONT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.applyMovement(Movement::BACK, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.applyMovement(Movement::LEFT, delta_time);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.applyMovement(Movement::RIGHT, delta_time);
	}
}

int render_mode = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
	{
		render_mode = (render_mode + 1) % 3;
		if (render_mode == 0)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (render_mode == 1)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (render_mode == 2)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
	}

	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
	{
		camera.speed *= 10.0f;
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
	{
		camera.speed /= 10.0f;
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		camera.speed *= 2.0f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		camera.speed *= 0.5f;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		if (solarsystem.time_scale < 0.0f)
			solarsystem.time_scale *= 2.0f;
		else
			solarsystem.time_scale *= 0.5f;

		if (abs(solarsystem.time_scale) < 1.0f / pow(2.0f, 6))
			solarsystem.time_scale *= -2.0f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		if (solarsystem.time_scale < 0.0f)
			solarsystem.time_scale *= 0.5f;
		else
			solarsystem.time_scale *= 2.0f;

		if (abs(solarsystem.time_scale) < 1.0f / pow(2.0f, 6))
			solarsystem.time_scale *= -2.0f;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		solarsystem.paused = !solarsystem.paused;
	}

	for (int i = 0; i < 10 && i < solarsystem.planets.size(); i++)
	{
		if (key == GLFW_KEY_0 + i && action == GLFW_PRESS)
		{
			camera.anchor = solarsystem.planets[i];
			//camera.offset = glm::normalize(camera.offset) * solarsystem.planets[i]->radius * 8.0f;
		}
	}

	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
	{
		if (ui.current_page < ui.pages.size() - 1)
		{
			ui.current_page += 1;
		}
		else
		{
			ui.current_page = 0;
		}
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		ui.enabled = !ui.enabled;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
}

bool first_mouse = true;
float last_x = 0.0f;
float last_y = 0.0f;

void mouse_cursor_callback(GLFWwindow* window, double pos_x, double pos_y)
{
	float offset_x = (float)pos_x - last_x;
	float offset_y = last_y - (float)pos_y;
	last_x = (float)pos_x;
	last_y = (float)pos_y;

	if (first_mouse)
	{
		first_mouse = false;
		offset_x = 0.0f;
		offset_y = 0.0f;
	}

	camera.processMouseMovement(offset_x, offset_y);
}

void mouse_scroll_callback(GLFWwindow* window, double offset_x, double offset_y)
{
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}

void APIENTRY debug_callback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:		 std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}