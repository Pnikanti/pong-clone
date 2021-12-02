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
		ImGui::GetIO().Fonts->AddFontDefault();

		gameFontP = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/fonts/bit5x3.ttf", 15.0f);
		gameFontH2 = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/fonts/bit5x3.ttf", 20.0f);
		gameFontH1 = ImGui::GetIO().Fonts->AddFontFromFileTTF("res/fonts/bit5x3.ttf", 40.0f);

		LOGGER_TRACE("WindowFlags: {0}", wFlags);
		LOGGER_TRACE("WindowBackgroundFlags: {0}", wBackgroundFlags);

	}

	void DebugGuiContext::Update() {
		ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT - 100));
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
			case GameState::Start:
			{
				ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2((int)Context::SCR_WIDTH, 300));
				ImGui::Begin("Start Sequence", &visible, wFlags);
				ImGui::PushFont(gameFontP);
				ImGuiExtender::TextCenter("Starting in: %i", Game::StartSequence[Game::SequenceIndex]);
				ImGui::PopFont();
				ImGui::End();
				break;
			}
			case GameState::MainMenu:
			{
				ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2((int)Context::SCR_WIDTH, 300));
				ImGui::Begin("Main Menu", &visible, wFlags);
				ImGui::PushFont(gameFontH1);
				ImGuiExtender::TextCenter("- Pong -");
				ImGui::PopFont();
				ImGui::NewLine();
				ImGui::PushFont(gameFontP);
				ImGuiExtender::TextCenter("Press \"Space\" to play");
				ImGuiExtender::TextCenter("Movement: \"WASD\", Pause: \"P\"");
				ImGui::PopFont();
				ImGui::End();

				ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT - 40));
				ImGui::SetNextWindowSize(ImVec2((int)Context::SCR_WIDTH, 80));
				ImGui::Begin("Version Notes", &visible, wFlags);
				ImGui::PushFont(gameFontP);
				// Hard-coded for now!
				ImGuiExtender::TextCenter("V0.4: Now with minor 8 bytes per game memory leaks! :)");
				ImGui::PopFont();
				ImGui::End();
				break;
			}
			case GameState::GameOver:
			{
				ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2((int)Context::SCR_WIDTH, 300));
				ImGui::Begin("Game over", &visible, wFlags);
				ImGui::PushFont(gameFontH2);
				ImGuiExtender::TextCenter("- Game over -");
				ImGui::PopFont();
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::PushFont(gameFontP);
				ImGuiExtender::TextCenter("Press \"Space\" to play");
				ImGui::PopFont();
				ImGui::End();
				break;
			}
			case GameState::Pause:
			{
				ImGui::SetNextWindowPos(ImVec2(0, (int)Context::SCR_HEIGHT / 2));
				ImGui::SetNextWindowSize(ImVec2((int)Context::SCR_WIDTH, 300));
				ImGui::Begin("Pause", &visible, wFlags);
				ImGui::PushFont(gameFontH2);
				ImGuiExtender::TextCenter("- Pause -");
				ImGui::PopFont();
				ImGui::NewLine();
				ImGui::NewLine();
				ImGui::PushFont(gameFontP);
				ImGuiExtender::TextCenter("Press \"Space\" to continue");
				ImGui::PopFont();
				ImGui::End();
				break;
			}
		}
		if (Game::State != GameState::MainMenu)
		{
			ImGui::SetNextWindowPos(ImVec2(50, 50));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
			ImGui::Begin("Left player", &visible, wFlags);
			ImGui::PushFont(gameFontH2);
			ImGui::Text("Score: %u", Game::ScoreLeftPlayer);
			ImGui::PopFont();
			ImGui::End();

			ImGui::SetNextWindowPos(ImVec2((int)Context::SCR_WIDTH - 250, 50));
			ImGui::SetNextWindowSize(ImVec2(200, 200));
			ImGui::Begin("Right player", &visible, wFlags);
			ImGui::PushFont(gameFontH2);
			ImGui::Text("Score: %u", Game::ScoreRightPlayer);
			ImGui::PopFont();
			ImGui::End();
		}
	}
}