#pragma once
#include <glfw3.h>
#include <glm.hpp>
#include <string>
#include <unordered_map>

class GameObject;
class OrthographicCamera;

class OpenGLContext {
public:
	int alive;
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;
	static GLFWwindow* window;
	static std::unordered_map<std::string, unsigned int> shaders;
public:
	OpenGLContext(const char* windowName, int width, int height);
	~OpenGLContext();
	void RenderOneFrame();
	void AddShader(std::string& shaderName, std::string& vertexShader, std::string& fragmentShader);
	void UpdateViewProjectionMatrix(OrthographicCamera* camera);
	void UpdateUniformResolution();
private:
	static void FrameBufferSizeCb(GLFWwindow* window, int width, int height);
	void UpdateAllRenderTargets();
private:
	glm::mat4 viewProjectionMatrix;
};