#include <iostream>
#include <gtx/string_cast.hpp>
#include "gameObject.h"
#include "game.h"

GameObject::GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(nullptr),
	position(glm::vec2(0.0f)),
	direction(glm::vec2(0.0f)),
	size(glm::vec2(0.5f, 2.0f)),
	speed(5.0f / 100)
{
	Game::gameObjects.push_back(this);
}

void GameObject::Advance()
{
	std::cout << "direction: " << glm::to_string(direction) << std::endl;

	position = position + (glm::vec2(speed) * direction);
}

void GameObject::Draw()
{
	graphics->Draw(*this);
}

void GameObject::ProcessInput()
{
	input->Update(*this);
}