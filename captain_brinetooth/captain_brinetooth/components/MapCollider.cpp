#include "MapCollider.h"

MapCollider::~MapCollider()
{
	//Destruimos todos los cuerpos 
	deleteChains();
}

void MapCollider::init()
{
	world = entity_->getWorld();

	createChainFixture();
}

void MapCollider::render()
{
	if (sdlutils().getDebug()) {
		for (int i = 0; i < bodies_.size(); i++) {
			SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderDrawLines(sdlutils().renderer(), bodies_[i].realPoints, bodies_[i].sizeVs + 1);
		}
	}
}

void MapCollider::update()
{
	if (sdlutils().getDebug()) {
		//Hay que hacer esto pero no quiero hacer esto pero hay que hacer esto pero no quiero hacer esto pero aun asi hay que hacerlo
		for (bodyChain& bodyC : bodies_) {
			for (int i = 0; i < bodyC.sizeVs + 1; i++) {
				//Quitamos el valor antiguo aplicado al punto, y le aplicamos el valor actual de la cámara 
				bodyC.realPoints[i].x = bodyC.realPoints[i].x + camPos.getX() - App::camera.x;
				bodyC.realPoints[i].y = bodyC.realPoints[i].y + camPos.getY() - App::camera.y;
			}
		}
		//Almacenamos la posicion de la cámara para poder contrarestar el valor a los vertices
		camPos = Vector2D(App::camera.x, App::camera.y);
	}
}

void MapCollider::createChainFixture() {
	bdDef.type = b2_staticBody;
	bdDef.userData.pointer = reinterpret_cast<uintptr_t>(entity_);

	if (sdlutils().getConsoleDebug()) std::cout << "VERTICES FIXTURE" << std::endl;

	for (int i = 0; i < bodies_.size(); i++) {
		//Creamos el nuevo cuerpo

		bodies_[i].body_ = world->CreateBody(&bdDef);
		bodies_[i].body_->SetFixedRotation(fixedRotation_);

		b2ChainShape chain;

		//Vertice fantasmas inicial								//Vertice fantasmas final
		//chain.CreateLoop(vs, 4/*, b2Vec2(sdlutils().width() / 6.5f, sdlutils().height() / 2.0f), b2Vec2(sdlutils().width() / 1.0f, sdlutils().height() / 2.0f)*/);
		//chain.CreateChain(vs, sizeChain, b2Vec2((vs[0].x) - 30.0f / sdlutils().getPPM(), ((vs[0].y)) / sdlutils().getPPM()), b2Vec2((vs[sizeChain - 1].x) / sdlutils().getPPM(), ((vs[sizeChain - 1].y - 30)) / sdlutils().getPPM()));
		//Posicion de los vertices para debuggear
		if (sdlutils().getConsoleDebug()) {
			for (int j = 0; j < bodies_[i].sizeVs; j++) {
				std::cout << bodies_[i].vs[j].x << " " << bodies_[i].vs[j].y << std::endl;
			}
			std::cout << "---" << std::endl;
		}
		//le pasamos los vertices del cuerpo
		chain.CreateLoop(bodies_[i].vs, bodies_[i].sizeVs);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &chain;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.filter.categoryBits = colLay_; // tag para determinar capa de colision
		fixtureDef.filter.maskBits = colMask_; // con que capas de colision se hace pues eso, colision
		//Inicializamos la fixture del cuerpo
		bodies_[i].fixt_ = bodies_[i].body_->CreateFixture(&fixtureDef);
	}
}

void MapCollider::deleteChains()
{
	for (auto& bodyC : bodies_) {
		bodyC.fixt_ = nullptr;
		bodyC.body_->GetWorld()->DestroyBody(bodyC.body_);

		delete[] bodyC.vs;
		delete[] bodyC.realPoints;
		int m = 10;
	}

	bodies_.clear();
}
