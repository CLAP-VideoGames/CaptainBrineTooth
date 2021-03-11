#pragma once
#include <iostream>


#include "box2d.h"

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"

class BoxCollider : public Component {
public:
	BoxCollider(bool dynamic = false)
	{
		isDynamic = dynamic;
	}

	virtual ~BoxCollider() {

	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		pos = tr_->getPos();
		size = Vector2D(tr_->getW(), tr_->getH());
		world = entity_->getWorld();
		assert(tr_ != nullptr);

		b2BodyDef bodyDef;
		if (isDynamic) {
			bodyDef.type = b2_dynamicBody;
		}
		else {
			bodyDef.type = b2_staticBody;
		}
		
		bodyDef.position.Set(pos.getX(), pos.getY());
		//Make the body
		body = world->CreateBody(&bodyDef);


		b2PolygonShape boxShape;
		boxShape.SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);
		b2FixtureDef fixtureDef;
		//Que el cubo real tenga la misma forma que la definicion
		fixtureDef.shape = &boxShape;
		//No puede ser la densidad 0 para un objeto dinamico
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 3.0f;

		fixture = body->CreateFixture(&fixtureDef);
	}

	void update() override {
		//std::cout << body->GetPosition().x << " " << body->GetPosition().y << " " << body->GetAngle() << std::endl;

		tr_->getPos().set(body->GetPosition().x, body->GetPosition().y);
		tr_->setRot(body->GetAngle());

		/*´Custom method to detect collisions and delete a body
		b2ContactEdge* b;
		
		b = body->GetContactList();

		if (b != nullptr && entra == false) {
			std::cout << "Collided";
			world->DestroyBody(b->contact->GetFixtureA()->GetBody());
			entra = 1;
		}*/

	}

	inline b2Body* getBody() const
	{
		return body;
	}

	inline b2Fixture* getFixture() const
	{
		return fixture;
	}

private:
	Transform* tr_;
	Vector2D pos, size;
	bool isDynamic;
	//bool entra = 0;

	b2World* world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

