#include <iostream>
#include <gtx/string_cast.hpp>
#include "gameObject.h"

GameObject::GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(nullptr),
	position(glm::vec2(0.0f)),
	direction(glm::vec2(0.0f)),
	size(glm::vec2(3.0f, 3.0f)),
	speed(10.0f / 100)
{}

void GameObject::Advance()
{
	std::cout << "position: " << glm::to_string(position) << std::endl;

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