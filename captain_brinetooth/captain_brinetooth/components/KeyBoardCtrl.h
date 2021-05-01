// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once


#include <SDL.h>
#include <cassert>
#include "Transform.h"
#include "AnimBlendGraph.h"
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include "../sdlutils/InputHandler.h"
#include "..//components/MapProcedural.h"

//#include "Player_Health.h"


class KeyBoardCtrl: public Component {
public:
	KeyBoardCtrl(MapProcedural* m);

	virtual ~KeyBoardCtrl();


	inline void setSpeed(float speed) {
		speed_ = speed;
	}

	void init() override;

	void update() override;

private:
	Transform *tr_;
	BoxCollider* collider_;
	AnimBlendGraph* animController_;
	float speed_;

	//Debug
	MapProcedural* mapa;
};

