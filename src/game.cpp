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
	MS_PER_UPDATE(0.02),
	state(GameState::MainMenu),
	context(nullptr),
	camera(nullptr)
{
	context = new OpenGLContext("Pong Clone", 800, 600);
	context->AddShader(std::string("PaddleShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/fragment.shader"));
	context->AddShader(std::string("BallShader"), std::string("res/shaders/vertex.shader"), std::string("res/shaders/ballFragment.shader"));
	camera = CreateCamera(800, 600);
	context->UpdateViewProjectionMatrix(camera);
	context->UpdateUniformResolution();
	input = new MasterInputComponent();
	GameLoop();
}

Game::~Game() {}

void Game::Start()
{
	CreatePaddle();
	CreateBall();
}

void Game::End()
{
	for (auto i : gameObjects)
	{
		if (i != nullptr) {
			delete i;
		}
	}
	gameObjects.clear();
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
			if (state == GameState::Play)
				Advance();

			lag -= MS_PER_UPDATE;
		}
		context->RenderOneFrame();
	}
}

void Game::Advance()
{
	for (auto i : gameObjects)
	{
		if (i != nullptr) {
			i->Advance();
		}
	}
}

void Game::ProcessInput()
{
	input->Update(*this);

	for (auto i : gameObjects)
	{
		if (i != nullptr) {
			i->ProcessInput();
		}
	}
}

void Game::CreateBall()
{
	GameObject* t = new GameObject(
		new InputComponent(),
		new CircleGraphicsComponent()
	);
	t->size = glm::vec2(1.0, 1.0);
	gameObjects.push_back(t);
}

void Game::CreatePaddle()
{
	GameObject* t = new GameObject(
		new PlayerInputComponent(),
		new QuadGraphicsComponent()
	);
	t->size = glm::vec2(0.5, 2.5);
	t->position = glm::vec2(-8.0, 0.0);
	t->speed = 0.3f;
	gameObjects.push_back(t);
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
	return 0;
}