#pragma once
#include "glm.hpp"

namespace Primitives {
	class Triangle {
	private:
		float vertices[6];
		unsigned int vao;
		unsigned int vbo;
	public:
		Triangle(float verts[6]);
		Triangle();
		void Draw(glm::vec2 position);
	};
}