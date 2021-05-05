#include "EnemyGenerator.h"


EnemyGenerator::EnemyGenerator()
{

}

EnemyGenerator::~EnemyGenerator()
{

}

Entity* EnemyGenerator::generateRandomEnemy(Vector2D pos)
{
	int rnd = sdlutils().rand().teCuoto(0, NUM_ENEMIES);
	switch (rnd)
	{
	case 0:
		return generateElfShark(pos);
		//generateFringeHead(pos);
		break;
	case 1:
		return generateMedusa(pos);
		break;
	case 2:
		return generateFringeHead(pos);
		break;
	case 3:
		return generatePompeyWorm(pos);
		break;
	default:
		break;
	}
}

Entity* EnemyGenerator::generateFringeHead(Vector2D pos)
{
	auto* enemy = createBasicEntity(pos, Vector2D(20,20), 0, Vector2D(0,0));
	enemy->addComponent<BoxCollider>(DYNAMIC, ENEMY, ENEMY_MASK);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);

	return enemy;
}

Entity* EnemyGenerator::generateMedusa(Vector2D pos)
{
	Config flowerJellyHat{};
	flowerJellyHat.pos = pos;
	flowerJellyHat.vel = Vector2D(0, 0);
	flowerJellyHat.size = Vector2D(100.0f, 100.0f);
	flowerJellyHat.friction = 0.2f;
	flowerJellyHat.physicType = DYNAMIC;
	flowerJellyHat.fixedRotation = true;
	flowerJellyHat.rotation = 0.0f;
	flowerJellyHat.col = ENEMY;
	flowerJellyHat.colMask = ENEMY_MASK;

	auto* fjh1 = createBasicEntity(flowerJellyHat.pos, flowerJellyHat.size, flowerJellyHat.rotation, flowerJellyHat.vel);
	AnimBlendGraph* fjh1_anim_controller = fjh1->addComponent<AnimBlendGraph>();
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 7, 6, 38, 8, -1);
	fjh1->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(flowerJellyHat.physicType, flowerJellyHat.col, flowerJellyHat.colMask);
	fjh1->addComponent<ContactDamage>();
	fjh1->addComponent<JellyHatBehavior>(fjh1);
	
	return fjh1;
}

Entity* EnemyGenerator::generateElfShark(Vector2D pos)
{
	Config elfShark{};
	elfShark.vel = Vector2D(0, 0);
	elfShark.size = Vector2D(240.0f, 200.0f);
	elfShark.friction = 0.2f;
	elfShark.physicType = DYNAMIC;
	elfShark.fixedRotation = true;
	elfShark.rotation = 0.0f;
	elfShark.col = ENEMY;
	elfShark.colMask = ENEMY_MASK;


	auto* elf = createBasicEntity(pos, elfShark.size, elfShark.rotation, elfShark.vel);
	//auto* elf1 = manager_->addEntity();
	//Transform* t = elf1->addComponent<Transform>(Vector2D(sdlutils().width() * 1.6f, sdlutils().height() * 0.3f), Vector2D(0, 0), 180.0f, 180.0f, 0.0f);
	//elf1->addComponent<BoxCollider>(KINEMATIC, ENEMY, ENEMY_MASK);
	elf->addComponent<BoxCollider>(elfShark.physicType, elfShark.col, elfShark.colMask, elfShark.isTrigger,
		elfShark.friction, elfShark.fixedRotation, elfShark.rotation, Vector2D(elfShark.size.getX() * 0.75, elfShark.size.getY() * 0.75), Vector2D(elfShark.pos.getX() * 1.33, elfShark.pos.getY() * 1.33));
	AnimBlendGraph* elf_anim_controller = elf->addComponent<AnimBlendGraph>();
	elf_anim_controller->addAnimation("idle", &sdlutils().images().at("elfshark_idle"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.6));
	elf_anim_controller->addAnimation("move", &sdlutils().images().at("elfshark_move"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.6));
	elf_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 48, 0, 0, 10, Vector2D(0.66, 0.6));
	elf_anim_controller->addAnimation("attack_end", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 24, 0, 11, 18, Vector2D(0.66, 0.6));
	//Proportion?
	elf_anim_controller->keepProportion("idle", Vector2D(elf->getComponent<Transform>()->getW(), elf->getComponent<Transform>()->getH()));
	elf_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	elf_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	elf_anim_controller->addTransition("idle", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "idle", "Attack", 0, true);
	elf_anim_controller->addTransition("move", "attack_ini", "Attack", 1, false);
	elf_anim_controller->addTransition("attack_end", "move", "Attack", 0, true);
	elf_anim_controller->addTransition("attack_ini", "move", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "idle", "Attack", 0, false);
	elf_anim_controller->addTransition("attack_ini", "attack_end", "Attack", 2, true);
	elf_anim_controller->setParamValue("Speed", 0);
	elf_anim_controller->setParamValue("Attack", 0);
	auto* trigger_elf = elf->addComponent<EnemyTrigger>(Vector2D(1000.0f, 600.0f));
	trigger_elf->addTriggerComponent<ElfSharkAttack>(elf);
	elf->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);

	return elf;
}

Entity* EnemyGenerator::generatePompeyWorm(Vector2D pos)
{
	Config pompeyWorm{};
	pompeyWorm.pos = Vector2D(700, sdlutils().height() * 2.0f - 200);
	pompeyWorm.vel = Vector2D(0, 0);
	pompeyWorm.size = Vector2D(100.0f, 60.0f);
	pompeyWorm.friction = 100;
	pompeyWorm.physicType = DYNAMIC;
	pompeyWorm.fixedRotation = true;
	pompeyWorm.rotation = 0.0f;
	pompeyWorm.col = ENEMY;
	pompeyWorm.colMask = ENEMY_MASK;
	


	auto* gusano = createBasicEntity(pompeyWorm.pos, pompeyWorm.size, pompeyWorm.rotation, pompeyWorm.vel);
	gusano->addComponent<BoxCollider>(pompeyWorm.physicType, pompeyWorm.col, pompeyWorm.colMask);
	AnimBlendGraph* gusano_anim_controller = gusano->addComponent<AnimBlendGraph>();
	gusano_anim_controller->addAnimation("idle", &sdlutils().images().at("pompey_worm_idle"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("move", &sdlutils().images().at("pompey_worm_move"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("attack", &sdlutils().images().at("pompey_worm_attack"), 2, 3, 6, 12, 0, 0, 4);
	//Proportion?
	gusano_anim_controller->keepProportion("idle", Vector2D(gusano->getComponent<Transform>()->getW(), gusano->getComponent<Transform>()->getH()));
	gusano_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	gusano_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	gusano_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	gusano_anim_controller->addTransition("move", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "move", "Attack", 0, true);
	gusano_anim_controller->setParamValue("Speed", 0);
	gusano_anim_controller->setParamValue("Attack", 0);
	auto* trigger_gusano = gusano->addComponent<EnemyTrigger>(Vector2D(800.0f, 500.0f));
	trigger_gusano->addTriggerComponent<PompeyWormAttack>(gusano);
	gusano->addComponent<Enemy_Health>(300, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
}

Entity* EnemyGenerator::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel)
{
	auto* e = entity_->getMngr()->addEntity(); //entity_->manager_->addEntity();
	e->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return e;
}

