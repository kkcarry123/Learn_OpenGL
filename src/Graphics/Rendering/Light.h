#pragma once

#include <glm/glm.hpp>
#include "../Shader.h"

struct PointLight
{
	glm::vec3 position;

	// Attenuation constants
	/*
	*  Quadratic Formular:
	*				1
	*	---------------------------
	*	 k0 + k1 * dist + k2 * dist^2
	* 
	* 
	*/
	float k0;
	float k1;
	float k2;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader, int idx);
};

struct DirLight
{
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader);
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;

	// Attenuation constants
	/*
	*  Quadratic Formular:
	*				1
	*	---------------------------
	*	 k0 + k1 * dist + k2 * dist^2
	*
	*
	*/
	float k0;
	float k1;
	float k2;

	float cutOff;
	// 光照的外围衰减区域
	float outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	void render(Shader shader, int idx);

};


class Light
{

};

