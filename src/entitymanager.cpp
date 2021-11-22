#include "entitymanager.h"
#include "physics.h"
#include "graphics.h"
#include "context.h"
#include "input.h"

std::vector<Entity*> EntityManager::Entities;

Entity* EntityManager::CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution)
{
	Entity* x = new Entity(input, physics, graphics, position, size, rotation, color, bodytype, density, friction, restitution);
	Entities.emplace_back(x);
	return x;
}

Entity* EntityManager::CreatePaddle(Side side, Input input)
{
	glm::vec2 position = glm::vec2(0.0f);
	InputComponent* inputComponent = nullptr;

	switch (side)
	{
	case Side::Left:
		position = glm::vec2(-25.0f, 0.0f);
		break;
	case Side::Right:
		position = glm::vec2(25.0f, 0.0f);
		break;
	}
	switch (input)
	{
	case Input::Human:
		inputComponent = new PlayerInputComponent();
		break;
	case Input::Ai:
		inputComponent = new ComputerInputComponent();
		break;
	}
	Entity* x = CreateEntity(
		inputComponent,
		new PhysicsPolygonComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(0.5f, 3.0f),
		position,
		0.0f,
		glm::vec3(255.0f, 255.0f, 0.0f),
		b2_kinematicBody,
		1.0f,
		1.0f,
		0.8f
	);
	return x;
}

Entity* EntityManager::CreateBall()
{
	Entity* x = CreateEntity(
		new InputComponent(),
		new PhysicsCircleComponent(),
		new OpenGL::QuadComponent(),
		glm::vec2(0.5f),
		glm::vec2(0.0f, 4.0f),
		30.0f,
		glm::vec3(255.0f, 255.0f, 255.0f),
		b2_dynamicBody,
		1.0f,
		1.0f,
		0.8f
	);
	return x;
}