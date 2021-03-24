// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <initializer_list>
#include <vector>
#include <list>

#include "ecs.h"
#include "Entity.h"
#include "box2d.h"

class Manager {
public:

	Manager();
	virtual ~Manager();

	// entities
	Entity* addEntity(bool sleeping) {
		Entity *e = new Entity(this, world_,sleeping);
		if (e != nullptr) {
			e->resetGroups();
			entities_.emplace_back(e);
		}
		return e;
	}

	// handlers
	template<typename T>
	inline void setHandler(Entity *e) {
		hdlrs_[ecs::hdlrIdx<T>] = e;
	}

	template<typename T>
	inline Entity* getHandler() {
		return hdlrs_[ecs::hdlrIdx<T>];
	}

	inline const std::vector<Entity*>& getEnteties() {
		return entities_;
	}

	inline b2World* getWorld() {
		return world_;
	}

	void update();
	void render();
	void refresh();

private:


	b2World* world_;
	std::vector<Entity*> entities_;
	std::array<Entity*, ecs::maxHdlr> hdlrs_;

};

