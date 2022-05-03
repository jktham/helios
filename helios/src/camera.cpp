#include "camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
	updateCameraVectors();
}

void Camera::applyMovement(Movement movement, float delta_time)
{
	float velocity = speed * delta_time;
	if (movement == Movement::FRONT)
		position += front * velocity;
	if (movement == Movement::BACK)
		position -= front * velocity;
	if (movement == Movement::LEFT)
		position -= right * velocity;
	if (movement == Movement::RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float offset_x, float offset_y)
{
	offset_x *= sensitivity;
	offset_y *= sensitivity;

	yaw += offset_x;
	pitch += offset_y;

	if (pitch > 89.99f)
		pitch = 89.99f;
	if (pitch < -89.99f)
		pitch = -89.99f;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front_dir{};
	front_dir.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_dir.y = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front_dir.z = sin(glm::radians(pitch));
	front = glm::normalize(front_dir);

	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 0.0f, 1.0f)));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), 1920.0f / 1080.0f, 0.1f, 200.0f);
}