#pragma once
#include <vector>

namespace OpenGL {
	class GuiContext;

	class GuiManager {
	public:
		GuiManager(const GuiManager&) = delete;
		~GuiManager();

		inline static GuiManager& Get() {
			static GuiManager Instance;
			return Instance;
		}

		void Init(size_t size);
		void Draw();
		void Begin();
		void End();
		inline std::vector<GuiContext*>& GetContexts() { return GuiContexts;  }
	protected:
		GuiManager::GuiManager() = default;
	private:
		ImGuiContext* Context;
		std::vector<GuiContext*> GuiContexts;
		bool FirstDraw;
	};
}