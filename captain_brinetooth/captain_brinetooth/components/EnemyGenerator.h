#pragma once
#include "..//ecs/Component.h"
#include "../utils/Vector2D.h"
#include "../ecs/Entity.h"
#include"..//ecs/Manager.h"
#include "..//components/Transform.h"
#include "BoxCollider.h"
#include "../game/CollisionLayers.h"
#include "..//components/ElfSharkAttack.h"
#include "..//components/Enemy_Health.h"
#include "..//components/JellyHatBehavior.h"
#include "..//components/FringeHeadAtack.h"
#include "..//components/ContactDamage.h"
#include "..//components/PompeyWormAttack.h"

class EnemyGenerator : public Component
{
	const int NUM_ENEMIES = 4; //Indica la cantidad de tipos de enemigos que existen
public:
	struct Config {
		Vector2D pos, vel, size;
		float friction, rotation;
		bool fixedRotation, isTrigger;
		TYPE physicType;
		std::string spriteId;
		int filsSprite, colsSprite;
		uint16 col, colMask;

		Config() : 
			pos(), 
			vel(), 
			size(), 
			friction(), 
			rotation(), 
			filsSprite(), 
			colsSprite(), 
			col(), 
			colMask(), 
			physicType(DYNAMIC), 
			spriteId(), 
			isTrigger(), 
			fixedRotation() {
		}
	};

	EnemyGenerator();
	~EnemyGenerator();

	Entity* generateRandomEnemy(Vector2D pos);

private:


	Entity* generateFringeHead(Vector2D pos);
	Entity* generateMedusa(Vector2D pos);
	Entity* generateElfShark(Vector2D pos);
	Entity* generatePompeyWorm(Vector2D pos);

	Entity* createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel);
};