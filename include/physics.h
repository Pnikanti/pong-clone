#pragma once
#include <glm/glm.hpp>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

class Entity;

class PhysicsWorld {
public:
	PhysicsWorld();
	~PhysicsWorld();
	void Update();
public:
	static b2World* World;
private:
	int32 VelocityIterations;
	int32 PositionIterations;
};

class PhysicsComponent {
public:
	PhysicsComponent();
	virtual ~PhysicsComponent() = default;
	void CreateEntity(Entity& entity);
	virtual void Update(Entity& entity);
public:
	b2Body* Body;
	b2Fixture* Fixture;
	b2BodyDef BodyDefinition;
	b2PolygonShape Shape;
	b2FixtureDef FixtureDefinition;
};

class PhysicsStaticComponent : public PhysicsComponent {
public:
	PhysicsStaticComponent();
	~PhysicsStaticComponent();
};

class PhysicsDynamicComponent : public PhysicsComponent {
public:
	PhysicsDynamicComponent();
	~PhysicsDynamicComponent();
	void Update(Entity& entity) override;
};