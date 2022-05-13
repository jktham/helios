#include "camera.h"
#include "ui.h"
#include "global.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	updateCameraVectors();
	updateViewMatrix();
	updateProjectionMatrix();
}

void Camera::updatePosition()
{
	position = anchor->position + offset;
}

void Camera::applyMovement(Movement movement, float delta_time)
{
	if (ui.pages[ui.current_page]->cursor_enabled)
	{
		return;
	}

	glm::vec3 movement_vector = glm::vec3(0.0f);

	if (movement == Movement::FRONT)
		movement_vector += front * speed * delta_time;
	if (movement == Movement::BACK)
		movement_vector -= front * speed * delta_time;
	if (movement == Movement::LEFT)
		movement_vector -= right * speed * delta_time;
	if (movement == Movement::RIGHT)
		movement_vector += right * speed * delta_time;

	offset += movement_vector;
}

void Camera::processMouseMovement(float offset_x, float offset_y)
{
	if (ui.pages[ui.current_page]->cursor_enabled)
	{
		return;
	}

	offset_x *= sensitivity;
	offset_y *= sensitivity;

	yaw += offset_x;
	pitch += offset_y;

	if (pitch > 89.9999f)
		pitch = 89.9999f;
	if (pitch < -89.9999f)
		pitch = -89.9999f;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 view_dir{};
	view_dir.x = cos(glm::radians(-yaw)) * cos(glm::radians(pitch));
	view_dir.y = sin(glm::radians(-yaw)) * cos(glm::radians(pitch));
	view_dir.z = sin(glm::radians(pitch));
	front = glm::normalize(view_dir);

	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f)));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateViewMatrix()
{
	view = glm::lookAt(position, position + front, up);
}

void Camera::updateProjectionMatrix()
{
	projection = glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.01f, 1000.0f);
}