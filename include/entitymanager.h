#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <box2d/b2_body.h>
#include "entity.h"
#include "game.h"

class PhysicsComponent;
namespace OpenGL { class GraphicsComponent; }

class EntityManager {
public:
	EntityManager(const EntityManager&) = delete; // copy-constructor
	void CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics);
	void CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation);
	void CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color);
	void CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution);
	void CreatePaddle(Side side, Input input);
	void CreateBall();
	static EntityManager& Get() { 
		static EntityManager Instance; 
		return Instance; 
	}
	static void Init(size_t size) { EntityManager::Get().Entities.reserve(size); }
	static std::vector<Entity*>& GetEntities() { return EntityManager::Get().Entities; }
protected:
	EntityManager::EntityManager() = default; // constructor
private:
	static std::vector<Entity*> Entities;
};