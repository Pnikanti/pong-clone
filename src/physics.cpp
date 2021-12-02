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

ContactListener::ContactListener() : Contacts() 
{ 
	Contacts.reserve(5); 
	PhysicsWorld::World->SetContactListener(this);
}

ContactListener::~ContactListener() {}

void ContactListener::BeginContact(b2Contact* contact)
{
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	Contact x = { contact->GetFixtureA(), contact->GetFixtureB(), manifold };
	Contacts.emplace_back(x);
}

void ContactListener::EndContact(b2Contact* contact)
{
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	Contact x = { contact->GetFixtureA(), contact->GetFixtureB(), manifold };
	std::vector<Contact>::iterator position;
	position = std::find(Contacts.begin(), Contacts.end(), x);

	if (position != Contacts.end())
		Contacts.erase(position);
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {}
void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {}

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
		Body->DestroyFixture(Fixture);
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
		Fixture = nullptr;
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
		Body->DestroyFixture(Fixture);
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
		Fixture = nullptr;
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
		Body->DestroyFixture(Fixture);
		PhysicsWorld::World->DestroyBody(Body);
		Body = nullptr;
		Fixture = nullptr;
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

