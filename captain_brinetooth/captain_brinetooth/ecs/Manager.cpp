// This file is part of the course TPV2@UCM - Samir Genaim

#include "Manager.h"

#include <algorithm>

Manager::Manager() {

	b2Vec2 gravity(0.0f, 9.8f);
	world_ = new b2World(gravity);
}

Manager::~Manager() {
	delete world_;

	for (auto e : entities_) {
		if(e->isSleeping())delete e;
	}
}

void Manager::refresh() {

	auto toRemove = std::remove_if( //
			entities_.begin(), //
			entities_.end(), //
			[](const Entity *e) { //
				return !e->isActive();
			});

	std::for_each(toRemove, entities_.end(), [](const Entity *e) {
		delete e;
	});

	// remove dead entities from the list of entities
	entities_.erase(toRemove, entities_.end());
}

void Manager::update() {
	auto n = entities_.size();
	for (auto i = 0u; i < n; i++)
		if(!entities_[i]->isSleeping())entities_[i]->update();
}

void Manager::render() {
	auto n = entities_.size();
	for (auto i = 0u; i < n; i++)
		if(!entities_[i]->isSleeping())entities_[i]->render();
}
