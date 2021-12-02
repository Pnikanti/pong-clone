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
		glm::vec2 paddlePosition = entity.GetPosition();
		bool upperBounds = paddlePosition.y < 17.5f;

		if (upperBounds)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y + 0.4f), entity.GetRotationRadians());
	}
	else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		auto body = entity.GetPhysicsComponent()->Body;
		glm::vec2 paddlePosition = entity.GetPosition();
		bool lowerBounds = paddlePosition.y > -17.5;

		if (lowerBounds)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y - 0.4f), entity.GetRotationRadians());
	}
}

void ComputerInputComponent::Update(Entity& entity)
{
	CurrentTime = (float)glfwGetTime();
	// Downgrade AI movements per second
	if (CurrentTime > PreviousTime + 0.02f)
	{
		PreviousTime = CurrentTime;
		auto body = entity.GetPhysicsComponent()->Body;
		auto it = EntityManager::Get().GetEntityMap().find("Ball");

		if (it == EntityManager::Get().GetEntityMap().end())
			return;

		Entity* ball = it->second;
		glm::vec2 ballPosition = ball->GetPosition();
		glm::vec2 paddlePosition = entity.GetPosition();

		// Downgrade AI accuracy
		if (ballPosition.y - 0.4f <= paddlePosition.y && paddlePosition.y <= ballPosition.y + 0.4f )
			return;

		bool upperBounds = paddlePosition.y < 17.5f;
		bool lowerBounds = paddlePosition.y > -17.5;

		// Complicated AI logic here
		if (upperBounds && ballPosition.y > paddlePosition.y)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y + 0.4f), entity.GetRotationRadians());
		else if (lowerBounds && ballPosition.y < paddlePosition.y)
			body->SetTransform(b2Vec2(paddlePosition.x, paddlePosition.y - 0.4f), entity.GetRotationRadians());
	}
}

void GameInputComponent::Update(Game& gameInstance)
{
	CurrentTime = (float)glfwGetTime();
	if (CurrentTime > PreviousTime + 0.3f)
	{
		switch (Game::State)
		{
		case GameState::Play:
		{
			if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				LOGGER_INFO("Main menu");
				gameInstance.State = GameState::MainMenu;
				gameInstance.End();
				gameInstance.ResetScore();
				PreviousTime = CurrentTime;
			}
			else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_P) == GLFW_PRESS)
			{
				LOGGER_INFO("Pause");
				gameInstance.State = GameState::Pause;
				PreviousTime = CurrentTime;
			}
			break;
		}
		case GameState::MainMenu:
		{
			if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				LOGGER_INFO("Exit");
				gameInstance.Exit();
				PreviousTime = CurrentTime;
			}
			else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				LOGGER_INFO("Start");
				gameInstance.State = GameState::Start;
				gameInstance.Start();
				PreviousTime = CurrentTime;
			}
			break;
		}
		case GameState::GameOver:
		{
			if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				LOGGER_INFO("Start");
				gameInstance.State = GameState::Start;
				gameInstance.End();
				gameInstance.Start();
				PreviousTime = CurrentTime;
			}
			else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				LOGGER_INFO("Main menu");
				gameInstance.State = GameState::MainMenu;
				gameInstance.End();
				gameInstance.ResetScore();
				PreviousTime = CurrentTime;
			}
			break;
		}
		case GameState::Pause:
		{
			if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_SPACE) == GLFW_PRESS)
			{
				LOGGER_INFO("Play");
				gameInstance.State = GameState::Play;
				PreviousTime = CurrentTime;
			}
			else if (glfwGetKey(OpenGL::Context::Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				LOGGER_INFO("Main menu");
				gameInstance.State = GameState::MainMenu;
				gameInstance.End();
				gameInstance.ResetScore();
				PreviousTime = CurrentTime;
			}
			break;
		}
		}
	}
}