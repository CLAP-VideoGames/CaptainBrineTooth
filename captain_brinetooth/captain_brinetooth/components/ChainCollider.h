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
	ChainCollider(std::vector<vector<T>> vertices, int typeAux = TYPE::STATIC, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true, float rotation = 0.0f){
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

		bdDef;
		bdDef.type = b2_staticBody;
		bdDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);

		createChainFixture();
	}

	void createChainFixture(){
		for (int i = 0;  i < vs.size(); i++){
			b2Body* b = world->CreateBody(&bdDef);
			bodies.push_back(b);
			b->SetFixedRotation(fixedRotation_);

			b2ChainShape chain;
			//Vertice fantasmas inicial								//Vertice fantasmas final
			//chain.CreateLoop(vs, 4/*, b2Vec2(sdlutils().width() / 6.5f, sdlutils().height() / 2.0f), b2Vec2(sdlutils().width() / 1.0f, sdlutils().height() / 2.0f)*/);
			//chain.CreateChain(vs, sizeChain, b2Vec2((vs[0].x) - 30.0f / sdlutils().getPPM(), ((vs[0].y)) / sdlutils().getPPM()), b2Vec2((vs[sizeChain - 1].x) / sdlutils().getPPM(), ((vs[sizeChain - 1].y - 30)) / sdlutils().getPPM()));
			
			chain.CreateLoop(vs[i], sizeVerticesChain);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &chain;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.4f;
			fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
			fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision
			b2Fixture* fixt = b->CreateFixture(&fixtureDef);
			fixtures.push_back(fixt);
		}

	}

	void deleteChainFixture() {
		for (auto& fixt : fixtures) {
			delete fixt; fixt = nullptr;
		}

		for (auto& bod : bodies)
			world->DestroyBody(bod);
	}

	template<class T>
	void setVertices(const std::vector<vector<T>>& vector){
		sizeVerticesChain = vector.size();

		for (int i = 0; i < sizeVerticesChain; i++){
			int sizeVertices = vector[i].size();
			b2Vec2* vertices = new b2Vec2[sizeVertices];
			for (int j = 0; j < sizeVertices; j++){
				vertices[j] = b2Vec2(vector[i][j].x, vector[i][j].y);
			}
			vs.push_back(vertices);
		}
		//for (int i = 0; i < sizeVerticesChain; i++) {
		//	//Nuevo array de vertices
		//	int sizeVertices = vector[i].size();
		//	b2Vec2* vertices = new b2Vec2[sizeVertices];
		//	for (int j = 0; j < sizeVertices; j++)
		//		vertices[i] = b2Vec2(vector[i][j].x, vector[i][j].y);

		//	for (int j = 0; j < sizeVertices; j++)
		//		std::cout << vertices[j].x << " " << vertices[j].y << std::endl;

		//	vs.push_back(vertices);
		//}
		//Por si por algun motivo los vertices se han puesto en sentido Antihorario
		/*for (int i = sizeChain - 1; i >= 0; i--){
			vs[(sizeChain - 1) - i ] = b2Vec2(vector[i].x, vector[i].y);
		}*/
	}

private:

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
	int type, const sizeVerticesChain;
	bool isTrigger, fixedRotation_;
	float rotation_, friction_;

	//bool entra = 0;
	b2BodyDef bdDef;

	b2World* world = nullptr;
	std::vector<b2Vec2*> vs;
	std::vector<b2Body*> bodies;
	std::vector<b2Fixture*> fixtures;
};

