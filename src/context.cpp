
#include <iostream>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/string_cast.hpp>
#include "context.h"
#include "shader.h"
#include "gameObject.h"
#include "game.h"
#include "camera.h"

unsigned int OpenGLContext::SCR_WIDTH = 800;
unsigned int OpenGLContext::SCR_HEIGHT = 600;
unsigned int OpenGLContext::shader = 0;
GLFWwindow* OpenGLContext::window = nullptr;

OpenGLContext::OpenGLContext(const char* windowName, int width, int height)
	: alive(1), viewProjectionMatrix(glm::mat4(1.0f))
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, NULL, NULL);

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

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

OpenGLContext::~OpenGLContext()
{
	glDeleteProgram(shader);
	glfwTerminate();
}

void OpenGLContext::FrameBufferSizeCb(GLFWwindow* window, int width, int height)
{
	SCR_WIDTH = width;
	SCR_HEIGHT = height;
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void OpenGLContext::UseShader(std::string& vertexShader, std::string& fragmentShader)
{
	std::cout << "Using shaders: " << vertexShader << ", " << fragmentShader << std::endl;
	ShaderHandler::ShaderProgramSource source = ShaderHandler::Parse(vertexShader, fragmentShader);
	shader = ShaderHandler::Create(source.VertexSource, source.FragmentSource);
}

void OpenGLContext::UpdateViewProjectionMatrix(OrthographicCamera* camera)
{
	std::cout << "viewProjectionMatrix: " << glm::to_string(camera->viewProjectionMatrix) << std::endl;
	viewProjectionMatrix = camera->viewProjectionMatrix;
	glUseProgram(shader);
	unsigned int viewProjectionUniform = glGetUniformLocation(shader, "viewProjection");
	glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	UpdateAllRenderTargets();
	glfwSwapBuffers(window);
	glfwPollEvents();
}