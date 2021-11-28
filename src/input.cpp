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
	CurrentMovement = (float)glfwGetTime();
	// Downgrade AI movements per second
	if (CurrentMovement > PreviousMovement + 0.03f)
	{
		PreviousMovement = CurrentMovement;
		auto body = entity.GetPhysicsComponent()->Body;
		auto it = EntityManager::Get().GetEntityMap().find("Ball");

		if (it == EntityManager::Get().GetEntityMap().end())
			return;

		Entity* ball = it->second;
		glm::vec2 ballPosition = ball->GetPosition();
		glm::vec2 paddlePosition = entity.GetPosition();
		// Downgrade AI accuracy
		if (ballPosition.y - 0.5f <= paddlePosition.y && paddlePosition.y <= ballPosition.y + 0.5f )
			return;
		// Complicated AI logic here
		if (ballPosition.y > paddlePosition.y)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y + 0.4f), entity.GetRotationRadians());
		else if (ballPosition.y < paddlePosition.y)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y - 0.4f), entity.GetRotationRadians());
	}
}

void GameInputComponent::Update(Game& gameInstance)
{
	switch (Game::State)
	{
	case GameState::Play:
	{
		if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			Game::State = GameState::MainMenu;
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
			Game::State = GameState::Play;
			LOGGER_INFO("Play");
			gameInstance.Start();
		}
		break;
	}
	case GameState::GameOver:
	{
		if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			Game::State = GameState::Play;
			LOGGER_INFO("Play");
			gameInstance.Start();
		}
		else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(OpenGL::Context::Window, true);
		}
		break;
	}
	}
}