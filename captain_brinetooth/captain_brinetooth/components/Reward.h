#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "box2d.h"
#include "../ecs/Entity.h"

enum class PosibleWeapons { TypeSword, TypeHammer, TypeChainsaw, TypeCrab, TypeMachineGun, TypeEel, TypeInk };
class Reward : public Component
{
public:
	Reward(int w);
	virtual ~Reward() {};
	void init() override;
	void update() override;
	void giveReward();


private:

	PosibleWeapons weapontoGive;
	Entity* leftlimit;
	Entity* rightlimit;
	Entity* hook;
	bool catched;
	Transform* tr_;
	BoxCollider* collider_;
	Vector2D speed;
	void adjustIfLimits();

	static void baitCollision(b2Contact* contact);
	void baitCatched(Entity* hook); // Metodo para la colision con el gancho en el que la velocidad se pone a cero y el cebo se pone donde esta el gancho(Cambio de animacion)
	//Cuando añadamos el top of the road , haremos que se destruya el objeto cuando el gancho llegue arriba del todo 
	
};