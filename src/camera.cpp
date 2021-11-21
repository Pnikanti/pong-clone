#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
	projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 10.0f)), viewMatrix(glm::mat4(1.0f))
{
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
	projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 10.0f);
	viewProjectionMatrix = projectionMatrix * viewMatrix;
}

