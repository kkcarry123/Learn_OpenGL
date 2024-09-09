#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/scene.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(std::string dir, std::string path, aiTextureType type);

	void generate();
	void load(bool flip = true);

	void bind();

	unsigned int id;
	aiTextureType type;
	std::string dir;
	std::string path;

};

