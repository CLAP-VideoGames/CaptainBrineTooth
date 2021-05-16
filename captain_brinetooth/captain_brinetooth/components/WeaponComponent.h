#pragma once

#include "../ecs/Component.h"
#include "../components/Player_Health.h"

class WeaponComponent : public Component {
public:
	bool getIsAttacking() { return isAttacking; }
protected:
	bool isAttacking = false;
};