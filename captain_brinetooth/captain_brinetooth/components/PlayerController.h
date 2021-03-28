#pragma once

#include "Transform.h"
#include "AnimBlendGraph.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"

#include "Player_Health.h"

class PlayerController : public Component {
public:																
	PlayerController(const float & speed = 3.0f, const float& forceJ = 3.0f):
															//falso				//falso
		tr_(nullptr), speed_(speed), forceJump_(forceJ), isOnFloor(true), isOnAir(false){

	}

	virtual ~PlayerController() {

	}

	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);
		collider_ = entity_->getComponent<BoxCollider>();
		assert(collider_ != nullptr);

		animController_ = entity_->getComponent<AnimBlendGraph>();

		lastTimeJumped = sdlutils().currRealTime();

		//health_ = entity_->getComponent<Player_Health>();
		//assert(health_!= nullptr);

	}


	void update() override {
		if (ih().keyDownEvent()) {
			assert(collider_ != nullptr);
			
			if (ih().isKeyDown(SDL_SCANCODE_LEFT)) {
				collider_->setSpeed(Vector2D(-speed_, 0.0f));
			}
			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT)) {
				collider_->setSpeed(Vector2D(speed_, 0.0f));
			}
			
			//Parte Vertical
			if (ih().isKeyDown(SDL_SCANCODE_SPACE) && isOnFloor){
				isOnFloor = false;

				//collider_->applyForce(Vector2D(0, -1), forceJump_ * 44.0f); Al ser gradual, le cuesta mucho más
				collider_->applyLinearForce(Vector2D(0, -1), forceJump_);
				
				//Realizar daño
				//health_->loseLife();

				//Test animacion de salto, Ejemplo de uso
				if(animController_ != nullptr )
					if(animController_->searchParamValue("NotOnFloor") != -1)
						animController_->setParamValue("NotOnFloor", 1);
			}
			else if (ih().isKeyDown(SDL_SCANCODE_X)) {
				
			}
	
		}

		//Timer provisional hasta tener los Triggers de colision, para preparar bien el saltos
		if (sdlutils().currRealTime() - lastTimeJumped >= time)
		{
			isOnFloor = true;
			lastTimeJumped = sdlutils().currRealTime();

			if (animController_ != nullptr)
				if (animController_->searchParamValue("NotOnFloor") != -1)
						animController_->setParamValue("NotOnFloor", 0);
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

		//private void OnTriggerEnter2D(Collider2D collision)
		//{
		//	if (!isInRest && !isJumping)
		//	{
		//		anim.SetBool("isJumping", false);
		//		anim.SetBool("isFalling", false);
		//		anim.SetTrigger("OnLand");
		//	}

		//	ResetSpeed();
		//	isInRest = true;
		//}

		//private void OnTriggerExit2D(Collider2D collision)
		//{
		//	isInRest = false;

		//	if (!isJumping)
		//		anim.SetBool("isFalling", true); //Si sale de una plataforma y no esta saltando, empieza la animacion de caida.
		//}
	}


private:
	Transform* tr_ = nullptr;
	BoxCollider* collider_ = nullptr;
	AnimBlendGraph* animController_ = nullptr;
	Player_Health* health_ = nullptr;


	int lastTimeJumped; 
	int time = 1500;
	float speed_, forceJump_;
	bool isOnFloor, isOnAir;

};