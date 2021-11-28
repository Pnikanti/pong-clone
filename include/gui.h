#pragma once
#include <imgui.h>
 
namespace OpenGL { class GraphicsComponent; }
class PhysicsComponent;
class Entity;
class Game;

namespace OpenGL {
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
		float fCurrentColor[3] = { 255.0f, 255.0f, 0.0f };
		float fSize = 0.5f;
		float fDensity = 0.5f;
		float fFriction = 0.5f;
		float fRestitution = 0.5f;
		float fRotation = 30.0f;
	};
}