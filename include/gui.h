#pragma once
#include <imgui.h>
 
namespace OpenGL { class GraphicsComponent; }
class PhysicsComponent;
class Entity;
class Game;

namespace OpenGL {
	class ImGuiExtender {
	public:
		static void TextCenter(const char* text, ...)
		{
			va_list args = nullptr;
			va_start(args, text);
			float fontSize = ImGui::GetFontSize() * strlen(text) / 2;
			ImGui::SameLine(
				ImGui::GetWindowSize().x / 2 -
				fontSize + (fontSize / 2)
			);
			ImGui::TextV(text, args);
			ImGui::NewLine();
			va_end(args);
		}
	};

	class GuiContext {
	public:
		virtual void Update();
		virtual void Update(Entity& entity);
	};

	class DebugGuiContext : public GuiContext {
	public:
		DebugGuiContext();
		void Update() override;
	public:
		ImGuiWindowFlags wFlags;
		bool visible;
	};

	class GameGuiContext : public GuiContext {
	public:
		GameGuiContext();
		void Update() override;
	public:
		ImGuiWindowFlags wFlags;
		ImGuiWindowFlags wBackgroundFlags;
		bool visible;
	private:
		ImFont* gameFontP;
		ImFont* gameFontH1;
		ImFont* gameFontH2;
	};
}