#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "Graphics/Objects/Model.h"

#include "Graphics/Models/Cube.hpp"
#include "Graphics/Models/Lamp.hpp"
#include "Graphics/Models/Gun.hpp"

#include "Graphics/Rendering/Light.h"

#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/Gamepad.h"
#include "IO/Camera.h"

#include "Screen/Screen.h"

/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

void process_input(/*GLFWwindow* window,*/ double dt/*, Gamepad& gamepad*/);

Screen screen;

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);

Camera cameras[2] = {
	Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	Camera(glm::vec3(10.0f, 10.0f, 10.0f))
};
int activeCam = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

// setup camera 
Camera Camera::defaultCamera(glm::vec3(0.0f, 0.0f, 0.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;

bool flashLightOn = true;

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!screen.init())
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load Glad" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();
	/*
	* Shaders
	*/

	// compile vertex shader
	Shader shader("assets/object.vs", "assets/object.fs");
	Shader lampShader("assets/object.vs", "assets/lamp.fs");

	Model m(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.5f));
	//m.loadModel("assets/model/source/kanna.gltf");
	// 鲨鱼
	//m.loadModel("assets/model/scene.gltf");
	m.loadModel("assets/model/guns/p90/scene.gltf");

	Gun g;
	g.loadModel("assets/model/guns/p90/scene.gltf");


	glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
	};
	Lamp lamps[4];
	for (unsigned int i = 0; i < 4; i++) {
		lamps[i] = Lamp(glm::vec3(1.0f),
			glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
			1.0f, 0.07f, 0.032f,
			pointLightPositions[i], glm::vec3(0.25f));
		lamps[i].init();
	}


	//Cube cube(Material::gold, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	//cube.init();

	DirLight dirLight = { glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.4f), glm::vec3(0.75f) };

	//Lamp lamp(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 
	//	1.0f, 0.07f, 0.032f,
	//	glm::vec3(-3.0f, -0.5f, -1.0f), glm::vec3(2.0f));
	//lamp.init();

	SpotLight spotLight = {
		cameras[activeCam].cameraPos, cameras[activeCam].cameraFront,
		glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(30.0f)),
		1.0f, 0.074f, 0.032f,
		glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f)
	};
	
	// 检查是否有手柄连接
	int joystickID = GLFW_JOYSTICK_1;
	if (!glfwJoystickPresent(joystickID))
	{
		std::cerr << "Joystick " << joystickID << " is not present" << std::endl;
		//glfwTerminate();
		//return -1;
	}
	else
	{
		// 创建手柄对象
		Gamepad gamepad(joystickID);
	}



	while (!screen.shouldClose())
	{
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		lastFrame = currentTime;
		// input
		process_input(deltaTime/*, gamepad*/);

		// rendering
		screen.update();

		shader.activate();
		//shader.setFloat("mixVal", mixVal);
		//shader.set3Float("light.position", lamp.pos);
		shader.set3Float("viewPos", cameras[activeCam].cameraPos);

		dirLight.direction = glm::vec3(
			glm::rotate(glm::mat4(1.0f), glm::radians(0.05f), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::vec4(dirLight.direction, 1.0f));
		dirLight.render(shader);

		for (int i = 0; i < 4; i++)
		{
			lamps[i].pointLight.render(shader, i);
		}
		shader.setInt("noPointLights", 4);

		if (flashLightOn)
		{
			spotLight.position = cameras[activeCam].cameraPos;
			spotLight.direction = cameras[activeCam].cameraFront;
			spotLight.render(shader, 0);
			shader.setInt("noSpotLights", 1);
		}
		else
		{
			shader.setInt("noSpotLights", 0);
		}

		
		
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		view = cameras[activeCam].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		g.render(shader);

		lampShader.activate();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		for (int i = 0; i < 4; i++)
		{
			lamps[i].render(lampShader);
		}

		
		screen.newFrame();
	}

	g.cleanup();

	for (int i = 0; i < 4; i++)
	{
		lamps[i].cleanup();
	}


	glfwTerminate();

	return 0;
}

void process_input(/*GLFWwindow* window,*/ double dt/*, Gamepad& gamepad*/)
{
	//gamepad.update();

	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		//glfwSetWindowShouldClose(window, true);
		screen.setShouldClose(true);
	}

	if (Keyboard::keyWentDown(GLFW_KEY_L))
	{
		flashLightOn = !flashLightOn;
	}

	// Change mix value
	if (Keyboard::key(GLFW_KEY_UP))
	{
		mixVal += 0.0005f;
		if (mixVal > 1)
		{
			mixVal = 1.0f;
		}
	}
	else if (Keyboard::key(GLFW_KEY_DOWN))
	{
		mixVal -= 0.0005f;
		if (mixVal < 0)
		{
			mixVal = 0.0f;
		}
	}

	if (Keyboard::keyWentDown(GLFW_KEY_TAB))
	{
		activeCam += (activeCam == 0) ? 1 : -1;
	}

	if (Keyboard::key(GLFW_KEY_W))
	{
		//y += 0.0005;
		//camera.updateCameraPos(CameraDirection::FORWARD, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::FORWARD, dt);
	}
	else if (Keyboard::key(GLFW_KEY_S))
	{
		//y -= 0.0005;
		//camera.updateCameraPos(CameraDirection::BACKWARD, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_A))
	{
		//x -= 0.0005;
		//camera.updateCameraPos(CameraDirection::LEFT, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::LEFT, dt);
	}
	else if (Keyboard::key(GLFW_KEY_D))
	{
		//x += 0.0005;
		//camera.updateCameraPos(CameraDirection::RIGHT, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_SPACE))
	{
		//y += 0.0005;
		//camera.updateCameraPos(CameraDirection::UP, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::UP, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_CONTROL))
	{
		//y += 0.0005;
		//camera.updateCameraPos(CameraDirection::DOWN, dt);
		cameras[activeCam].updateCameraPos(CameraDirection::DOWN, dt);
	}

	double dx = Mouse::getDX(), dy = Mouse::getDY();
	if (dx != 0 || dy != 0)
	{
		//camera.updateCameraDirection(dx, dy);
		cameras[activeCam].updateCameraDirection(dx, dy);
	}

	double scrollDy = Mouse::getScrollDY();
	if (scrollDy != 0)
	{
		//camera.updateCameraZoom(scrollDy);
		cameras[activeCam].updateCameraZoom(scrollDy);
	}
	/*if (Keyboard::key(GLFW_KEY_W))
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.01f, 0.0f));

	}
	else if (Keyboard::key(GLFW_KEY_S))
	{
		transform = glm::translate(transform, glm::vec3(0.0f, -0.01f, 0.0f));

	}
	else if (Keyboard::key(GLFW_KEY_A))
	{
		transform = glm::translate(transform, glm::vec3(-0.01f, 0.0f, 0.0f));

	}
	else if (Keyboard::key(GLFW_KEY_D))
	{
		transform = glm::translate(transform, glm::vec3(0.01f, 0.0f, 0.0f));

	}*/

	/*
	if (gamepad.isButtonPressed(GLFW_GAMEPAD_BUTTON_A))
	{
		std::cout << "Button A pressed" << std::endl;
	}

	float leftStickX = gamepad.getAxisValue(GLFW_GAMEPAD_AXIS_LEFT_X);
	float leftStickY = gamepad.getAxisValue(GLFW_GAMEPAD_AXIS_LEFT_Y);

	std::cout << "Left Stick X: " << leftStickX << ", Y: " << leftStickY << std::endl;
	*/
}