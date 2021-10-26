#pragma once
#include <string>
#include <memory>
#include <glm.hpp>
#include "input.h"
#include "graphics.h"
#include "physics.h"

class GameObject {
public:
	std::string name;
	glm::vec2 position;
	glm::vec2 direction;
	float speed;
public:
	GameObject::GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent);
	void Advance();
	void Draw();
	void ProcessInput();
private:
	InputComponent* input;
	PhysicsComponent* physics;
	GraphicsComponent* graphics;
};