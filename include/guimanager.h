#pragma once
#include <vector>

namespace OpenGL {
	class GuiContext;

	class GuiManager {
	public:
		GuiManager(const GuiManager&) = delete;
		~GuiManager();

		static GuiManager& Get() {
			static GuiManager Instance;
			return Instance;
		}

		void Init(size_t size);
		void Draw();
		void Begin();
		void End();
		std::vector<GuiContext*>& GetContexts();
	protected:
		GuiManager::GuiManager() = default;
	private:
		ImGuiContext* Context;
		std::vector<GuiContext*> GuiContexts;
		bool FirstDraw;
	};
}