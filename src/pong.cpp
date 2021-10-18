#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include "shaderHandler.h"

class OpenGLContext {
public:
	OpenGLContext();
	void StartRendering();
private:
	inline void SetBGColor(float red, float green, float blue, float alpha);
	void DrawTriangle();
	void DrawRectangle();
	void InitTriangle();
	void InitRectangle();
	static void FrameBufferSizeCb(GLFWwindow* window, int width, int height);
	static void KeyCb(GLFWwindow* window, int key, int scancode, int action, int mods);
private:
	GLFWwindow* window;
	unsigned int shader;
	unsigned int vertexArrayObject;
	unsigned int vertexBufferObject;
	unsigned int elementBufferObject;
};

OpenGLContext::OpenGLContext()
	: window(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(800, 600, "OpenGLContext", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCb);
	glfwSetKeyCallback(window, KeyCb);

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
	InitRectangle();
}

void OpenGLContext::FrameBufferSizeCb(GLFWwindow* window, int width, int height)
{
	std::cout << "Resizing window. Width: " << width << ", height: " << height << std::endl;
	glViewport(0, 0, width, height);
}

void OpenGLContext::KeyCb(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, true);
		break;
	}
}

inline void OpenGLContext::SetBGColor(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
}

void OpenGLContext::DrawTriangle()
{
	glUseProgram(shader);
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void OpenGLContext::DrawRectangle()
{
	glUseProgram(shader);
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}
void OpenGLContext::InitTriangle()
{
	// X, Y, Z
	// Range is normalized i.e. from -1 to 1. E.g. width 0 would be 400 in 800 pixel width.
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	glGenVertexArrays(1, &vertexArrayObject); // Create n-amount of VAOs
	glBindVertexArray(vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject); // Create n-amount of VBOs
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // Bind buffer object to the OpenGL context so it can be used
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Store vertex data into VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Tell shaders where to get the data found in the VBO and how it should be intepreted.
	glEnableVertexAttribArray(0); // Enable array data in VBO

	ShaderHandler::ShaderProgramSource source = ShaderHandler::Parse("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shader = ShaderHandler::Create(source.VertexSource, source.FragmentSource);
}

void OpenGLContext::InitRectangle()
{
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bototm right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &elementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Tell shaders where to get the data found in the VBO and how it should be intepreted.
	glEnableVertexAttribArray(0); // Enable array data in VBO

	ShaderHandler::ShaderProgramSource source = ShaderHandler::Parse("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	shader = ShaderHandler::Create(source.VertexSource, source.FragmentSource);
}

void OpenGLContext::StartRendering()
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		SetBGColor(0.2f, 0.3f, 0.3f, 1.0f);
		DrawRectangle();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
}
int main()
{
	OpenGLContext context = OpenGLContext();
	context.StartRendering();
	return 0;
}