#pragma once
#include <iostream>


#include "box2d.h"
#include <cmath>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"


//1 pixel(X) 0.0002645833 m
//then n pixels = n * 0.0002645833;

enum TYPE { STATIC, DYNAMIC, KINEMATIC };

class BoxCollider : public Component {
public:
	BoxCollider(int typeAux = TYPE::STATIC, int colLay = 1 ,bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true,  float rotation = 0.0f)
	{
		type = typeAux;
		isTrigger = isTriggerAux;
		friction_ = friction;
		tr_ = nullptr;
		colLay_ = colLay;
		fixedRotation_ = fixedRotation;
		rotation_ = rotation;
	}

	virtual ~BoxCollider() {
		
	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		pos = tr_->getPos();

		size = Vector2D(tr_->getW() / sdlutils().getPPM() , tr_->getH() / sdlutils().getPPM());
		world = entity_->getWorld();

		b2BodyDef bodyDef;
		switch (type) {
		case TYPE::STATIC:
			bodyDef.type = b2_staticBody;
			break;

		case TYPE::DYNAMIC:
			bodyDef.type = b2_dynamicBody;
			break;

		case TYPE::KINEMATIC:
			bodyDef.type = b2_kinematicBody;
			break;

		default:
			bodyDef.type = b2_staticBody;
			break;
		}
		
		bodyDef.position.Set(pos.getX() / sdlutils().getPPM(), pos.getY() / sdlutils().getPPM());
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
		fixtureDef.friction = friction_;
		fixtureDef.isSensor = isTrigger;

		body->SetFixedRotation(fixedRotation_);
		
		setColLays(fixtureDef);

		fixture = body->CreateFixture(&fixtureDef);
		
	}

	void setSpeed(Vector2D speed)
	{
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = speed.getX(); vel.y = speed.getY();
		body->SetLinearVelocity(vel);
	
	}

	/// <summary>
	/// Aplica una fuerza gradual en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyForce(Vector2D dir, float force = 1.0f){
		body->ApplyForce(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
	}

	/// <summary>
	/// Aplica una fuerza inmediata en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyLinearForce(Vector2D dir, float force = 1.0f){
		body->ApplyLinearImpulse(b2Vec2(dir.getX()* force, dir.getY()* force) , body->GetWorldCenter(), true);
	}

	void setColLays(b2FixtureDef& fix) {
		//switch (colLay_)
		//{
		//case 2:
		//	fix.filter.categoryBits = 0x0004; // tag para determinar capa de colision
		//	fix.filter.maskBits = 0x0004; // con que capas de colision se hace pues eso, colision
		//	break;
		//default:
		//	fix.filter.categoryBits = 0x0002;
		//	fix.filter.maskBits = 0x0002;
		//	break;
		//}
	}

	void update() override {

		//if(type == TYPE::DYNAMIC)
			//std::cout << body->GetPosition().x << " " << body->GetPosition().y << " " << body->GetAngle() << " " << tr_->getRot() << std::endl;

		actRenderPos();

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

	inline b2Body* getBody() const{
		return body;
	}

	inline b2Fixture* getFixture() const{
		return fixture;
	}

	/// <summary>
	/// Actualiza las coordenadas físicas de un cuerpo
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	inline void actPhyscialPos(int x, int y){
		int x_ = x / sdlutils().getPPM();
		int y_ = y / sdlutils().getPPM();

		b2Vec2 toMove(x_, y_);
		body->SetTransform(body->GetPosition() + toMove, body->GetAngle());

		actRenderPos();
	}

	inline void actRenderPos(){
		tr_->getPos().set(round((body->GetPosition().x * sdlutils().getPPM()) - tr_->getW() / 2.0f), round((body->GetPosition().y * sdlutils().getPPM()) - tr_->getH() / 2.0f));
	}


private:
	Transform* tr_;
	Vector2D pos, size;
	int type;
	bool isTrigger;
	float rotation_;
	float friction_;
	int colLay_;
	//bool entra = 0;
	bool fixedRotation_;
	b2World* world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

