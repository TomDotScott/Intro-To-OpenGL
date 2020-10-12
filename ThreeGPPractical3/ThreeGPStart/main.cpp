// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Glew is a library that handles OpenGL extensions for us
#include <glew.h>

// GLFW is a library to handle OpenGL window setup as well as input
#include <GLFW/glfw3.h>

// GLM is a maths library
#include <glm\glm.hpp>

// Helpers written by Keith to make the initial learning easier
#include "Helper.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

// Forward declarations
bool InitialiseGeometry();
void Render();

// Globals
GLuint gProgram;
GLuint gVAO;

int main()
{
	// Use the helper function to set up GLFW, GLEW and OpenGL
	GLFWwindow* window = Helpers::CreateGLFWWindow(1024, 768, "Simple example");
	if (!window)
		return -1;

	// Create our geoemetry
	if (!InitialiseGeometry())
		glfwTerminate();

	// Enter main loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{		
		// Render each time around the loop
		Render();
		
		// GLFW updating
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

// Create geometry
bool InitialiseGeometry()
{
	// Create some vertices
	std::vector<GLfloat> vertices =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,

		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	/*
		Create a Vertex Buffer Object (VBO) to hold vertex positions
	*/
	
	GLuint positionsVBO;	
	
	// Generate 1 buffer id (name), put the resulting identifier in positionsVBO variable
	glGenBuffers(1, &positionsVBO);

	// Bind the buffer to the context at the GL_ARRAY_BUFFER binding point (target). 
	// This is the first time this buffer is used so this also creates the buffer object.
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// Fill the bound buffer with the vertices, we pass the size in bytes and a pointer to the data
	// the last parameter is a hint to OpenGL that we will not alter the vertices again
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Clear binding - not absolutely required but a good idea!
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Load and create vertex and fragment shaders
	GLuint vertex_shader = Helpers::LoadAndCompileShader(GL_VERTEX_SHADER,"Data/Shaders/vertex_shader.glsl");
	GLuint fragment_shader = Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER,"Data/Shaders/fragment_shader.glsl");
	if (vertex_shader == 0 || fragment_shader == 0)
		return false;

	/*
		Create a Program object to hold the compiled shaders
	*/

	// Create a new program (returns a unqiue id)
	gProgram = glCreateProgram();

	// Attach the vertex shader to this program (copies it)
	glAttachShader(gProgram, vertex_shader);

	// The attibute 0 maps to the input stream "vertex_position" in the vertex shader
	// But not needed if you use (location=0) in the vertex shader itself. Just here to show you the old way.
	//glBindAttribLocation(gProgram, 0, "vertex_position");

	// Attach the fragment shader (copies it)
	glAttachShader(gProgram, fragment_shader);

	// Done with thes originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(gProgram))
		return false;
	
	/*
		Create a Vertex Array Object (VAO) to wrap or 'record' all bindings etc. needed to render
		As well as the make up of any streamed data
		Once we bind the VAO subsequent binds etc. are 'recorded' in the VAO. 
	*/

	// Create a unique id for a vertex array object
	glGenVertexArrays(1, &gVAO);

	// Bind it to be current and since first use this also allocates memory for it
	// Note no target binding point as there is only one type of vao
	glBindVertexArray(gVAO);

	// Bind the vertex buffer to the context (records this action in the VAO)
	glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);

	// Enable the first attribute in the program (the vertices) to stream to the vertex shader
	glEnableVertexAttribArray(0);

	// Describe the make up of the vertex stream
	glVertexAttribPointer(
		0,                  // attribute 0
		3,                  // size in components of each item in the stream e.g. a position has 3 components x,y and z
		GL_FLOAT,           // type of the item
		GL_FALSE,           // normalized or not (advanced)
		0,                  // stride (advanced)
		(void*)0            // array buffer offset (advanced)
	);

	// Clear VAO binding
	glBindVertexArray(0);

	return true;
} 

// Render the scene
void Render()
{
	// Cull (do not draw) triangles facing away from us
	glEnable(GL_CULL_FACE);

	// Uncomment to render in wireframe (can be useful when debugging)	
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Set the screen clear colour
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Clear the colour buffer (and the depth buffer)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use our program. Doing this enables the shaders we attached previously.
	glUseProgram(gProgram);
	 
	// Use our vertex shader object. This carries out the buffer binding and enabling of the attribute index recorded before.
	// In effect it replays it
	glBindVertexArray(gVAO);

	// Draw triangle primitives from the bound buffer data, starting from vertex 0 with 3 vertices in total
	glDrawArrays(GL_TRIANGLES, 0, 6); 
}