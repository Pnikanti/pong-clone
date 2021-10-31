#pragma once
#include <array>
#include <glm.hpp>

namespace Primitives {
	struct Triangle {
		float vertices[9] = {
		   -0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};
	};
}