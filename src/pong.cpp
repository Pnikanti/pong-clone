#include <iostream>
#include <glew.h>
#include <glfw3.h>

class OpenGLContext {
public:
	OpenGLContext();
	void StartRendering();
private:
	GLFWwindow* window;
	static void FrameBufferSizeCb(GLFWwindow* window, int width, int height);
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

void OpenGLContext::FrameBufferSizeCb(GLFWwindow* window, int width, int height)
{
	std::cout << "Resizing window. Width: " << width << ", height: " << height << std::endl;
	glViewport(0, 0, width, height);
}

void OpenGLContext::StartRendering()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}
int main()
{
	OpenGLContext context = OpenGLContext();
	context.StartRendering();
	return 0;
}