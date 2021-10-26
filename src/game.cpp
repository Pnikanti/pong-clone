#include <glfw3.h>
#include <iostream>
#include "context.h"
#include "gameObject.h"
#include "input.h"
#include "graphics.h"
#include "game.h"
#include "physics.h"

std::vector<GameObject*> Game::gameObjects(std::vector<GameObject*>(3));

// 0.0111 = 90 / 1
Game::Game() 
	: context(nullptr), MS_PER_UPDATE(0.0111), test(nullptr)
{
	context = new OpenGLContext("Pong Clone");
	context->UseShader(std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
}
Game::~Game() {}

void Game::Start()
{
	test = CreateTestGameObject();
	GameLoop();
}

void Game::GameLoop()
{
	double previous = glfwGetTime();
	double current = glfwGetTime();
	double elapsed = 0.0;
	double lag = 0.0;

	while (context->alive)
	{
		current = glfwGetTime();
		elapsed = current - previous;
		previous = current;
		lag += elapsed;

		ProcessInput();
		while (lag >= MS_PER_UPDATE)
		{
			Advance();
			lag -= MS_PER_UPDATE;
		}
		context->RenderOneFrame();
	}
}

void Game::Advance()
{
	for (auto i : Game::gameObjects)
	{
		if (i != nullptr) {
			i->Advance();
		}
	}
}

void Game::ProcessInput()
{
	for (auto i : Game::gameObjects)
	{
		if (i != nullptr) {
			i->ProcessInput();
		}
	}
}

GameObject* Game::CreateTestGameObject()
{
	return new GameObject(
		new PlayerInputComponent(),
		new TriangleGraphicsComponent()
	);
}

int main()
{
	Game x = Game();
	x.Start();
	return 0;
}