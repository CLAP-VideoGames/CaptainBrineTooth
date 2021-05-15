#pragma once

#include "../ecs/Component.h"

class WeaponComponent : public Component {
public:
	bool getIsAttacking() { return isAttacking; }
protected:
	bool isAttacking = false;
};