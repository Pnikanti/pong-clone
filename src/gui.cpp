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
		ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH - 350, 0));
		ImGui::SetNextWindowSize(ImVec2(350, 200));
		ImGui::Begin("Debug", &visible, wFlags);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Total Allocated (bytes): %u", Context::AllocatedMemory);
		ImGui::Text("Freed (bytes): %u", Context::FreedMemory);
		ImGui::Text("Current (bytes): %u", Context::AllocatedMemory - Context::FreedMemory);
		ImGui::Text("Entity vector size: %u", EntityManager::Get().GetEntities().size());
		ImGui::End();
	}

	void GameGuiContext::Update()
	{
		switch (Game::State)
		{
			case GameState::MainMenu:
			{
				ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH / 2, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2(500, 400));
				ImGui::Begin("Main Menu", &visible, wFlags);
				ImGui::Text("Pong");
				ImGui::Text("Press \"Space\" to play");
				ImGui::End();
				break;
			}
			case GameState::GameOver:
			{
				ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH / 2, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2(500, 400));
				ImGui::Begin("Game over", &visible, wFlags);
				ImGui::Text("Game over");
				ImGui::Text("Press \"Space\" to play");
				ImGui::End();
				break;
			}
			case GameState::Pause:
			{
				ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH / 2, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2(500, 400));
				ImGui::Begin("Pause", &visible, wFlags);
				ImGui::Text("Pause");
				ImGui::Text("Press \"Space\" to play");
				ImGui::End();
				break;
			}
			case GameState::Play:
			{
				ImGui::SetNextWindowPos(ImVec2(50, 50));
				ImGui::SetNextWindowSize(ImVec2(200, 200));
				ImGui::Begin("Left player", &visible, wFlags);
				ImGui::Text("Score: 0");
				ImGui::End();
				ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH - 250, 50));
				ImGui::SetNextWindowSize(ImVec2(200, 200));
				ImGui::Begin("Right player", &visible, wFlags);
				ImGui::Text("Score: 0");
				ImGui::End();
				break;
			}
		}
	}
}