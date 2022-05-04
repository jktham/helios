#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Planet
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view{};
	glm::mat4 projection{};

	float radius = 1.0f;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLuint shader = 0;

	void compileShader();
	void generateBuffers();
	void generateMesh();
	void updateModelMatrix();
	void updateBuffers();
	void draw();
};