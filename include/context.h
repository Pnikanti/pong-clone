#pragma once
#include <glfw3.h>
#include <string>

class GameObject;

class OpenGLContext {
public:
	int alive;
	static unsigned int shader;
	static GLFWwindow* window;
public:
	OpenGLContext(const char* windowName);
	~OpenGLContext();
	void RenderOneFrame();
	void UseShader(std::string& vertexShader, std::string& fragmentShader);
private:
	static void FrameBufferSizeCb(GLFWwindow* window, int width, int height);
	void UpdateAllRenderTargets();
};