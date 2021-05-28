#include "BoxCollider.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

BoxCollider::~BoxCollider() {
	body->GetWorld()->DestroyBody(body);
	int m = 10;
}

void BoxCollider::init() {
	tr_ = entity_->getComponent<Transform>();

	//assert(tr_ != nullptr);
	//Actualizamos la posicion en caso de que tenga un componente Transform
	if (tr_ != nullptr)
	{
		if (entity_->getComponent<AnimBlendGraph>() != nullptr) {
			Vector2D anchorPoint = entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->anchor();
			pos_ = Vector2D(tr_->getPos().getX() - (size_.getX() * anchorPoint.getX()), tr_->getPos().getY() - (size_.getY() * anchorPoint.getY()));
		}
		else {
			pos_ = tr_->getPos();
		}
		if (size_.getX() <= 0 || size_.getY() <= 0)
			size_ = Vector2D(tr_->getW(), tr_->getH());
		else
			size_ = Vector2D(size_.getX(), size_.getY());
	}
	else {
		//Pasamos el tamaño a medidas de box2d
		pos_ = Vector2D(pos_.getX() / sdlutils().getPPM(), pos_.getY() / sdlutils().getPPM());
	}

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

	float x = pos_.getX() / sdlutils().getPPM();
	float y = pos_.getY() / sdlutils().getPPM();
	bodyDef.position.Set(x, y);
	bodyDef.angle = rotation_;

	//Stores the entity in the body for future reference in collisions
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);

	//Make the body
	body = world->CreateBody(&bodyDef);

	b2PolygonShape boxShape;
	boxShape.SetAsBox(size_.getX() / sdlutils().getPPM() / 2.0f, size_.getY() / sdlutils().getPPM() / 2.0f);

	b2FixtureDef fixtureDef;
	//Que el cubo real tenga la misma forma que la definicion
	fixtureDef.shape = &boxShape;
	//No puede ser la densidad 0 para un objeto dinamico
	fixtureDef.density = density_;
	fixtureDef.friction = friction_;
	fixtureDef.isSensor = isTrigger;

	body->SetFixedRotation(fixedRotation_);

	fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
	fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision

	fixture = body->CreateFixture(&fixtureDef);

	isTriggerColliding_ = false;
}

void BoxCollider::update() {
	actRenderPos();

	if (tr_ != nullptr)
		tr_->setRot((body->GetAngle() * (180.0f)) / M_PI);

	/*�Custom method to detect collisions and delete a body
	b2ContactEdge* b;

	b = body->GetContactList();

	if (b != nullptr && entra == false) {
		std::cout << "Collided";
		world->DestroyBody(b->contact->GetFixtureA()->GetBody());
		entra = 1;
	}*/
}

void BoxCollider::render() {
	#ifdef _DEBUG
	if (sdlutils().getDebug()) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 0, 255, 0, 255);

		int w = size_.getX();
		int h = size_.getY();

		//Ya que la posición de un objeto físico es el centro de la masa, tenemos que llevar el punto a la parte superior izquierda
		//Le restamos la posición de la cámara
		float x = round((body->GetPosition().x * sdlutils().getPPM()) - (w / 2.0f)) - App::camera.x;
		float y = round((body->GetPosition().y * sdlutils().getPPM()) - (h / 2.0f)) - App::camera.y;
		SDL_Rect dest = build_sdlrect(x, y, w, h);

		SDL_RenderDrawRect(sdlutils().renderer(), &dest);
	}
	#endif
}

void BoxCollider::setSpeed(Vector2D speed) {
	b2Vec2 vel = body->GetLinearVelocity();
	vel.x = speed.getX(); vel.y = speed.getY();
	body->SetLinearVelocity(vel);
}

