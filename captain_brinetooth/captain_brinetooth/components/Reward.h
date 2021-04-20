#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "box2d.h"
#include "../ecs/Entity.h"

class Reward : public Component
{
public:
	Reward();
	virtual ~Reward() {};
	void init() override;
	void update() override;



private:


	Entity* leftlimit;
	Entity* rightlimit;
	bool catched;
	Transform* tr_;
	BoxCollider* collider_;
	Vector2D speed;
	void baitMovement();

	static void baitCollision(b2Contact* contact);
	void baitCatched(Entity* hook); // Metodo para la colision con el gancho en el que la velocidad se pone a cero y el cebo se pone donde esta el gancho(Cambio de animacion)
	//Cuando añadamos el top of the road , haremos que se destruya el objeto cuando el gancho llegue arriba del todo 
	void giveReward();
};