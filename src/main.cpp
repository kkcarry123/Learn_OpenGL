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

#include "Shader.h"

#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/Joystick.h"

/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

float mixVal = 0.5f;

glm::mat4 transform = glm::mat4(1.0f);
Joystick mainJ(0);

int main()
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "My OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Focus to this glfw window
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to load Glad" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	/*
	* Shaders
	*/

	// compile vertex shader
	Shader shader("assets/Vertex_core.glsl", "assets/Fragment_core.glsl");
	//Shader shader2("assets/Vertex_core.glsl", "assets/Fragment_core2.glsl");
	

	float vertices[] = {
		// Position				Colors				Texture Coordinates
		-0.5f, -0.5f,  0.0f,	1.0f, 1.0f, 0.5f,	0.0f, 0.0f, //botttom left
		-0.5f,  0.5f,  0.0f,	0.5f, 1.0f, 0.75f,  0.0f, 1.0f, //bottom right
		 0.5f,  -0.5f, 0.0f,	0.6f, 1.0f, 0.2f,   1.0f, 0.0f, //top left
		 0.5f,	0.5f,  0.0f,	1.0f, 0.2f, 1.0f,   1.0f, 1.0f  //top riht

	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 1, 2
	};



	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO 28:49
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// GL_STATIC_DRAW就像是告诉Opengl把这段VBO放到一个静态区域，或者这片内存区不会被经常访问
	// GL_DYNAMIC_DRAW就正好相反
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Set Attribute Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Load and create a texture
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	//unsigned char* data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
	unsigned char* data = stbi_load("assets/obama10.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shader.activate();
	//glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
	shader.setInt("texture1", 0);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load("assets/americanflag1_.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data);
	shader.setInt("texture2", 1);

	
	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//trans = glm::scale(trans, glm::vec3(0.5f, 1.5f, 0.5f));
	//shader.activate();
	//shader.setMat4("transform", trans);
	

	/*
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = glm::scale(trans2, glm::vec3(1.5f));
	trans2 = glm::rotate(trans2, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader2.activate();
	shader2.setMat4("transform", trans2);
	*/
	//glUseProgram(shaderPrograms[0]);
	//glUniformMatrix4fv(glGetUniformLocation(shaderPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(trans));

	mainJ.update();
	if (mainJ.isPresent())
	{
		std::cout << mainJ.getName() << "is present!!" << std::endl;
	}
	else
	{
		std::cout << "joystick not present!!" << std::endl;
	}


	while (!glfwWindowShouldClose(window))
	{
		// 2024/08/01看到了Tutorial 8 - Shaders/Transformations的15:03
		// input
		process_input(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//glBindTexture(GL_TEXTURE_2D, texture1);

		//trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 300.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		//glUniformMatrix4fv(glGetUniformLocation(shader.id, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		//shader.activate();
		//shader.setMat4("transform", trans);
		

		// Draw shapes
		glBindVertexArray(VAO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		shader.activate();
		shader.setFloat("mixVal", mixVal);
		shader.setMat4("transform", transform);
		glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
		// First triangle
		//glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glUniform1i(glGetUniformLocation(shader.id, "texture1"), 0);
		//glUniform1i(glGetUniformLocation(shader.id, "texCoord"), 1);
		//printf("%d \n", glGetUniformLocation(shader.id, "texture1"));
		

		//trans2 = glm::rotate(trans2, glm::radians((float)glfwGetTime() / -100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//shader2.activate();
		//shader2.setMat4("transform", trans2);

		//shader2.activate();
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));

		// Second Triangle
		//glUseProgram(shaderPrograms[1]);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(window, true);
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

	mainJ.update();

	float lx = mainJ.axesStates(GLFW_JOYSTICK_AXES_LEFT_STICK_X);
	float ly = -mainJ.axesStates(GLFW_JOYSTICK_AXES_LEFT_STICK_Y);

	if (std::abs(lx) > 0.5f)
	{
		transform = glm::translate(transform, glm::vec3(lx / 10, 0.0f, 0.0f));

	}
	if (std::abs(ly) > 0.5f)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, ly / 10, 0.0f));
	}

}