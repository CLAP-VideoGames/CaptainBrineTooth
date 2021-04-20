
#pragma once
#include <vector>
#include "box2d.h"

#include "../game/CollisionLayers.h"

#include "../components/Sword.h"
#include "../components/Button.h"
#include "../components/Hammer.h"
#include "../components/Rotate.h"
#include "../components/Bounce.h"
#include "../components/Image.h"
#include "../components/LoseLife.h"
#include "../components/Chainsaw.h"
#include "../components/Animation.h"
#include "../components/Transform.h"
#include "../components/Armas_HUD.h"
#include "../components/Rectangle.h"
#include "../components/BoxCollider.h"
#include "../components/CameraFollow.h"
#include "../components/EnemyTrigger.h"
#include "../components/KeyBoardCtrl.h"
#include "../components/Enemy_Health.h"
#include "../ecs/SoundManager.h"
#include "../components/MapCollider.h"
#include "../components/Player_Health.h"
#include "../components/enemyMovement.h"
#include "../components/MapProcedural.h"
#include "../components/AnimBlendGraph.h"
#include "../components/ElfSharkAttack.h"
#include "../components/PompeyWormAttack.h"
#include "../components/FringeHeadAtack.h"
#include "../components/PlayerController.h"
#include "../components/Crab.h"
#include "../components/MachineGun.h"
#include "../components/Image.h"
#include "../components/JellyHatBehavior.h"
#include "../components/Slider.h"
#include "../components/Inventory.h"
#include "../components/GetAWeaponOnCollision.h"
#include "../components/Gancho.h"
#include "../components/Cuerda.h"
#include "../components/PescaController.h"

class Manager;
class App;
class Entity;

class GameState
{
public:
	GameState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd);
	virtual ~GameState();

	struct Config {
		Vector2D pos, vel, size;
		float friction, rotation;
		bool fixedRotation, isTrigger;
		TYPE physicType;
		std::string spriteId;
		int filsSprite, colsSprite;
		uint16 col, colMask;

		Config() : pos(), vel(), size(), friction(), rotation(), filsSprite(), colsSprite(), col(), colMask(), physicType(STATIC), spriteId(), isTrigger(), fixedRotation() {
		}
	};

	Entity* createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel);

	virtual void render() const;
	virtual void update();
	virtual void refresh();
	virtual void init() {};

	Manager* getMngr() { return manager_; };
	App* getApp() { return app; };

	void popState() { popped = true; };
	bool isPopped() { return popped; };

protected:
	Manager* manager_;
	App* app;
	std::vector<Entity*> stage;
	bool popped;
};

