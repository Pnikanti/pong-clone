#pragma once
#include <glfw3.h>
#include <glm.hpp>
#include <string>

class GameObject;
class OrthographicCamera;

class OpenGLContext {
public:
	int alive;
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static unsigned int shader;
	static GLFWwindow* window;
public:
	OpenGLContext(const char* windowName, int width, int height);
	~OpenGLContext();
	void RenderOneFrame();
	void UseShader(std::string& vertexShader, std::string& fragmentShader);
	void UpdateViewProjectionMatrix(OrthographicCamera* camera);
private:
	static void FrameBufferSizeCb(GLFWwindow* window, int width, int height);
	void UpdateAllRenderTargets();
private:
	glm::mat4 viewProjectionMatrix;
};