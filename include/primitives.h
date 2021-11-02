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
	struct Quad {
		float vertices[12] = {
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		unsigned int indices[6] = {
			0, 1, 3,
			1, 2, 3
		};
	};
}