
#pragma once
#include <vector>
#include "box2d.h"

#include "../game/CollisionLayers.h"

#include "../components/SoundManager.h"
#include "../components/AnimBlendGraph.h"
#include "../components/Animation.h"
#include "../components/Button.h"
#include "../components/Transform.h"
#include "../components/ElfSharkAttack.h"
#include "../components/EnemyTrigger.h"
#include "../components/Rotate.h"
#include "../components/ChainCollider.h"
#include "../components/Bounce.h"
#include "../components/Armas_HUD.h"
#include "../components/Rectangle.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Player_Health.h"
#include "../components/enemyMovement.h"
#include "../components/PlayerController.h"
#include "../components/Chainsaw.h"
#include "../components/CameraFollow.h"
#include "../components/LoseLife.h"
#include "../components/Sword.h"
#include "../components/Hammer.h"
#include "../components/MapProcedural.h"
#include "../components/BoxCollider.h"
#include "../components/FringeHeadAtack.h"
#include "../components/Enemy_Health.h"

class Manager;
class App;
class Entity;


class GameState
{
protected:
	Manager* manager_;
	App* app;
	SoundManager* soundController;
	std::vector<Entity*> stage;
public:
	GameState(App* a, b2World* mundo, SoundManager* snd);
	virtual ~GameState();
	virtual void render() const;
	virtual void update();
	virtual void refresh();
	virtual void init() {};
	Manager* getMan();
};

