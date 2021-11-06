#include <iostream>
#include "input.h"
#include "gameObject.h"
#include "context.h"
#include "game.h"

void InputComponent::Update(GameObject& object) {}
void InputComponent::Update(Game& game) {}
void InputComponent::Update() {}

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

void MasterInputComponent::Update(Game& game)
{
	switch (game.state)
	{
		case GameState::Play:
		{
			if (glfwGetKey(OpenGLContext::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				game.state = GameState::MainMenu;
			}
			break;
		}
		case GameState::MainMenu:
		{
			if (glfwGetKey(OpenGLContext::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose(OpenGLContext::window, true);
			}
			else if (glfwGetKey(OpenGLContext::window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				game.state = GameState::Play;
				game.Start();
			}
			break;
		}
		case GameState::GameOver:
		{
			if (glfwGetKey(OpenGLContext::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				game.state = GameState::MainMenu;
				game.End();
			}
			break;
		}
	}
}