#include <glfw/glfw3.h>
#include "input.h"
#include "context.h"
#include "entity.h"
#include "game.h"
#include "log.h"
#include "physics.h"
#include "entitymanager.h"

void InputComponent::Update(Entity& entity)
{

}

void InputComponent::Update(Game& application)
{

}

void InputComponent::Update()
{

}

void PlayerInputComponent::Update(Entity& entity)
{
	if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		auto body = entity.GetPhysicsComponent()->Body;
		if (entity.GetPosition().y < 17.5f)
			body->SetTransform(b2Vec2(entity.GetPosition().x, entity.GetPosition().y + 0.4f), entity.GetRotationRadians());
	}
	else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		auto body = entity.GetPhysicsComponent()->Body;
		if (entity.GetPosition().y > -17.5f)
			body->SetTransform(b2Vec2(entity.GetPosition().x, entity.GetPosition().y - 0.4f), entity.GetRotationRadians());
	}
}

void ComputerInputComponent::Update(Entity& entity)
{
	auto body = entity.GetPhysicsComponent()->Body;
}

void GameInputComponent::Update(Game& game)
{
	switch (game.State)
	{
	case GameState::Play:
	{
		if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			game.State = GameState::MainMenu;
			LOGGER_INFO("Main menu");
		}
		break;
	}
	case GameState::MainMenu:
	{
		if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(OpenGL::Context::Window, true);
		}
		else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			game.State = GameState::Play;
			LOGGER_INFO("Play");
			game.Start();
		}
		break;
	}
	case GameState::GameOver:
	{
		if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			game.State = GameState::Play;
			LOGGER_INFO("Play");
			game.Start();
		}
		else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(OpenGL::Context::Window, true);
		}
		break;
	}
	}
}