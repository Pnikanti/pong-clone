#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "guimanager.h"
#include "gui.h"
#include "context.h"

namespace OpenGL {
	GuiManager::~GuiManager()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiManager::Init(size_t size)
	{
		FirstDraw = true;
		Context = ImGui::CreateContext();
		IMGUI_CHECKVERSION();
		ImGui::SetCurrentContext(Context);
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::StyleColorsDark();
		GuiContexts.reserve(size);
	}

	void GuiManager::Draw()
	{
		// hack to get ImGui working
		if (FirstDraw)
		{
			ImGui_ImplGlfw_InitForOpenGL(Context::Window, true);
			ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
			FirstDraw = false;
		}

		Begin();
		for (auto i : GuiContexts)
		{
			if (i != nullptr)
			{
				i->Update();
			}
		}
		End();
	}

	void GuiManager::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void GuiManager::End()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	std::vector<GuiContext*>& GuiManager::GetContexts()
	{
		return GuiContexts;
	}
}