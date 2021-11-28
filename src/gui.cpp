#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <box2d/b2_body.h>
#include "gui.h"
#include "entitymanager.h"
#include "entity.h"
#include "context.h"
#include "log.h"
#include "graphics.h"
#include "physics.h"
#include "game.h"

namespace OpenGL {
	void GuiContext::Update() {}
	void GuiContext::Update(Entity& entity) {}
	void GuiContext::Update(Game& game) {}

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
		ImGui::Text("Entity vector size: %u", EntityManager::Get().GetEntities().size());
		ImGui::End();
	}

	void GameGuiContext::Update(Game& game)
	{
		if (game.State == GameState::MainMenu)
		{
			ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH / 2, (int)Context::SCR_HEIGHT / 2)); // top-left
			ImGui::SetNextWindowSize(ImVec2(500, 400));
			ImGui::Begin("Main Menu", &visible, wFlags);
			ImGui::Text("Pong");
			ImGui::Text("Press \"Space\" to play");
			ImGui::End();
		}
	}
}