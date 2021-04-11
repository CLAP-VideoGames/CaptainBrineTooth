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


struct bodyChain {
	//Tamaño del array de vertices del cuerpo
	int sizeVs;
	//Definicion de vertices del cuerpo
	b2Vec2* vs;
	//Cuerpo
	b2Body* body_;
	//Fixture del cuerpo
	b2Fixture* fixt_;

};

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

		std::cout << "VERTICES FIXTURE" << std::endl;
		for (int i = 0;  i < bodies_.size(); i++){
			//Creamos el nuevo cuerpo
			bodies_[i].body_ = world->CreateBody(&bdDef);
			bodies_[i].body_->SetFixedRotation(fixedRotation_);

			b2ChainShape chain;
			//Vertice fantasmas inicial								//Vertice fantasmas final
			//chain.CreateLoop(vs, 4/*, b2Vec2(sdlutils().width() / 6.5f, sdlutils().height() / 2.0f), b2Vec2(sdlutils().width() / 1.0f, sdlutils().height() / 2.0f)*/);
			//chain.CreateChain(vs, sizeChain, b2Vec2((vs[0].x) - 30.0f / sdlutils().getPPM(), ((vs[0].y)) / sdlutils().getPPM()), b2Vec2((vs[sizeChain - 1].x) / sdlutils().getPPM(), ((vs[sizeChain - 1].y - 30)) / sdlutils().getPPM()));
			
			for (int j = 0; j < bodies_[i].sizeVs; j++){
				std::cout << bodies_[i].vs[j].x << " " << bodies_[i].vs[j].y << std::endl;
			}

			std::cout << "---" << std::endl;

			//le pasamos los vertices del cuerpo
			chain.CreateLoop(bodies_[i].vs, bodies_[i].sizeVs);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &chain;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.4f;
			fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
			fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision
			//Inicializamos la fixture del cuerpo
			bodies_[i].fixt_ = bodies_[i].body_->CreateFixture(&fixtureDef);
		}
	}

	void deleteChains() {

		//for (auto& fixt : fixtures) {
		//	delete fixt; fixt = nullptr;
		//}

		//for (auto& bod : bodies)
		//	world->DestroyBody(bod);
	}

	template<class T>
	void setVertices(const std::vector<vector<T>>& verticesVector){
		//Para cada definicion del lista de vertices, las introducimos en el vector de cuerpos
		for (int i = 0; i < verticesVector.size(); i++){
			bodyChain newBody;
			//Inicializamos el tamaño de la lista de vertices
			newBody.sizeVs = verticesVector[i].size();
			//Creamos un nuevo array para la lista de vectores
			newBody.vs = new b2Vec2[newBody.sizeVs];
			//Copiamos cada uno de los valores en cada una de las posiciones del array
			for (int j = 0; j < newBody.sizeVs; j++) {
				std::cout << verticesVector[i][j].x << " " << verticesVector[i][j].y << std::endl;
				newBody.vs[j] = b2Vec2(verticesVector[i][j].x, verticesVector[i][j].y);
			}

			std::cout << "---" << std::endl;

			//Introducimos el cuerpo en el vector
			bodies_.push_back(newBody);
		}
		//Por si por algun motivo los vertices se han puesto en sentido Antihorario : vs[(sizeChain - 1) - i ]
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

	//Vector de cuerpos
	std::vector<bodyChain> bodies_;
};

