
#pragma once
#include <vector>
#include "box2d.h"

#include "../game/CollisionLayers.h"
#include "../game/App.h"

#include "../components/Sword.h"
#include "../components/Button.h"
#include "../components/Hammer.h"
#include "../components/Rotate.h"
#include "../components/Bounce.h"
#include "../components/Image.h"
#include "../components/ParallaxScroll.h"
#include "../components/LoseLife.h"
#include "../components/Chainsaw.h"
#include "../components/Animation.h"
#include "../components/Transform.h"
#include "../components/Armas_HUD.h"
#include "../components/Rectangle.h"
#include "../components/BoxCollider.h"
#include "../components/TriggerCollider.h"
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
#include "../components/ThornFishAttack.h"
#include "../components/PlayerController.h"
#include "../components/Crab.h"
#include "../components/MachineGun.h"
#include "../components/Image.h"
#include "../components/JellyHatBehavior.h"
#include "../components/Slider.h"
#include "../components/ExplanationMessage.h"
#include "../components/Inventory.h"
#include "../components/GetAWeaponOnCollision.h"
#include "../components/Gancho.h"
#include "../components/Cuerda.h"
#include "../components/Reward.h"
#include "../components/PescaController.h"
#include "../components/VideoPlayer.h"
#include "../components/Fade.h"
#include "../components/FishlerController.h"
#include "../components/SkillTree.h"

class Manager;
class App;
class Entity;

//Info de guardado
struct GameInfo {
	std::array<bool, 6> abilities;
	int points;
};

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
	virtual void init();

	SDL_Rect ScaleSDL_Rect(Texture* text, const Vector2D& pos, float zoomFactor, float sizeFactor, bool centered = false);

	Manager* getMngr() { return manager_; };
	App* getApp() { return app; };

	void popState() { popped = true; };
	bool isPopped() { return popped; };

	void saveGame();
	void loadGame();
	void deleteSaveFile();

	GameInfo gameInfo;

protected:
	Manager* manager_;
	App* app;
	Entity* fader;
	std::vector<Entity*> stage;
	bool popped;
};

