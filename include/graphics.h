#pragma once
#include <glm.hpp>
#include "primitives.h"
class GameObject;

class GraphicsComponent {
public:
	glm::vec2 size;
	glm::mat4 model;
public:
	GraphicsComponent();
	virtual void Draw(GameObject& object);
};

class TriangleGraphicsComponent : public GraphicsComponent {
public:
	glm::vec2 size;
	glm::mat4 model;
public:
	TriangleGraphicsComponent();
	void Draw(GameObject& object) override;
private:
	Primitives::Triangle triangle;
	unsigned int vao;
	unsigned int vbo;
};