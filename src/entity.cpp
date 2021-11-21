#include <glm/gtx/string_cast.hpp>
#include "entity.h"
#include "log.h"
#include "physics.h"
#include "input.h"

/* Entity handles rotation in degrees. When radians are used
(mainly in graphics and physics components) they are converted expliticly.
I.e. Entity takes rotation in degrees and passes it to needed components as radians.
*/

Entity::Entity(InputComponent* inputComponent, PhysicsComponent* physicsComponent, OpenGL::GraphicsComponent* graphicsComponent) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(physicsComponent),
	Position(glm::vec2(0.0f)),
	Size(glm::vec2(1.0f)),
	Rotation(0.0f),
	Color(glm::vec3(1.0f, 1.0f, 0.1f)),
	BodyType(b2_staticBody),
	Density(1.0f),
	Friction(0.4f),
	Restitution(0.4f)
{
	LOGGER_TRACE("Entity constructor called");
	physics->CreateEntity(*this);
}
Entity::Entity(InputComponent* inputComponent, PhysicsComponent* physicsComponent, OpenGL::GraphicsComponent* graphicsComponent, glm::vec2 position, glm::vec2 size, float rotation) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(physicsComponent),
	Position(position),
	Size(size),
	Rotation(rotation),
	Color(glm::vec3(1.0f, 1.0f, 0.1f)),
	BodyType(b2_staticBody),
	Density(1.0f),
	Friction(0.4f),
	Restitution(0.4f)
{
	LOGGER_TRACE("Entity overloaded constructor called");
	physics->CreateEntity(*this);
}

Entity::Entity(InputComponent* inputComponent, PhysicsComponent* physicsComponent, OpenGL::GraphicsComponent* graphicsComponent, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(physicsComponent),
	Position(position),
	Size(size),
	Rotation(rotation),
	Color(color),
	BodyType(b2_staticBody),
	Density(1.0f),
	Friction(0.4f),
	Restitution(0.4f)
{
	LOGGER_TRACE("Entity overloaded constructor called");
	physics->CreateEntity(*this);
}

Entity::Entity(InputComponent* inputComponent, PhysicsComponent* physicsComponent, OpenGL::GraphicsComponent* graphicsComponent, glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution) :
	input(inputComponent),
	graphics(graphicsComponent),
	physics(physicsComponent),
	Position(position),
	Size(size),
	Rotation(rotation),
	Color(color),
	BodyType(bodytype),
	Density(density),
	Friction(friction),
	Restitution(restitution)
{
	LOGGER_TRACE("Entity overloaded constructor called");
	physics->CreateEntity(*this);
}
Entity::~Entity()
{
	LOGGER_TRACE("Entity destructor called");
	delete graphics;
	delete physics;
	graphics = nullptr;
	physics = nullptr;
}

glm::vec2 Entity::GetSize() { return Size * 2.0f; }
glm::vec2 Entity::GetPosition() { return Position; }
float Entity::GetRotationDegrees() { return Rotation; }
float Entity::GetRotationRadians() { return glm::radians(Rotation); }

void Entity::SetAttributes(glm::vec2 position, glm::vec2 size, float rotation)
{
	Position = position;
	Size = size;
	Rotation = rotation;
	physics->CreateEntity(*this);
}

void Entity::SetAttributes(glm::vec2 position, glm::vec2 size, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution)
{
	Position = position;
	Size = size;
	Rotation = rotation;
	BodyType = bodytype;
	Density = density;
	Friction = friction;
	Restitution = restitution;

	physics->CreateEntity(*this);
}

void Entity::ProcessInput()
{
	input->Update(*this);
}

void Entity::Advance()
{
	physics->Update(*this);
}

void Entity::Draw()
{
	graphics->Draw(*this);
}