#include <iostream>
#include "input.h"
#include "gameObject.h"
#include "context.h"

void InputComponent::Update(GameObject& object) {}

void PlayerInputComponent::Update(GameObject& object)
{
	if (glfwGetKey(OpenGLContext::window, GLFW_KEY_W) == GLFW_PRESS)
	{
		object.direction.y = 1;
	}
	else if (glfwGetKey(OpenGLContext::window, GLFW_KEY_S) == GLFW_PRESS)
	{
		object.direction.y = -1;
	}
	else {
		object.direction.y = 0;
	}
}
