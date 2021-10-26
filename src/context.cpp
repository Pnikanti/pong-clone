#include <iostream>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include "context.h"
#include "shader.h"
#include "gameObject.h"
#include "game.h"

unsigned int OpenGLContext::shader = 0;
GLFWwindow* OpenGLContext::window = nullptr;

OpenGLContext::OpenGLContext(const char* windowName)
	: alive(1)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, windowName, NULL, NULL);

	if (!window)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCb);

	GLenum glewInitialized = glewInit();

	if (glewInitialized == GLEW_OK)
	{
		std::cout << "Glew Version: " << glewGetString(GLEW_VERSION) << std::endl;
	}
	else 
	{
		std::cout << "Glew Error: " << glewGetErrorString(glewInitialized) << std::endl;
	}

	glViewport(0, 0, 800, 600);
}

OpenGLContext::~OpenGLContext()
{
	glDeleteProgram(shader);
	glfwTerminate();
}

void OpenGLContext::FrameBufferSizeCb(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OpenGLContext::UseShader(std::string& vertexShader, std::string& fragmentShader)
{
	std::cout << "Using shaders: " << vertexShader << ", " << fragmentShader << std::endl;
	ShaderHandler::ShaderProgramSource source = ShaderHandler::Parse(vertexShader, fragmentShader);
	shader = ShaderHandler::Create(source.VertexSource, source.FragmentSource);
}

void OpenGLContext::UpdateAllRenderTargets()
{
	for (auto i : Game::gameObjects)
	{
		if (i != nullptr) {
			i->Draw();
		}
	}
}
void OpenGLContext::RenderOneFrame()
{
	alive = !glfwWindowShouldClose(window);
	glClear(GL_COLOR_BUFFER_BIT);
	UpdateAllRenderTargets();
	glfwSwapBuffers(window);
	glfwPollEvents();
}