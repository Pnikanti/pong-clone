#pragma once
#include <glm.hpp>
#include "primitives.h"
class GameObject;

class GraphicsComponent {
public:
	virtual void Draw(GameObject& object);
};

class TriangleGraphicsComponent : public GraphicsComponent {
public:
	TriangleGraphicsComponent();
	void Draw(GameObject& object) override;
private:
	Primitives::Triangle triangle;
	unsigned int vao;
	unsigned int vbo;
};

class QuadGraphicsComponent : public GraphicsComponent {
public:
	QuadGraphicsComponent();
	void Draw(GameObject& object) override;
private:
	Primitives::Quad quad;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};

class CircleGraphicsComponent : public GraphicsComponent {
public:
	CircleGraphicsComponent();
	void Draw(GameObject& object) override;
private:
	Primitives::Quad quad;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
};