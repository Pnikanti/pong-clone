#include <iostream>
#include "gameObject.h"
#include "game.h"

GameObject::GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent) 
	: input(inputComponent), graphics(graphicsComponent), position(glm::vec2(0, 0)), direction(glm::vec2(0, 0)), speed(0.05f)
{
	Game::gameObjects.push_back(this);
}


void GameObject::Advance()
{
	position = position + (glm::vec2(speed) * direction);
	std::cout << position.x << ", " << position.y << std::endl;
}

void GameObject::Draw()
{
	graphics->Update(*this);
}

void GameObject::ProcessInput()
{
	input->Update(*this);
}