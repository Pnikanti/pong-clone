#pragma once
#include <string>
#include <memory>
#include <glm.hpp>
#include "input.h"
#include "graphics.h"
#include "physics.h"

class GameObject {
public:
	float speed;
	std::string name;
	glm::vec2 position;
	glm::vec2 direction;
public:
	GameObject(InputComponent* inputComponent, GraphicsComponent* graphicsComponent);
	void Advance();
	void Draw();
	void ProcessInput();
private:
	InputComponent* input;
	PhysicsComponent* physics;
	GraphicsComponent* graphics;
};