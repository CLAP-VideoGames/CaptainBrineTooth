#pragma once
#include "../ecs/Component.h"
#include "box2d.h"
#include"Transform.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "Enemy_Health.h"

class ShootDetect : public Component
{
public:
	ShootDetect(Transform* tr, Entity* e) : enemytransform(tr), enemy(e) {};
	virtual ~ShootDetect() {};
	void init() override;
	void update() override;
	void render() override {};
	static void shoot(b2Contact* contact);
	static void unShoot(b2Contact* contact);
	void setShootFalse();
	void shootatTime();
	void createBullet();

protected:
	Entity* enemy;
	float minTime;
	float passedTime;
	bool shootenabled;
	float offsetbala;
	float velocity;
	Transform* enemytransform;

};

// if(tiempo pasado + tiempo minimo < tiempo actual)
	// Generar la bala
	// tiempo pasado = tiempo actual
	//