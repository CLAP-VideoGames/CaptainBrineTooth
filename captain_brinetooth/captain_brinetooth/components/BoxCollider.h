#pragma once
#include <iostream>

#include "box2d.h"
#include <cmath>
#include "../utils/Vector2D.h"
#include "../ecs/Component.h"
#include "../ecs/Entity.h"
#include "Transform.h"
#include "Animation.h"
#include "AnimBlendGraph.h"
#include "../sdlutils/SDLUtils.h"
#include "../game/App.h"

//1 pixel(X) 0.0002645833 m
//then n pixels = n * 0.0002645833;

enum TYPE { STATIC, DYNAMIC, KINEMATIC };

class BoxCollider : public Component {
public:
	/// <summary>
	/// Crea un Collider en forma de caja
	/// </summary>
	/// <param name="typeAux">tipo físco del cuerpo</param>
	/// <param name="collisionLayer">Capa de colision </param>
	/// <param name="collisionMask">Capa de colision para ignorar</param>
	/// <param name="isTriggerAux"> determinar si es trigger</param>
	/// <param name="friction"> coeficiente de fricción</param>
	/// <param name="fixedRotation"> determinar si la rotación es fija</param>
	/// <param name="rotation"> rotacion en grados</param>
	/// <param name="pos_"> posición en caso de que no exista el componente Transform. Por defecto (0,0)</param>
	/// <param name="size_">tamaño en caso de que no exista el componente Transform. Por defecto (10,10)</param>
	BoxCollider(int typeAux = TYPE::STATIC, const uint16& collisionLayer = 0x0001, const uint16& collisionMask = 0xFFFF, bool isTriggerAux = false, float friction = 0.7f, bool fixedRotation = true, float rotation = 0.0f, const Vector2D& pos = Vector2D(0, 0), const Vector2D& size = Vector2D(10, 10)) :
		type(typeAux), isTrigger(isTriggerAux), friction_(friction), colLay_(collisionLayer),
		colMask_(collisionMask), fixedRotation_(fixedRotation), rotation_(rotation), pos_(pos), size_(size) {}

	virtual ~BoxCollider();

	void init() override;

	void update() override;

	void render() override;

	/// <summary>
	/// Aplica una velocidad dado un Vector2D dirección
	/// </summary>
	/// <param name="speed"></param>
	void setSpeed(Vector2D speed);

	/// <summary>
	/// Aplica una fuerza gradual en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyForce(Vector2D dir, float force = 1.0f);

	/// <summary>
	/// Aplica una fuerza inmediata en la direccion indicada
	/// </summary>
	/// <param name="dir">Vector2D de dirección</param>
	/// <param name="force">Fuerza aplicada</param>
	void applyLinearForce(Vector2D dir, float force = 1.0f);

	/// <summary>
	/// Devuelve el cuerpo físico
	/// </summary>
	/// <returns></returns>
	b2Body* getBody();

	/// <summary>
	/// Devuelve la fixtura (rigidBody) del cuerpo físico
	/// </summary>
	/// <returns>b2Fixture*</returns>
	b2Fixture* getFixture();

	/// <summary>
	/// Actualiza las coordenadas físicas de un cuerpo en base a las actuales
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	 void actPhyscialPos(int x, int y);

	/// <summary>
	/// Cambia las coordenadas físicas de un cuerpo y su rotación
	/// </summary>
	/// <param name="x">in pixels</param>
	/// <param name="y">in pixels</param>
	/// <param name="rotation">in degrees</param>
	 void setPhysicalTransform(int x, int y, float degrees);

	/// <summary>
	/// Devuelve la posición físicas en metros
	/// </summary>
	/// <returns></returns>
	 Vector2D getPhysicalPos();

	/// <summary>
	/// Actualiza los valores de la posición en funcion de la posición del cuerpo físico.
	/// </summary>
	void actRenderPos();


private:
	Transform* tr_;
	Vector2D pos_, size_;
	int type;
	bool isTrigger;
	float rotation_, friction_;
	uint16 colLay_, colMask_;

	//bool entra = 0;
	bool fixedRotation_;
	std::shared_ptr<b2World> world = nullptr;
	b2Body* body = nullptr;
	b2Fixture* fixture = nullptr;

};

