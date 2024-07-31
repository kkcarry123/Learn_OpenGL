#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

/*
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main()
{
	int success;
	char infoLog[512];
	
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

	/*
	* Shaders
	*/

	// compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSrc = loadShaderSrc("assets/Vertex_core.glsl");
	const GLchar* vertShader = vertexShaderSrc.c_str();
	glShaderSource(vertexShader, 1, &vertShader, NULL);
	glCompileShader(vertexShader);

	// catch error
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error with vertex Shder Comp.: " << std::endl << infoLog << std::endl;
	}

	unsigned int fragmentShaders[2];

	fragmentShaders[0] = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSrc = loadShaderSrc("assets/Fragment_core.glsl");
	const GLchar* fragShader = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShaders[0], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[0]);

	// catch error
	glGetShaderiv(fragmentShaders[0], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[0], 512, NULL, infoLog);
		std::cout << "Error with Fragment Shder Comp.: " << std::endl << infoLog << std::endl;
	}

	fragmentShaders[1] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShaderSrc = loadShaderSrc("assets/Fragment_core2.glsl");
	fragShader = fragmentShaderSrc.c_str();
	glShaderSource(fragmentShaders[1], 1, &fragShader, NULL);
	glCompileShader(fragmentShaders[1]);

	// catch error
	glGetShaderiv(fragmentShaders[1], GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaders[1], 512, NULL, infoLog);
		std::cout << "Error with Fragment Shder Comp.: " << std::endl << infoLog << std::endl;
	}

	// Create Shader
	unsigned int shaderPrograms[2];

	shaderPrograms[0] = glCreateProgram();

	glAttachShader(shaderPrograms[0], vertexShader);
	glAttachShader(shaderPrograms[0], fragmentShaders[0]);
	glLinkProgram(shaderPrograms[0]);

	// catch errors
	glGetProgramiv(shaderPrograms[0], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderPrograms[0], 512, NULL, infoLog);
		std::cout << "Linking error: " << std::endl << infoLog << std::endl;
	}

	shaderPrograms[1] = glCreateProgram();

	glAttachShader(shaderPrograms[1], vertexShader);
	glAttachShader(shaderPrograms[1], fragmentShaders[1]);
	glLinkProgram(shaderPrograms[1]);

	// catch errors
	glGetProgramiv(shaderPrograms[1], GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderPrograms[1], 512, NULL, infoLog);
		std::cout << "Linking error: " << std::endl << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaders[0]);
	glDeleteShader(fragmentShaders[1]);

	// Vertex Array
	//float vertices[] =
	//{
	//	0.5f,	0.5f, 0.0f, // top right
	//   -0.5f,	0.5f, 0.0f, // top left
	//   -0.5f,  -0.5f, 0.0f, // bottom left
	//	0.5f,  -0.5f, 0.0f // bottom right
	//	
	//};
	//
	//int indices[] =
	//{
	//	0, 1, 2,	// first triangle
	//	2, 3, 0		// second triangle
	//};

	float vertices[] = {
		// First Triangle
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		-0.1f, -0.5f, 0.0f,

		// Second Triangle
		0.5f, -0.5f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.1f, -0.5f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5
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

	// Set Attribute Pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Set up EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	while (!glfwWindowShouldClose(window))
	{
		// 2024/07/31看到了Tutorial 7 - Displaying Shapes的59:50 
		// input
		process_input(window);

		// rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw shapes
		glBindVertexArray(VAO);

		// First triangle
		glUseProgram(shaderPrograms[0]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		// Second Triangle
		glUseProgram(shaderPrograms[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));

		// send new frame to window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VAO);
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Could not open: " << filename << std::endl;
	}
	
	file.close();

	return ret;

}