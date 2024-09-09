#pragma once
#include "../Objects/Model.h"
#include "../../IO/Camera.h"
#include "../../IO/Keyboard.h"


class Gun : public Model
{
public:
	Gun()
		: Model(glm::vec3(0.0f), glm::vec3(0.5f)) 
	{}

	void render(Shader shader, bool setModel = false)
	{
		glm::mat4 model = glm::mat4(1.0f);

		// Set position
		pos = Camera::defaultCamera.cameraPos + glm::vec3(Camera::defaultCamera.cameraFront * 8.0f) + glm::vec3(Camera::defaultCamera.cameraUp * -3.15f);
		model = glm::translate(model, pos);

		float theta;

		theta = acos(glm::dot(Camera::defaultCamera.worldUp, Camera::defaultCamera.cameraFront) / 
			glm::length(Camera::defaultCamera.cameraUp) / glm::length(Camera::defaultCamera.cameraFront));

		model = glm::rotate(model, atanf(1) * 2 - theta, Camera::defaultCamera.cameraRight);


		// Rotate around cameraUp using dot product
		glm::vec2 front2d = glm::vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
		float theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, Camera::defaultCamera.cameraFront.z < 0 ? theta : -theta, Camera::defaultCamera.cameraUp);

		// Rotate around camera right using dot product
		//theta = acos(glm::dot(Camera::defaultCamera.cameraUp, Camera::defaultCamera.cameraFront) /
		//	glm::length(Camera::defaultCamera.cameraUp) / glm::length(Camera::defaultCamera.cameraFront));

		//model = glm::rotate(model, glm::radians(90.0f) - theta, Camera::defaultCamera.cameraRight); // Offset by pi / 2 radians bc angle cameraFront between camerFront and gunFront

		model = glm::scale(model, size);

		if (Keyboard::key(GLFW_KEY_SPACE))
		{
			std::cout << Camera::defaultCamera.cameraFront.x << " " <<
				Camera::defaultCamera.cameraFront.y << " " <<
				Camera::defaultCamera.cameraFront.z << " " <<
				Camera::defaultCamera.cameraUp.x << " " <<
				Camera::defaultCamera.cameraUp.y << " " <<
				Camera::defaultCamera.cameraUp.z << " " << std::endl;
		}

		// Scale

		shader.setMat4("model", model);

		Model::render(shader, setModel);


	}

};