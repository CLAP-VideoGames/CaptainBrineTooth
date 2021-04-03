#pragma once
#include "../ecs/Component.h"
#include "box2d.h"

class ContactDamage : public Component
{
public:

	ContactDamage() {};
	virtual ~ContactDamage() {};
	void init() override;
	void update() override {};
	void render() override {};
	void makeDamage();
	static void callDamage(b2Contact* contact);
};

