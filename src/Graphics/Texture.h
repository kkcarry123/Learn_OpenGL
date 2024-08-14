#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

class Texture
{
public:
	Texture();
	Texture(const char* path, const char* name, bool defaultParams = true);

	void generate();
	void load(bool flip = true);

	void setFilter(GLenum all);
	void setFilter(GLenum mag, GLenum min);

	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);

	void bind();

	// Texture object
	unsigned int id;
	unsigned int tex;
	const char* name;

private:
	 int currentID;
	
	const char* path;
	int width;
	int height;
	int nChannels;

};

