#include <glm/glm.hpp>

enum class Movement
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

class Camera
{
public:
	float speed = 1.0f;
	float sensitivity = 0.1f;
	float fov = 90.0f;

	float yaw = 0.0f;
	float pitch = 0.0f;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 front{};
	glm::vec3 right{};
	glm::vec3 up{};

	glm::mat4 view;
	glm::mat4 projection;

	Camera();
	void applyMovement(Movement movement, float delta_time);
	void processMouseMovement(float offset_x, float offset_y);
	void updateCameraVectors();
	void updateViewMatrix();
	void updateProjectionMatrix();
};