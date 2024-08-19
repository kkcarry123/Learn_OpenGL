#pragma once

#include "Cube.hpp"
#include "../Rendering/Light.h"

class Lamp : public Cube
{
public:
	glm::vec3 lightColor;

	// Light strength values
	PointLight pointLight;

	Lamp() {};

	Lamp(glm::vec3 lightColor,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float k0,
		float k1,
		float k2,
		glm::vec3 pos,
		glm::vec3 size)
		: lightColor(lightColor),
		pointLight({ pos, k0, k1, k2, ambient, diffuse, specular } ),
		Cube(Material::white_plastic, pos, size) {}

	void render(Shader shader)
	{
		// Set Light Color
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}

};
