#pragma once
#include "ecs/Component.h"
#include "box2d.h"
#include"components/Transform.h"

class ShootDetect : public Component
{
public:
	ShootDetect(Transform* tr) { enemytransform = tr; };
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