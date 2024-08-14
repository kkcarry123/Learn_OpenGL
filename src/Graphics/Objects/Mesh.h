#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoord;

	static std::vector<struct Vertex> genList(float* vertices, int noVertices);
};
typedef struct Vertex Vertex;

class Mesh
{

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<Texture> textures;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void render(Shader shader);

	void cleanup();

private:
	unsigned int VBO, EBO;

	void setup();


};

