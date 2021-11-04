#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "context.h"
#include "gameObject.h"
#include "input.h"
#include "graphics.h"
#include "game.h"
#include "physics.h"
#include "camera.h"

std::vector<GameObject*> Game::gameObjects(std::vector<GameObject*>(3));

Game::Game() :
	MS_PER_UPDATE(0.0111),
	context(nullptr),
	camera(nullptr)
{
	context = new OpenGLContext("Pong Clone", 800, 600);
	context->AddShader(std::string("PaddleShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
	context->AddShader(std::string("BallShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/ballFragment.shader"));
	camera = CreateCamera(800, 600);
	context->UpdateViewProjectionMatrix(camera);
	context->UpdateUniformResolution();
}

Game::~Game() {}

void Game::Start()
{
	CreatePlayer();
	CreateTestGameObject();
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
	GameObject* t = new GameObject(
		new InputComponent(),
		new QuadGraphicsComponent()
	);
	t->size = glm::vec2(2.0, 2.0);
	t->position = glm::vec2(-5.0, 0.0);
	return t;
}

GameObject* Game::CreatePlayer()
{
	GameObject* t = new GameObject(
		new PlayerInputComponent(),
		new CircleGraphicsComponent()
	);
	return t;
}

OrthographicCamera* Game::CreateCamera(float width, float height)
{
	float aspectRatio = width / height;
	float cameraDimensions = 8.0f;
	float bottom = -cameraDimensions;
	float top = cameraDimensions;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;

	std::cout << "aspectRatio: " << aspectRatio << std::endl;
	std::cout << "bottom: " << bottom << std::endl;
	std::cout << "top: " << top << std::endl;
	std::cout << "left: " << left << std::endl;
	std::cout << "right: " << right << std::endl;

	return new OrthographicCamera(left, right, bottom, top);
}
int main()
{
	Game x = Game();
	x.Start();
	return 0;
}