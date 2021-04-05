#pragma once

#include "Transform.h"
#include "AnimBlendGraph.h"
#include "BoxCollider.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"

#include "SoundManager.h"

class PlayerController : public Component {
public:																
	PlayerController(const float & speed = 3.0f, const float& forceJ = 5.0f , const float& dashS = 7.0f):
															//falso				//falso
		tr_(nullptr), speed_(speed), forceJump_(forceJ), isOnFloor(true), isOnAir(false), dashSpeed(dashS), isDashing(false), canDash(true){

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

		lasTimeDashed = sdlutils().currRealTime();

		snd = entity_->getComponent<SoundManager>();
		assert(snd!= nullptr);
	}

	//Necesitamos un getter de la velocidad del collider
	void update() override {
		if (ih().keyDownEvent()) {
			assert(collider_ != nullptr);
			//Parte Horizontal
			if (ih().isKeyDown(SDL_SCANCODE_LEFT) && !isDashing) {
				b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
				collider_->setSpeed(Vector2D(-speed_, vel.y));
				snd->playSoundEffect("walk");
				animController_->setParamValue("Speed", 1);
				animController_->flipX(false);
			}
			else if (ih().isKeyDown(SDL_SCANCODE_RIGHT) && !isDashing) {
				b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
				collider_->setSpeed(Vector2D(speed_, vel.y));
				snd->playSoundEffect("walk");
				animController_->setParamValue("Speed", 1);
				animController_->flipX(true);
			}
			
			//Parte Vertical
			if (ih().isKeyDown(SDL_SCANCODE_SPACE) && isOnFloor && !isDashing){
				isOnFloor = false;
				b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
				//collider_->applyForce(Vector2D(0, -1), forceJump_ * 44.0f); Al ser gradual, le cuesta mucho m�s
				collider_->applyLinearForce(Vector2D(0, -1), forceJump_);
				
				//Realizar da�o
				//health_->loseLife();

				animController_->setParamValue("NotOnFloor", 1);
				snd->playSoundEffect("player_jump");
			}
			else if (ih().isKeyDown(SDL_SCANCODE_X)) {
				//snd->ChangeMainMusic("adventure");
				snd->setGeneralVolume(snd->GeneralVolume() - 5);
			}
			else if (ih().isKeyDown(SDL_SCANCODE_Z)) {
				//snd->ChangeMainMusic("adventure");
				snd->setGeneralVolume(snd->GeneralVolume() + 5);
			}

			if (ih().isKeyDown(SDLK_LSHIFT) && !isDashing && canDash) {
				gravity = collider_->getBody()->GetGravityScale();
				collider_->getBody()->SetGravityScale(0.0f);
				collider_->setSpeed(Vector2D(0, 0));
				isDashing = true;
				collider_->applyLinearForce(Vector2D(1,0), dashSpeed);
				canDash = false;
			}

		}

		if(collider_->getBody()->GetLinearVelocity().x == 0){
			animController_->setParamValue("Speed", 0);
		}

		if (isDashing) {
			b2Vec2 vel = collider_->getBody()->GetLinearVelocity();
			if (vel.x > 0) {
				collider_->setSpeed(Vector2D(vel.x - 0.3, 0));
			}
			else {
				collider_->getBody()->SetGravityScale(gravity);
				isDashing = false;
			}
		}

		if (sdlutils().currRealTime() - lasTimeDashed >= dashCoolDown) {
			lasTimeDashed = sdlutils().currRealTime();
			canDash = true;
		}

		//Timer provisional hasta tener los Triggers de colision, para preparar bien el saltos
		if (sdlutils().currRealTime() - lastTimeJumped >= time)
		{
			isOnFloor = true;
			lastTimeJumped = sdlutils().currRealTime();

			animController_->setParamValue("NotOnFloor", 0);
		}
	}
	//Joseda es tonto
	void methods()
	{
		//#region Vertical
		//	//Si presiono el salto, me elevo a una distancia predeterminada.
		//	//Inicializo una cuenta regresiva.
		//	if (Input.GetButtonDown("Jump") && isInRest)
		//	{
		//		rbParent.velocity = Vector2.up * jumpForce;
		//		isJumping = true;//isInRest se vuelve falso al dejar de tocar el suelo en el OnTriggerExit2D.

		//		anim.SetBool("isJumping", true);//Comienza la animaci�n de salto.
		//		jumpCounter = jumpTime;
		//	}
		////Si mantengo el salto, es que he dejado de tocar el suelo y empieza una cuenta regresiva.
		////Mientras la cuenta no llegue a 0, seguir� subiendo
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
		//		ReduceXSpeed();//reduzco la velocidad en el aire, solo cuando est� cayendo
		//	}
		//}
		////En caso de que suelte el boton de saltar, no podre volver a
		////saltar hasta que est� en el suelo.
		//if (isJumping && !Input.GetButton("Jump")) //solamente se aplica cuando se ha saltado y dejo de presionar.
		//{
		//	isJumping = false;
		//	ReduceXSpeed();//reduzco la velocidad en el aire, solo cuando est� cayendo
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
	SoundManager* snd = nullptr;


	int lastTimeJumped; 
	int time = 1500, dashCoolDown = 10000, lasTimeDashed;
	float speed_, forceJump_, maxSpeed, dashSpeed, gravity;
	bool isOnFloor, isOnAir, isDashing, canDash;
	
};