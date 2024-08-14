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

#include "Graphics/Models/Cube.hpp"

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(/*GLFWwindow* window,*/ double dt/*, Gamepad& gamepad*/);

Screen screen;

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);

Camera cameras[2] = {
	Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
	Camera(glm::vec3(10.0f, 10.0f, 10.0f))
};
int activeCam = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;
float x, y, z;


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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	*/

	/*
	* Shaders
	*/

	// compile vertex shader
	Shader shader("assets/object.vs", "assets/object.fs");

	Cube cube(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	cube.init();

	/*
	


	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO 28:49
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// GL_STATIC_DRAW就像是告诉Opengl把这段VBO放到一个静态区域，或者这片内存区不会被经常访问
	// GL_DYNAMIC_DRAW就正好相反
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up EBO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set Attribute Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Load and create a texture
	//unsigned int texture1, texture2;

	Texture texture1("assets/obama10.jpg", "texture1");
	texture1.load();
	Texture texture2("assets/americanflag1_.png", "texture2");
	texture2.load();

	shader.activate();
	shader.setInt("texture1", texture1.id);
	shader.setInt("texture2", texture2.id);
	*/
	
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
		shader.setFloat("mixVal", mixVal);
		//glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
		//std::cout << "mixVal: " << glGetUniformLocation(shader.id, "texture0") << std::endl;
		//std::cout << "mixVal: " << glGetUniformLocation(shader.id, "texture1") << std::endl;

		/*
		texture1.bind();
		glActiveTexture(GL_TEXTURE0);


		glActiveTexture(GL_TEXTURE1);
		texture2.bind();
		*/
		//glBindTexture(GL_TEXTURE_2D, texture2.id);
		//glBindTexture(GL_TEXTURE_2D, texture1);

		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 300.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		//shader.activate();
		//shader.setMat4("transform", trans);

		// create tranformation for screen
		//glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-55.0f), glm::vec3(0.5f));
		//view = glm::translate(view, glm::vec3(-x, -y, -z));
		//view = camera.getViewMatrix();
		view = cameras[activeCam].getViewMatrix();
		projection = glm::perspective(glm::radians(cameras[activeCam].getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		

		// Draw shapes
		//glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//shader.activate();

		//shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		cube.render(shader);
		//shader.setMat4("transform", transform);
		//glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(0);

		// send new frame to window
		screen.newFrame();
	}

	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);


	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
}

void process_input(/*GLFWwindow* window,*/ double dt/*, Gamepad& gamepad*/)
{
	//gamepad.update();

	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		//glfwSetWindowShouldClose(window, true);
		screen.setShouldClose(true);
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