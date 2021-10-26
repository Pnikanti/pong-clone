#pragma once
#include "primitives.h"
class GameObject;

class GraphicsComponent {
public:
	virtual void Update(GameObject& object);
};

class TriangleGraphicsComponent : public GraphicsComponent {
public:
	TriangleGraphicsComponent();
	void Update(GameObject& object) override;
private:
	Primitives::Triangle triangle;
};