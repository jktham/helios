#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Planet
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view{};
	glm::mat4 projection{};

	float radius = 10.0f;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	unsigned int shader = 0;

	void compileShader();
	void generateBuffers();
	void generateMesh();
	void updateModelMatrix();
	void updateBuffers();
	void draw();
};