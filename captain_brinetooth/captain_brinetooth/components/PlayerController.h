#pragma once

#include "Transform.h"
#include "AnimBlendGraph.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"

class PlayerController : public Component {
public:
	PlayerController()
	{

	}

	virtual ~PlayerController() {

	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		collider_ = entity_->getComponent<BoxCollider>();
		assert(collider_ != nullptr);
		animController_ = entity_->getComponent<AnimBlendGraph>();
		//assert(animController_ != nullptr);
	}


	void update() override {
		if (ih().keyDownEvent()) {
			assert(collider_ != nullptr);
			if (ih().isKeyDown(SDL_SCANCODE_UP)) {
				collider_->setSpeed(Vector2D(0.0f, -speed_));
			}
			else if (ih().isKeyDown(SDL_SCANCODE_DOWN)) {
				collider_->setSpeed(Vector2D(0.0f, speed_));
			}
			else if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
				collider_->setSpeed(Vector2D(-speed_, 0.0f));
			}
			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
				collider_->setSpeed(Vector2D(speed_, 0.0f));
			}
			else if (ih().isKeyDown(SDL_SCANCODE_SPACE)) {
				collider_->setSpeed(Vector2D(0.0f, 0.0f));
			}
			//Test animacion, Ejemplo de uso
			else if (ih().isKeyDown(SDL_SCANCODE_A)) {
				animController_->setParamValue("NotOnFloor", 1);
			}
			else if (ih().isKeyDown(SDL_SCANCODE_X)) {
				animController_->setParamValue("NotOnFloor", 0);
			}
		}

	}

	void methods()
	{
		//#region Vertical
		//	//Si presiono el salto, me elevo a una distancia predeterminada.
		//	//Inicializo una cuenta regresiva.
		//	if (Input.GetButtonDown("Jump") && isInRest)
		//	{
		//		rbParent.velocity = Vector2.up * jumpForce;
		//		isJumping = true;//isInRest se vuelve falso al dejar de tocar el suelo en el OnTriggerExit2D.

		//		anim.SetBool("isJumping", true);//Comienza la animación de salto.
		//		jumpCounter = jumpTime;
		//	}
		////Si mantengo el salto, es que he dejado de tocar el suelo y empieza una cuenta regresiva.
		////Mientras la cuenta no llegue a 0, seguirá subiendo
		//if (Input.GetButton("Jump") && isJumping)
		//{
		//	if (jumpCounter > 0)
		//	{
		//		rbParent.velocity = Vector2.up * jumpForce;
		//		jumpCounter -= Time.deltaTime;
		//	}
		//	else
		//	{
		//		isJumping = false;
		//		ReduceXSpeed();//reduzco la velocidad en el aire, solo cuando está cayendo
		//	}
		//}
		////En caso de que suelte el boton de saltar, no podre volver a
		////saltar hasta que esté en el suelo.
		//if (isJumping && !Input.GetButton("Jump")) //solamente se aplica cuando se ha saltado y dejo de presionar.
		//{
		//	isJumping = false;
		//	ReduceXSpeed();//reduzco la velocidad en el aire, solo cuando está cayendo
		//}

		//#endregion

		//	//Se detecta el input en cada frame y se ejecuta la animacion del jugador.
		//	#region Horizontal
		//	horizontalMovement = runVelocity * Input.GetAxis("Horizontal");
		////Animacion de movimiento
		//anim.SetFloat("xMove", Mathf.Abs(horizontalMovement));

		////Cambio de sentido del Sprite del jugador
		//if (Input.GetAxis("Horizontal") > 0) parent.localScale = new Vector3(1, 1, 1);
		//if (Input.GetAxis("Horizontal") < 0) parent.localScale = new Vector3(-1, 1, 1);

		//#endregion
	}


private:
	Transform* tr_;
	BoxCollider* collider_;
	AnimBlendGraph* animController_;
	float speed_;

};