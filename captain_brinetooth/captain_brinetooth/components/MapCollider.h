#pragma once
#include <iostream>
#include <cmath>
#include <vector>

#include "../sdlutils/SDLUtils.h"
#include "tmxlite/Object.hpp"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "../game/App.h"
#include "Transform.h"

struct bodyChain {
	//Tamaño del array de vertices del cuerpo
	int sizeVs;
	//Definicion de vertices del cuerpo
	b2Vec2* vs;
	//Definicion de vertices del cuerpo en coordenadas de pixeles
	SDL_Point* realPoints;
	//Cuerpo
	b2Body* body_;
	//Fixture del cuerpo
	b2Fixture* fixt_;
};

class MapCollider : public Component {
public:
	template<class T>
	MapCollider(std::vector<std::vector<T>> vertices, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true, float rotation = 0.0f){
		isTrigger = isTriggerAux;
		friction_ = friction;
		tr_ = nullptr;
		colLay_ = collisionLayer;
		colMask_ = collisionMask;
		fixedRotation_ = fixedRotation;
		rotation_ = rotation;

		setVertices(vertices);
	}

	virtual ~MapCollider();

	void init() override;

	void render() override;

	void update() override;

	void createChainFixture();

	void deleteChains();

	template<class T>
	void setVertices(const std::vector<std::vector<T>>& verticesVector){
		//Para cada definicion del lista de vertices, las introducimos en el vector de cuerpos
		for (int i = 0; i < verticesVector.size(); i++){
			bodyChain newBody;
			//Inicializamos el tamaño de la lista de vertices
			newBody.sizeVs = verticesVector[i].size();
			//Creamos un nuevo array para la lista de vectores
			newBody.vs = new b2Vec2[newBody.sizeVs];
			//Creamos un nuevo array para la lista de vectores para que se puedan ver en pantalla los vertices
			newBody.realPoints = new SDL_Point[newBody.sizeVs + 1];
			//Copiamos cada uno de los valores en cada una de las posiciones del array
			int j;

			for (j = 0; j < newBody.sizeVs; j++) {
				if (sdlutils().getConsoleDebug())
					std::cout << verticesVector[i][j].x << " " << verticesVector[i][j].y << std::endl;

				newBody.vs[j] = b2Vec2(verticesVector[i][j].x, verticesVector[i][j].y);
				SDL_Point point = {verticesVector[i][j].x * sdlutils().getPPM(), verticesVector[i][j].y * sdlutils().getPPM()};
				newBody.realPoints[j] = point;
			}
			//Para que conecte con el del final. No es Loop.
			newBody.realPoints[j] = newBody.realPoints[0];

			if (sdlutils().getConsoleDebug())
				std::cout << "---" << std::endl;

			//Introducimos el cuerpo en el vector
			bodies_.push_back(newBody);
		}
		//Por si por algun motivo los vertices se han puesto en sentido Antihorario : vs[(sizeChain - 1) - i ]

		//SDL_Log("Mouse position: x=%i y=%i",  //    Print mouse position
		//	mouse_position.x, mouse_position.y
		//);

		
	}

	/// <summary>
	/// Devuelve el layer de colision 
	/// </summary>
	const uint16& getColLayer() { return colLay_; };

	/// <summary>
	/// Devuelve la mascara de colision 
	/// </summary>
	const uint16& getColMask() { return colMask_; };

private:
	//Body parameters
	Transform* tr_;
	Vector2D pos, size;
	uint16 colLay_ , colMask_;
	int type, const sizeVerticesChain;
	bool isTrigger, fixedRotation_;
	float rotation_, friction_;
	b2BodyDef bdDef;
	//Puntero al mundo
	std::shared_ptr<b2World> world = nullptr;
	//Vector de cuerpos
	std::vector<bodyChain> bodies_;
	//Cam position
	Vector2D camPos = Vector2D(0,0);
};

