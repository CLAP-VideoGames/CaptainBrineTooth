#pragma once
#include <iostream>


#include "box2d.h"
#include <cmath>

#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "../sdlutils/SDLUtils.h"
#include <vector>

#include "tmxlite/Object.hpp"


//1 pixel(X) 0.0002645833 m
//then n pixels = n * 0.0002645833;

//enum TYPE { STATIC, DYNAMIC, KINEMATIC };

class ChainCollider : public Component {
public:
	template<class T>
	ChainCollider(std::vector<T> vertices, int typeAux = TYPE::STATIC, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true, float rotation = 0.0f){
		//type = typeAux;
		isTrigger = isTriggerAux;
		friction_ = friction;
		tr_ = nullptr;
		colLay_ = collisionLayer;
		colMask_ = collisionMask;
		fixedRotation_ = fixedRotation;
		rotation_ = rotation;

		
		setVertices(vertices);
	}

	virtual ~ChainCollider() {
	}

	void init() override {
		//tr_ = entity_->getComponent<Transform>();
		//assert(tr_ != nullptr);

		//pos = tr_->getPos();

		//size = Vector2D(tr_->getW() / sdlutils().getPPM(), tr_->getH() / sdlutils().getPPM());
		world = entity_->getWorld();

		/*b2BodyDef bodyDef;
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
		}*/
		
		//example();

	//	//Podemos usar un vector<b2Vec> para ir haciendo emplace back y una vez finalizado, 
		//copiar todos esos datos a un  b2Vec2* vs = new b2Vec2[value]; para crear la cadena
		b2BodyDef bdDef;
		bdDef.type = b2_staticBody;
		bdDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);
		b2Body* body = world->CreateBody(&bdDef);
		//b2Vec2* vs = new b2Vec2[sizeChain];

		b2ChainShape chain;
		//Vertice fantasmas inicial								//Vertice fantasmas final
		//chain.CreateLoop(vs, 4/*, b2Vec2(sdlutils().width() / 6.5f, sdlutils().height() / 2.0f), b2Vec2(sdlutils().width() / 1.0f, sdlutils().height() / 2.0f)*/);
		//chain.CreateChain(vs, sizeChain, b2Vec2((vs[0].x) - 30.0f / sdlutils().getPPM(), ((vs[0].y)) / sdlutils().getPPM()), b2Vec2((vs[sizeChain - 1].x) / sdlutils().getPPM(), ((vs[sizeChain - 1].y - 30)) / sdlutils().getPPM()));
		chain.CreateLoop(vs, sizeChain);

		body->SetFixedRotation(fixedRotation_);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &chain;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
		fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision
		b2Fixture* fix = body->CreateFixture(&fixtureDef);
	}

	void setSpeed(Vector2D speed){
		b2Vec2 vel = body->GetLinearVelocity();
		vel.x = speed.getX(); vel.y = speed.getY();
		body->SetLinearVelocity(vel);
	}

	/// <summary>
	/// Aplica una fuerza gradual en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyForce(Vector2D dir, float force = 1.0f) {
		dir.normalize();
		body->ApplyForce(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
	}

	/// <summary>
	/// Aplica una fuerza inmediata en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyLinearForce(Vector2D dir, float force = 1.0f) {
		dir.normalize();
		body->ApplyLinearImpulse(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
	}

	void update() override {
		//actRenderPos();
		//tr_->setRot((body->GetAngle() * (180.0f)) / M_PI);
	}

	inline b2Body* getBody() const {
		return body;
	}

	inline b2Fixture* getFixture() const {
		return fixture;
	}

	/// <summary>
	/// Actualiza las coordenadas físicas de un cuerpo
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	inline void actPhyscialPos(int x, int y) {
		int x_ = x / sdlutils().getPPM();
		int y_ = y / sdlutils().getPPM();

		b2Vec2 toMove(x_, y_);
		body->SetTransform(body->GetPosition() + toMove, body->GetAngle());

		actRenderPos();
	}

	inline void actRenderPos() {
		tr_->getPos().set(round((body->GetPosition().x * sdlutils().getPPM()) - tr_->getW() / 2.0f), round((body->GetPosition().y * sdlutils().getPPM()) - tr_->getH() / 2.0f));
	}

private:
	template<class T>
	void setVertices(const std::vector<T>& vector){
		sizeChain = vector.size();
		vs = new b2Vec2[sizeChain];
		for (int i = sizeChain - 1; i >= 0; i--){
			vs[(sizeChain - 1) - i ] = b2Vec2(vector[i].x, vector[i].y);
		}
		
		/*for (int i = 0; i < sizeChain; i++){
			vs[i] = b2Vec2(vector[i].x, vector[i].y);
		}*/
	}

	void example(){
		//Podemos usar un vector<b2Vec> para ir haciendo emplace back y una vez finalizado, 
		//copiar todos esos datos a un  b2Vec2* vs = new b2Vec2[value]; para crear la cadena
		b2BodyDef bdDef;
		bdDef.type = b2_staticBody;
		bdDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);
		b2Body* body = world->CreateBody(&bdDef);
		b2Vec2* vs = new b2Vec2[4];
		int alt1 = 2.3f;
		int alt2 = 2.0f;

		vs[0] = b2Vec2((sdlutils().width() / 15.0f) / sdlutils().getPPM(), (sdlutils().height() / alt1) / sdlutils().getPPM());
		vs[1] = b2Vec2((sdlutils().width() / 5.0f) / sdlutils().getPPM(), (sdlutils().height() / alt2) / sdlutils().getPPM());
		vs[2] = b2Vec2((sdlutils().width() / 0.5f) / sdlutils().getPPM(), (sdlutils().height() / alt2) / sdlutils().getPPM());
		vs[3] = b2Vec2((sdlutils().width() / 0.1f) / sdlutils().getPPM(), (sdlutils().height() / alt1) / sdlutils().getPPM());

		b2ChainShape chain;
		//Vertice fantasmas inicial								//Vertice fantasmas final
	//chain.CreateLoop(vs, 4/*, b2Vec2(sdlutils().width() / 6.5f, sdlutils().height() / 2.0f), b2Vec2(sdlutils().width() / 1.0f, sdlutils().height() / 2.0f)*/);
		chain.CreateChain(vs, 4, b2Vec2((sdlutils().width() / 16.0f) / sdlutils().getPPM(), (sdlutils().height() / 2.0f) / sdlutils().getPPM()), b2Vec2((sdlutils().width() / 1.0f) / sdlutils().getPPM(), (sdlutils().height() / 2.0f) / sdlutils().getPPM()));

		body->SetFixedRotation(fixedRotation_);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &chain;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.4f;
		fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
		fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision
		b2Fixture* fix = body->CreateFixture(&fixtureDef);
	}

	Transform* tr_;
	Vector2D pos, size;
	uint16 colLay_ , colMask_;
	int type, const sizeChain;
	bool isTrigger, fixedRotation_;
	float rotation_, friction_;

	b2Vec2* vs;
	//bool entra = 0;
	b2World* world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

