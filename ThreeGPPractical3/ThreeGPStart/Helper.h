#pragma once

#include <glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Helpers
{
	GLFWwindow* CreateGLFWWindow(int width, int height, const std::string& title);

	// Loads a whole file into a string e.g. for shaders
	std::string stringFromFile(std::string filepath);
	void GlErrorCheck();

	bool DidShaderCompileOK(GLuint id);

	bool LinkProgramShaders(GLuint shaderProgram);

	// Returns 0 on error
	GLuint LoadAndCompileShader(GLenum shaderType, const std::string& shaderFilename);
};

