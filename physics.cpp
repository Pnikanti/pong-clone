#include <glm/gtx/string_cast.hpp>
#include "physics.h"
#include "entity.h"
#include "game.h"
#include "log.h"

b2World* PhysicsWorld::World = nullptr;

PhysicsWorld::PhysicsWorld() :
	VelocityIterations(6), 
	PositionIterations(2)
{
	World = new b2World({ 0.0f, 0.0f });
}

PhysicsWorld::~PhysicsWorld()
{
	delete World;
	World = nullptr;
}

void PhysicsWorld::Update()
{
	auto bodies = World->GetBodyCount();
	LOGGER_TRACE("Updating \"{0}\" bodies!", bodies);
	World->Step(Game::TimeStep, VelocityIterations, PositionIterations);
}

PhysicsComponent::PhysicsComponent() :
	Body(nullptr),
	Fixture(nullptr),
	BodyDefinition(),
	Shape(),
	FixtureDefinition() 
{
	LOGGER_TRACE("PhysicsComponent constructor called");
}

void PhysicsComponent::CreateEntity(Entity& entity)
{
	LOGGER_TRACE("CreateEntity called");
	if (Body != nullptr)
	{
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
	}

	Shape.SetAsBox(entity.Size.x, entity.Size.y);

	FixtureDefinition.shape = &Shape;
	FixtureDefinition.density = entity.Density;
	FixtureDefinition.friction = entity.Friction;
	FixtureDefinition.restitution = entity.Restitution;

	BodyDefinition.type = entity.BodyType;
	BodyDefinition.position.Set(entity.Position.x, entity.Position.y);

	Body = PhysicsWorld::World->CreateBody(&BodyDefinition);
	Body->SetTransform(Body->GetPosition(), glm::radians(entity.Rotation));
	Fixture = Body->CreateFixture(&FixtureDefinition);
}

void PhysicsComponent::Update(Entity& entity) {}

PhysicsStaticComponent::PhysicsStaticComponent()
{
	LOGGER_TRACE("PhysicsStaticComponent constructor called");
}

PhysicsStaticComponent::~PhysicsStaticComponent()
{
	LOGGER_TRACE("PhysicsStaticComponent destructor called");
	if (Body == nullptr)
		return;

	Body->DestroyFixture(Fixture);
	PhysicsWorld::World->DestroyBody(Body);
	Body = nullptr;
	Fixture = nullptr;
}

PhysicsDynamicComponent::PhysicsDynamicComponent()
{
	LOGGER_TRACE("PhysicsDynamicComponent constructor called");
}

PhysicsDynamicComponent::~PhysicsDynamicComponent()
{
	LOGGER_TRACE("PhysicsDynamicComponent destructor called");
	if (Body == nullptr)
		return;

	Body->DestroyFixture(Fixture);
	PhysicsWorld::World->DestroyBody(Body);
	Body = nullptr;
	Fixture = nullptr;
}

void PhysicsDynamicComponent::Update(Entity& entity)
{
	b2Vec2 position = Body->GetPosition();
	entity.Position = glm::vec2(position.x, position.y);
	entity.Rotation = glm::degrees(Body->GetAngle());
}