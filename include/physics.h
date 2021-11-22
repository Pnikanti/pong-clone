#pragma once
#include <glm/glm.hpp>
#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
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
	virtual ~PhysicsComponent();
	virtual void CreateFromEntity(Entity& entity);
	virtual void Update(Entity& entity);
public:
	b2Body* Body;
	b2Fixture* Fixture;
	b2BodyDef BodyDefinition;
	b2FixtureDef FixtureDefinition;
};

class PhysicsPolygonComponent : public PhysicsComponent {
public:
	PhysicsPolygonComponent() = default;
	~PhysicsPolygonComponent() = default;
	void Update(Entity& entity) override;
	void CreateFromEntity(Entity& entity) override;
public:
	b2PolygonShape Shape;
};


class PhysicsCircleComponent : public PhysicsComponent {
public:
	PhysicsCircleComponent() = default;
	~PhysicsCircleComponent() = default;
	void Update(Entity& entity) override;
	void CreateFromEntity(Entity& entity) override;
public:
	b2CircleShape Shape;
};