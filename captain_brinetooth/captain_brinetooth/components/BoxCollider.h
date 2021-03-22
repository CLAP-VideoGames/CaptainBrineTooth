#pragma once
#include <iostream>


#include "box2d.h"
#include <cmath>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"


const double PIXELS_IN_METERS = 200;

class BoxCollider : public Component {
public:
	BoxCollider(float rotation = 0.0f, bool dynamic = false)
	{
		rotation_ = rotation;
		isDynamic = dynamic;
	}

	virtual ~BoxCollider() {
		
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		pos = tr_->getPos();

		//1 pixel(X) 0.0002645833 m
		//then n pixels = n * 0.0002645833;
		size = Vector2D(tr_->getW() / PIXELS_IN_METERS, tr_->getH() / PIXELS_IN_METERS);
		world = entity_->getWorld();

		b2BodyDef bodyDef;
		if (isDynamic) {
			bodyDef.type =b2_dynamicBody;
		}
		else {
			bodyDef.type = b2_staticBody;
		}
		
		bodyDef.position.Set(pos.getX() /PIXELS_IN_METERS, pos.getY() / PIXELS_IN_METERS);
		//bodyDef.angle = rotation_;
		//Make the body
		body = world->CreateBody(&bodyDef);


		b2PolygonShape boxShape;
		boxShape.SetAsBox(size.getX() / 2.0f, size.getY() / 2.0f);
		b2FixtureDef fixtureDef;
		//Que el cubo real tenga la misma forma que la definicion
		fixtureDef.shape = &boxShape;
		//No puede ser la densidad 0 para un objeto dinamico
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.7f;
		
		fixture = body->CreateFixture(&fixtureDef);
		
	}

	void setSpeed(Vector2D speed)
	{
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = speed.getX(); vel.y = speed.getY();
		body->SetLinearVelocity(vel);
	
	}

	void update() override {

		//if(isDynamic)
			//std::cout << body->GetPosition().x << " " << body->GetPosition().y << " " << body->GetAngle() << " " << tr_->getRot() << std::endl;

		tr_->getPos().set(round((body->GetPosition().x * PIXELS_IN_METERS ) - tr_->getW()/2.0f), round((body->GetPosition().y * PIXELS_IN_METERS)- tr_->getH() / 2.0f));
		tr_->setRot((body->GetAngle() * (180.0f))/ M_PI);
		
		/*�Custom method to detect collisions and delete a body
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
	float rotation_;
	//bool entra = 0;

	b2World* world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

