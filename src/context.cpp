#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "context.h"
#include "gui.h"
#include "camera.h"
#include "game.h"
#include "log.h"
#include "shader.h"
#include "entity.h"
#include "entitymanager.h"

void* operator new(size_t size)
{
	OpenGL::Context::AllocatedMemory += size;
	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	OpenGL::Context::FreedMemory += size;
	free(memory);
}

namespace OpenGL {
	unsigned int Context::AllocatedMemory = 0;
	unsigned int Context::FreedMemory = 0;
	unsigned int Context::SCR_WIDTH = 800;
	unsigned int Context::SCR_HEIGHT = 600;
	glm::mat4 Context::viewProjectionMatrix = glm::mat4(0.0f);
	OrthographicCamera* Context::Camera = nullptr;
	GLFWwindow* Context::Window = nullptr;
	Gui* Context::GuiContext = nullptr;
	std::unordered_map<std::string, unsigned int> Context::Shaders = std::unordered_map<std::string, unsigned int>();

	Context::Context(int width, int height, const char* windowName)
		: Alive(1)
	{
		SCR_WIDTH = width;
		SCR_HEIGHT = height;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, windowName, NULL, NULL);

		if (!Window)
		{
			glfwTerminate();
		}

		glfwMakeContextCurrent(Window);
		glfwSetFramebufferSizeCallback(Window, FrameBufferSizeCb);
		GLenum glewInitialized = glewInit();

		if (glewInitialized == GLEW_OK)
		{
			LOGGER_INFO("Glew version: {0}", glewGetString(GLEW_VERSION));
		}
		else
		{
			LOGGER_ERROR("Glew error: {0}", glewGetErrorString(glewInitialized));
		}

		glfwSwapInterval(1); // Enable vsync
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		GuiContext = new Gui();
		LOGGER_INFO("Context initialized!");
	}

	Context::~Context()
	{
		ImGui::DestroyContext();
		glfwTerminate();
	}

	void Context::FrameBufferSizeCb(GLFWwindow* window, int width, int height)
	{
		SCR_WIDTH = width;
		SCR_HEIGHT = height;

		if (Camera != nullptr)
		{
			delete Camera;
			Camera = nullptr;
		}

		Camera = CreateCamera((float)SCR_WIDTH, (float)SCR_HEIGHT);
		UpdateViewProjectionMatrix(Camera);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	}

	void Context::RenderGui()
	{
		GuiContext->Begin();
		for (auto i : Game::GuiContexts)
		{
			if (i != nullptr)
			{
				i->Update();
			}
		}
		GuiContext->End();
	}

	void Context::AddShader(std::string& shaderName, std::string& vertexShader, std::string& fragmentShader)
	{
		LOGGER_INFO("Using shaders: {0}, {1}", vertexShader, fragmentShader);
		ShaderHandler::ShaderProgramSource source = ShaderHandler::Parse(vertexShader, fragmentShader);
		Shaders[shaderName] = ShaderHandler::Create(source.VertexSource, source.FragmentSource);
	}

	void Context::Start()
	{
		if (Camera != nullptr)
		{
			delete Camera;
			Camera = nullptr;
		}

		Camera = CreateCamera((float)SCR_WIDTH, (float)SCR_HEIGHT);
		UpdateViewProjectionMatrix(Camera);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	}

	void Context::UpdateViewProjectionMatrix(OrthographicCamera* camera)
	{
		LOGGER_TRACE("Updating viewProjectionmatrix: {0}", glm::to_string(camera->viewProjectionMatrix));
		viewProjectionMatrix = camera->viewProjectionMatrix;

		for (auto i : Shaders)
		{
			glUseProgram(i.second);
			unsigned int viewProjectionUniform = glGetUniformLocation(i.second, "viewProjection");
			glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(viewProjectionMatrix));
		}
	}

	void Context::UpdateAllRenderTargets()
	{
		for (auto i : EntityManager::GetEntities())
		{
			if (i != nullptr)
				i->Draw();
		}
	}

	OrthographicCamera* Context::CreateCamera(float width, float height)
	{
		float aspectRatio = width / height;
		float cameraDimensions = 20.0f;
		float bottom = -cameraDimensions;
		float top = cameraDimensions;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;

		LOGGER_TRACE("aspectRatio: {0}", aspectRatio);
		LOGGER_TRACE("bottom: {0}", bottom);
		LOGGER_TRACE("top: {0}", top);
		LOGGER_TRACE("left: {0}", left);
		LOGGER_TRACE("right: {0}", right);

		return new OrthographicCamera(left, right, bottom, top);
	}

	void Context::RenderOneFrame()
	{
		Alive = !glfwWindowShouldClose(Window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		UpdateAllRenderTargets();
		RenderGui();
		glfwSwapBuffers(Window);
		glfwPollEvents();
	}
}