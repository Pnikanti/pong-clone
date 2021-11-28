#pragma once

#include <glm/glm.hpp>
#include <box2d/b2_body.h>
#include <memory>
#include <vector>
#include <robin_hood/robin_hood.h>
#include <string>
#include "entity.h"
#include "game.h"

class PhysicsComponent;
namespace OpenGL { class GraphicsComponent; }

class EntityManager {
public:
	EntityManager(const EntityManager&) = delete; // copy-constructor

	static EntityManager& Get() {
		static EntityManager Instance;
		return Instance;
	}
	void Init(size_t size);
	void Draw();
	std::vector<Entity*>& GetEntities();
	robin_hood::unordered_map<std::string, Entity*>& GetEntityMap();
protected:
	EntityManager::EntityManager() = default; // constructor
private:
	std::vector<Entity*> Entities;
	robin_hood::unordered_map<std::string, Entity*> EntityMap;
};