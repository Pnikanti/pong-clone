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
	FixtureDefinition() {}

PhysicsComponent::~PhysicsComponent()
{
	LOGGER_TRACE("PhysicsComponent destructor called");
	if (Body == nullptr)
		return;

	Body->DestroyFixture(Fixture);
	PhysicsWorld::World->DestroyBody(Body);
	Body = nullptr;
	Fixture = nullptr;
}

void PhysicsComponent::CreateFromEntity(Entity& entity)
{
	LOGGER_TRACE("CreateFromEntity called");
	if (Body != nullptr)
	{
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
	}

	b2PolygonShape Shape;
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


void PhysicsCircleComponent::Update(Entity& entity)
{
	b2Vec2 position = Body->GetPosition();
	entity.Position = glm::vec2(position.x, position.y);
	entity.Rotation = glm::degrees(Body->GetAngle());
}

void PhysicsPolygonComponent::Update(Entity& entity)
{
	b2Vec2 position = Body->GetPosition();
	entity.Position = glm::vec2(position.x, position.y);
	entity.Rotation = glm::degrees(Body->GetAngle());
}

void PhysicsCircleComponent::CreateFromEntity(Entity& entity)
{
	LOGGER_TRACE("Circle CreateFromEntity called");
	if (Body != nullptr)
	{
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
	}

	Shape.m_radius = (entity.Size.x + entity.Size.y) / 4;

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

void PhysicsPolygonComponent::CreateFromEntity(Entity& entity)
{
	LOGGER_TRACE("Polygon CreateFromEntity called");
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

