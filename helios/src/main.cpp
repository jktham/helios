#include "camera.h"
#include "planet.h"
#include "solarsystem.h"
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

// callbacks
void processInputState(GLFWwindow* window, float delta_time);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	// window setup
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1920, 1080, "helios", NULL, NULL);
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 1920, 1080);

	// callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_cursor_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// opengl settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEBUG_OUTPUT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLineWidth(1.0f);
	glPointSize(4.0f);

	// setup
	float current_frame = 0.0f;
	float last_frame = 0.0f;
	float delta_time = 0.0f;

	camera.position = glm::vec3(-40.0f, 0.0f, 0.0f);

	solarsystem.initializePlanets();
	solarsystem.generatePlanets();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		if ((float)glfwGetTime() - last_frame < 1.0f / 120.0f)
			continue;

		current_frame = (float)glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		std::cout << std::fixed;
		std::cout << std::setprecision(4);
		std::cout << "delta: " << delta_time << ", pos: (" << camera.position.x << ", " << camera.position.y << ", " << camera.position.z << ")\n";

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInputState(window, delta_time);

		camera.updateViewMatrix();
		camera.updateProjectionMatrix();

		solarsystem.updatePlanets(delta_time);
		solarsystem.drawPlanets();

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

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		solarsystem.time_scale -= 0.5f;
	}
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		solarsystem.time_scale += 0.5f;
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		solarsystem.paused = !solarsystem.paused;
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
	camera.speed += (float)offset_y * 10.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
}