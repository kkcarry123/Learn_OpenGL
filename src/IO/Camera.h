#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{

public:
	glm::vec3 cameraPos;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	glm::vec3 worldUp;

	
	static Camera defaultCamera;

	float yaw;
	float pitch;
	float speed;
	float zoom; // FOV

	Camera(glm::vec3 position);

	void updateCameraDirection(double dx, double dy);
	void updateCameraPos(CameraDirection dir, double dt);
	void updateCameraZoom(double dy);

	float getZoom();

	glm::mat4 getViewMatrix();

	

private:
	void updateCameraVectors();


};