void BoxCollider::applyForce(Vector2D dir, float force) {
	dir.normalize();
	body->ApplyForce(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
}

void BoxCollider::applyLinearForce(Vector2D dir, float force)
{
	dir.normalize();
	body->ApplyLinearImpulse(b2Vec2(dir.getX() * force, dir.getY() * force), body->GetWorldCenter(), true);
}

 b2Body* BoxCollider::getBody(){
	return body;
}

 b2Fixture* BoxCollider::getFixture() {
	return fixture;
}

 void BoxCollider::actPhyscialPos(float x, float y)
{
	float x_ = x / sdlutils().getPPM();
	float y_ = y / sdlutils().getPPM();

	b2Vec2 toMove(x_, y_);
	body->SetTransform(body->GetPosition() + toMove, body->GetAngle());

	actRenderPos();
}

 void BoxCollider::setPhysicalTransform(float x, float y, float degrees) {
	float x_ = x / sdlutils().getPPM();
	float y_ = y / sdlutils().getPPM();
	float newRot = (degrees * M_PI) / (180.0f);

	b2Vec2 toMove(x_, y_);
	body->SetTransform(toMove, newRot);
	actRenderPos();
}

 Vector2D BoxCollider::getPhysicalPos()
{
	b2Vec2 physpos = body->GetPosition();
	Vector2D position;
	position.set(Vector2D(physpos.x * sdlutils().getPPM(), physpos.y * sdlutils().getPPM()));
	return position;
}

 void BoxCollider::actRenderPos()
{
	//Actualizamos la posición difereciando entre si es del Transform o sólo del parámetro Pos
	if (tr_ != nullptr) {
		if (entity_->getComponent<AnimBlendGraph>() != nullptr) {
			Vector2D anchorPoint = entity_->getComponent<AnimBlendGraph>()->getCurrentAnimation()->anchor();
			tr_->getPos().set(round((body->GetPosition().x * sdlutils().getPPM())), round((body->GetPosition().y) * sdlutils().getPPM()));
		}
		else {
			tr_->getPos().set(round(body->GetPosition().x * sdlutils().getPPM()), round(body->GetPosition().y * sdlutils().getPPM()));
		}
	}
	else {
		pos_.set(round((body->GetPosition().x * sdlutils().getPPM()) - size_.getX() / 2.0f), round((body->GetPosition().y * sdlutils().getPPM()) - size_.getY() / 2.0f));
	}
}

 void BoxCollider::triggerCollide(bool state)
 {
	 isTriggerColliding_ = state;
 }

 const uint16& BoxCollider::getColLayer() const
 {
	 return colLay_;
 }

 const Vector2D& BoxCollider::getSize()
 {
	 return size_;
 }

 const uint16& BoxCollider::getColMask() const
 {
	 return colMask_;
 }

 const bool& BoxCollider::isTriggerColliding()
 {
	 return isTriggerColliding_;
 }

 void BoxCollider::Resize(Vector2D size, Vector2D anchor)
 {
	 size_ = size;

	 auto type = body->GetType();
	 auto bodypos = body->GetPosition();

	 body->DestroyFixture(fixture);
	 fixture = nullptr;
	 world->DestroyBody(body);
	 body = nullptr;

	 b2BodyDef bodyDef;
	 bodyDef.type = type;

	 bodyDef.position.Set(bodypos.x, bodypos.y);
	 bodyDef.angle = rotation_;

	 //Stores the entity in the body for future reference in collisions
	 bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);

	 //Make the body
	 body = world->CreateBody(&bodyDef);

	 b2PolygonShape boxShape;
	 boxShape.SetAsBox(size_.getX() / sdlutils().getPPM() * 0.5f, size_.getY() / sdlutils().getPPM() *0.5f);

	 b2FixtureDef fixtureDef;
	 //Que el cubo real tenga la misma forma que la definicion
	 fixtureDef.shape = &boxShape;
	 //No puede ser la densidad 0 para un objeto dinamico
	 fixtureDef.density = density_;
	 fixtureDef.friction = friction_;
	 fixtureDef.isSensor = isTrigger;

	 fixtureDef.filter.categoryBits = getColLayer(); // tag para determinar capa de colision
	 fixtureDef.filter.maskBits = getColMask(); // con que capas de colision se hace pues eso, colision

	 fixture = body->CreateFixture(&fixtureDef);
 }

 void BoxCollider::changeColLayer_and_Mask(const uint16& colL, const uint16& colM)
 {
	 b2Filter filt;
	 filt.categoryBits = colL;
	 filt.groupIndex = fixture->GetFilterData().groupIndex;
	 filt.maskBits = colM;
	 fixture->SetFilterData(filt);
 }
