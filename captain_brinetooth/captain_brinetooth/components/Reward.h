#pragma once
#include "Transform.h"
#include "../ecs/Component.h"
#include "box2d.h"

class Reward :Component
{
public:
	Reward();
	virtual ~Reward() {};
	void init() override;
	void update() override;



private:

	Transform* tr_;
	Vector2D speed;
	void checkLimits();
	static void baitCollision(b2Contact* contact);
	void baitCatched(Entity* hook); // Metodo para la colision con el gancho en el que la velocidad se pone a cero y el cebo se pone donde esta el gancho(Cambio de animacion)
	//Cuando añadamos el top of the road , haremos que se destruya el objeto cuando el gancho llegue arriba del todo 
};