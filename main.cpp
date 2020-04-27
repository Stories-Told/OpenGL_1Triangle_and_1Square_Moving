#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <fstream>
#include "readShaders.h"

using namespace std;

#define numVAOs 2

GLuint renderingProgram;
GLuint renderingProgram2;
GLuint vao[numVAOs];

// Create the shader program (vertex and fragment)
// Use this shader for following the book
GLuint createShaderProgram()
{
	// Create the vertex and fragment shaders
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	string vertShaderStr = readShaderSource("vertShader.glsl");
	string fragShaderStr = readShaderSource("fragShader.glsl");

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	// Load and Complie the vertex and fragment shader
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	// Create a program and then attach and link the vertex and fragment shader
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	return vfProgram;
}

// Use this shader for practice
GLuint createShaderProgram2()
{
	// Create the vertex and fragment shaders
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	string vertShaderStr = readShaderSource("vertShader2.glsl");
	string fragShaderStr = readShaderSource("fragShader2.glsl");

	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	// Load and compile the shaders
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	glCompileShader(fShader);

	// Create a program and link the shaders
	GLuint vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);

	return vfProgram;
}


void init(GLFWwindow* window)
{
	// First square
	renderingProgram = createShaderProgram();
	// Generate and bind the first vertex shader
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	// Second square
	renderingProgram2 = createShaderProgram2();
	// Generate and bind the second vertex shader
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[1]);
}

// Variables offsetting the triangles in display()
float x = 0.0f; // First triangle : moves right and left
float y = 0.0f; // First square : moves up and down
float inc = 0.01f;
// Displays the window
void display(GLFWwindow* window, double currentTime)
{
	// Clears the background to black each frame
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// First triangle
	glUseProgram(renderingProgram); // Just loads the shaders onto the hardware
	
	// Move the triangle
	x += inc;
	if (x > 1.0f) inc = -0.01f;
	if (x < -1.0f) inc = 0.01f;
	GLuint offsetLocX = glGetUniformLocation(renderingProgram, "offset");
	glProgramUniform1f(renderingProgram, offsetLocX, x);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Second square
	glUseProgram(renderingProgram2);

	// Move the square
	y += inc;
	if (y > 1.0f) inc = -0.01f;
	if (y < -1.0f) inc = 0.01f;
	GLuint offsetLocY = glGetUniformLocation(renderingProgram2, "offset");
	glProgramUniform1f(renderingProgram2, offsetLocY, y);
	glPointSize(30.0f); // Increases the single vertex size
	glDrawArrays(GL_POINTS, 0, 1);

	// Display wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

int main(void)
{
	// Check to make sure glfw initilized
	if (!glfwInit()) { exit(EXIT_FAILURE); }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(600, 200, "CGP_00", NULL, NULL);
	glfwMakeContextCurrent(window);
	// Check to make sure glew initilized
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1); // Enables v-sync

	// Initilize the window
	init(window);

	// While loop to keep the window open
	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window); // Paints the screen
		glfwPollEvents(); // Handles key events like key presses
	}

	// Close the window
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}