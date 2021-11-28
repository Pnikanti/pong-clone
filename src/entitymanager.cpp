#include "entitymanager.h"
#include "physics.h"
#include "graphics.h"
#include "context.h"
#include "input.h"
#include "log.h"

void EntityManager::Init(size_t size)
{
	Entities.reserve(size);
	EntityMap.reserve(size);
}

void EntityManager::Draw()
{
	for (auto i : Entities)
	{
		if (i != nullptr)
			i->Draw();
	}
}