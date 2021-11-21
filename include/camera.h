#pragma once
#include <glm/glm.hpp>

class OrthographicCamera {
public:
	OrthographicCamera(float left, float right, float bottom, float top);
	void SetProjection(float left, float right, float bottom, float top);
public:
	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 viewProjectionMatrix;
};