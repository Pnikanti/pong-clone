#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <box2d/b2_body.h>
#include "gui.h"
#include "entitymanager.h"
#include "entity.h"
#include "context.h"
#include "log.h"
#include "graphics.h"
#include "physics.h"

namespace OpenGL {
	Gui::Gui() 
		: Context(ImGui::CreateContext())
	{
		IMGUI_CHECKVERSION();
		ImGui::SetCurrentContext(Context);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Context::Window, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
	}

	Gui::~Gui() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Gui::Begin() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void Gui::End() {
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GuiContext::Update() {}

	DebugGuiContext::DebugGuiContext() : wFlags(0), visible(true)
	{
		wFlags |= ImGuiWindowFlags_NoBackground;
		wFlags |= ImGuiWindowFlags_NoTitleBar;
		wFlags |= ImGuiWindowFlags_NoResize;
		wFlags |= ImGuiWindowFlags_NoMove;
		LOGGER_TRACE("WindowFlags: {0}", wFlags);
	}

	GameGuiContext::GameGuiContext() : wFlags(0), wBackgroundFlags(0), visible(true)
	{
		wFlags |= ImGuiWindowFlags_NoBackground;
		wFlags |= ImGuiWindowFlags_NoTitleBar;
		wFlags |= ImGuiWindowFlags_NoResize;
		wFlags |= ImGuiWindowFlags_NoMove;
		wBackgroundFlags |= ImGuiWindowFlags_NoTitleBar;
		wBackgroundFlags |= ImGuiWindowFlags_NoResize;
		wBackgroundFlags |= ImGuiWindowFlags_NoMove;
		LOGGER_TRACE("WindowFlags: {0}", wFlags);
		LOGGER_TRACE("WindowBackgroundFlags: {0}", wBackgroundFlags);

	}

	void DebugGuiContext::Update() {
		ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH - 350, 0)); // top-left
		ImGui::SetNextWindowSize(ImVec2(350, 200));
		ImGui::Begin("Debug", &visible, wFlags);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Total Allocated (bytes): %u", Context::AllocatedMemory);
		ImGui::Text("Freed (bytes): %u", Context::FreedMemory);
		ImGui::Text("Current (bytes): %u", Context::AllocatedMemory - Context::FreedMemory);
		ImGui::Text("Entity vector size: %u", EntityManager::GetEntities().size());
		ImGui::End();
	}

	void GameGuiContext::Update()
	{
	}
}