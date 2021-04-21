#pragma once
#include "../ecs/Component.h"
#include "../utils/Vector2D.h"
#include "./BoxCollider.h"
#include "../ecs/Entity.h"


class Gancho : public Component
{
public :
	Gancho();
	virtual ~Gancho() {};
	void init() override;
	void update() override;
	bool hasBaitRef();
	void setBaitReference(Entity* b) { baitRef = b; }
	int getContactId() { return contactid; } //In order to know wether  has contact with the top of the rod or with the down floor

private:

	int contactid;  // 1 DownMovement //2 UpMovement  //3 Top of the Rod
	bool move;
	float moveTime;         //CurrRealTime
	float timetoMove = 300;       //Definded time 
	BoxCollider* hookBody;
	Vector2D speed;      //Movement in Y 
	Entity* baitRef;
	//Collision check with top and floor
	static void contactWithSomething(b2Contact* contact);
	void hookMovement();   //Check contacts at time
	void collisionAnswer(Entity* contactedfloor); //In order to find ait to obtain points /weapons...etc
	void setSpeed(int amount) { speed* amount; }
};

