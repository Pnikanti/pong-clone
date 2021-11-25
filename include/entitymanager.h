#pragma once

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <memory>
#include <vector>
#include <unordered_map>
#include <string>
#include "entity.h"
#include "game.h"

class PhysicsComponent;
namespace OpenGL { class GraphicsComponent; }

class EntityManager {
public:
	EntityManager(const EntityManager&) = delete; // copy-constructor

	Entity* CreateEntity(InputComponent* input, PhysicsComponent* physics, OpenGL::GraphicsComponent* graphics, glm::vec2 size, glm::vec2 position, float rotation, glm::vec3 color, b2BodyType bodytype, float density, float friction, float restitution);
	Entity* CreatePaddle(Side side, Input input);
	Entity* CreateBall();

	static EntityManager& Get() {
		static EntityManager Instance;
		return Instance;
	}
	static void Init(size_t size) { 
		EntityManager::GetEntities().reserve(size); 
		EntityManager::GetEntityMap().reserve(size);
	}
	static std::vector<Entity*>& GetEntities() {
		static std::vector<Entity*> Entities;
		return Entities;
	}
	static std::unordered_map<std::string, Entity*>& GetEntityMap() { 
		static std::unordered_map<std::string, Entity*> EntityMap;
		return EntityMap; 
	}

protected:
	EntityManager::EntityManager() = default; // constructor
};