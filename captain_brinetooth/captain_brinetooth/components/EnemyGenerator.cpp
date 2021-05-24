#include "EnemyGenerator.h"

void EnemyGenerator::init()
{
	phase = -1;
	reward_mod_ = 1.0;
}

void EnemyGenerator::update()
{
	if (phase >= 0) {
		switch (phase)
		{
		case 0:
			reward_mod_ = 0.75;
			break;
		case 1:
			reward_mod_ = 1.0;
			break;
		case 2:

			reward_mod_ = 1.25;
			break;
		default:
			reward_mod_ = 1.0;
			break;
		}
	}
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
	case 4:
		return generateThornFish(pos);
		break;
	default:
		break;
	}
}

Entity* EnemyGenerator::generateFishler(Vector2D pos)
{
	Config fishlerConfig{};
	fishlerConfig.vel = Vector2D();
	fishlerConfig.size = Vector2D(300, 200);
	fishlerConfig.friction = 0.0f;
	fishlerConfig.physicType = DYNAMIC;
	fishlerConfig.col = ENEMY;
	fishlerConfig.colMask = ENEMY_MASK;
	fishlerConfig.fixedRotation = true;
	fishlerConfig.rotation = 0.0f;
	Entity* fishler = entity_->getMngr()->addEntity();
	fishler->addComponent<Transform>(pos, fishlerConfig.vel, fishlerConfig.size.getX(), fishlerConfig.size.getY(), fishlerConfig.rotation);
	fishler->addComponent<BoxCollider>(fishlerConfig.physicType, fishlerConfig.col, fishlerConfig.colMask, false, fishlerConfig.friction, true,
		0.0, Vector2D(fishlerConfig.size.getX()*0.2f, fishlerConfig.size.getY()), Vector2D(), 10000);
	AnimBlendGraph* fishler_anim_controller = fishler->addComponent<AnimBlendGraph>();
	fishler_anim_controller->addAnimation("move", &sdlutils().images().at("fishler_move"), 2, 3, 4, 10, -1);
	fishler_anim_controller->addAnimation("iddle", &sdlutils().images().at("fishler_iddle"), 4, 3, 11, 5, 0);
	fishler_anim_controller->addAnimation("gun", &sdlutils().images().at("fishler_gun"), 3, 3, 8, 10, 0);
	fishler_anim_controller->addAnimation("spikes", &sdlutils().images().at("fishler_spikes"), 6, 3, 16, 5, 0);
	fishler_anim_controller->addAnimation("run_ready", &sdlutils().images().at("fishler_run"), 8, 6, 43, 5, 0, 1, 20);
	fishler_anim_controller->addAnimation("run", &sdlutils().images().at("fishler_run"), 8, 6, 43, 5, -1, 21, 34);
	fishler_anim_controller->addAnimation("death", &sdlutils().images().at("fishler_death"), 5, 7, 35, 12, 0);

	//Trancisiones
	fishler_anim_controller->addTransition("move", "gun", "fishler_action", 1, false);
	fishler_anim_controller->addTransition("move", "run_ready", "fishler_action", 2, false);
	fishler_anim_controller->addTransition("run_ready", "run", "fishler_action", 3, false);
	fishler_anim_controller->addTransition("move", "spikes", "fishler_action", 4, false);
	fishler_anim_controller->addTransition("gun", "move", "fishler_action", 0, false);
	fishler_anim_controller->addTransition("run", "move", "fishler_action", 0, false);
	fishler_anim_controller->addTransition("spikes", "move", "fishler_action", 0, false);

	//Phase change
	fishler_anim_controller->addTransition("move", "iddle", "fishler_action", 5, false);
	fishler_anim_controller->addTransition("gun", "iddle", "fishler_action", 5, false);
	fishler_anim_controller->addTransition("run", "iddle", "fishler_action", 5, false);
	fishler_anim_controller->addTransition("iddle", "spikes", "fishler_action", 4, false);

	//Death
	fishler_anim_controller->addTransition("iddle", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("move", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("gun", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("spikes", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("run_ready", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("run", "death", "Dead", 1, false);
	fishler_anim_controller->addTransition("death", "iddle", "Dead", 99, false);

	fishler_anim_controller->setParamValue("fishler_action", 0);
	fishler_anim_controller->setParamValue("Dead", 0);

	fishler->addComponent<Enemy_Health>(5000, 500, Vector2D(300, 20), build_sdlcolor(255, 0, 0, 255), 50);
	fishler->addComponent<FishlerController>();
	fishler->addComponent<ContactDamage>(fishler);

	return fishler;
}

void EnemyGenerator::setPhase(int fase) {
	phase = fase;
}

Entity* EnemyGenerator::generateFringeHead(Vector2D pos)
{
	Config fringeHead{};
	fringeHead.vel = Vector2D(0, 0);
	fringeHead.size = Vector2D(60.0f, 60.0f);
	fringeHead.friction = 0;
	fringeHead.physicType = DYNAMIC;
	fringeHead.fixedRotation = true;
	fringeHead.rotation = 0.0f;
	fringeHead.col = ENEMY;
	fringeHead.colMask = ENEMY_MASK;

	auto* enemy = createBasicEntity(pos, fringeHead.size, fringeHead.rotation, fringeHead.vel);
	BoxCollider* collider = enemy->addComponent<BoxCollider>(fringeHead.physicType, fringeHead.col, fringeHead.colMask, false, fringeHead.friction,
		fringeHead.fixedRotation, fringeHead.rotation, Vector2D(), Vector2D(), 10000);
	AnimBlendGraph* anim_controller = enemy->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("idle", &sdlutils().images().at("fringehead_idle"), 1, 12, 12, 24, -1);
	anim_controller->addAnimation("attack", &sdlutils().images().at("fringehead_atk"), 1, 13, 13, 24, 0, 0, 12, Vector2D(0.5, 0.7));
	anim_controller->addAnimation("death", &sdlutils().images().at("fringehead_death"), 2, 3, 6, 24, 0, 0, 4, Vector2D(0.5, 0.7));
	//Proportion?
	anim_controller->keepProportion("idle", Vector2D(enemy->getComponent<Transform>()->getW(), enemy->getComponent<Transform>()->getH()));
	anim_controller->addTransition("idle", "attack", "Shoot", 1, false);
	anim_controller->addTransition("attack", "idle", "Shoot", 0, true);
	anim_controller->addTransition("idle", "death", "Dead", 1, false);
	anim_controller->addTransition("attack", "death", "Dead", 1, false);
	anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	anim_controller->setParamValue("Dead", 0);
	anim_controller->setParamValue("Shoot", 0);
	enemy->addComponent<FringeHeadAtack>();
	enemy->addComponent<Enemy_Health>(125, 12 * reward_mod_, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 200), 50);
	enemy->addComponent<ContactDamage>(enemy);

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
	fjh1_anim_controller->addAnimation("idle", &sdlutils().images().at("Medusa"), 5, 5, 25, 8, -1, 0, 21);
	fjh1_anim_controller->addAnimation("death", &sdlutils().images().at("medusa_death"), 2, 3, 6, 24, 0, 0, 4);
	fjh1_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	fjh1_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	fjh1_anim_controller->setParamValue("Dead", 0);
	fjh1->addComponent<Enemy_Health>(150, 15 * reward_mod_, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	fjh1->addComponent<BoxCollider>(flowerJellyHat.physicType, flowerJellyHat.col, flowerJellyHat.colMask, false, 0.7f, true, 0.0, Vector2D(), Vector2D(), 10000);
	fjh1->addComponent<ContactDamage>(fjh1);

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
	elf->addComponent<BoxCollider>(elfShark.physicType, elfShark.col, elfShark.colMask, false,
		elfShark.friction, elfShark.fixedRotation, elfShark.rotation, Vector2D(elfShark.size.getX() * 0.5, elfShark.size.getY() * 0.4),
		Vector2D(elfShark.pos.getX() * 1.5, elfShark.pos.getY() * 0.8), 35000);
	AnimBlendGraph* elf_anim_controller = elf->addComponent<AnimBlendGraph>();
	elf_anim_controller->addAnimation("idle", &sdlutils().images().at("elfshark_idle"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.8));
	elf_anim_controller->addAnimation("move", &sdlutils().images().at("elfshark_move"), 1, 2, 2, 12, -1, 0, 1, Vector2D(0.66, 0.8));
	elf_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 48, 0, 0, 10, Vector2D(0.5, 0.8));
	elf_anim_controller->addAnimation("attack_end", &sdlutils().images().at("elfshark_attack"), 1, 19, 19, 24, 0, 11, 18, Vector2D(0.5, 0.8));
	elf_anim_controller->addAnimation("death", &sdlutils().images().at("elfshark_death"), 2, 3, 6, 24, 0, 0, 4, Vector2D(0.5, 0.8));
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
	elf_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("move", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("attack_ini", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("attack_end", "death", "Dead", 1, false);
	elf_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	elf_anim_controller->setParamValue("Dead", 0);
	elf_anim_controller->setParamValue("Speed", 0);
	elf_anim_controller->setParamValue("Attack", 0);
	auto* trigger_elf = elf->addComponent<EnemyTrigger>(Vector2D(900.0f, 400.0f));
	trigger_elf->addTriggerComponent<ElfSharkAttack>(elf);
	elf->addComponent<Enemy_Health>(225, 22 * reward_mod_, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	elf->addComponent<ContactDamage>(elf);

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



	auto* gusano = createBasicEntity(pos, pompeyWorm.size, pompeyWorm.rotation, pompeyWorm.vel);
	gusano->addComponent<BoxCollider>(pompeyWorm.physicType, pompeyWorm.col, pompeyWorm.colMask, pompeyWorm.isTrigger, pompeyWorm.friction,
		pompeyWorm.fixedRotation, pompeyWorm.rotation, Vector2D(), Vector2D(), 10000);
	AnimBlendGraph* gusano_anim_controller = gusano->addComponent<AnimBlendGraph>();
	gusano_anim_controller->addAnimation("idle", &sdlutils().images().at("pompey_worm_idle"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("move", &sdlutils().images().at("pompey_worm_move"), 1, 2, 2, 12, -1);
	gusano_anim_controller->addAnimation("attack", &sdlutils().images().at("pompey_worm_attack"), 2, 3, 6, 12, 0, 0, 4);
	gusano_anim_controller->addAnimation("death", &sdlutils().images().at("pompey_worm_death"), 2, 3, 6, 24, 0, 0, 4);
	//Proportion?
	gusano_anim_controller->keepProportion("idle", Vector2D(gusano->getComponent<Transform>()->getW(), gusano->getComponent<Transform>()->getH()));
	gusano_anim_controller->addTransition("idle", "move", "Speed", 1, false);
	gusano_anim_controller->addTransition("move", "idle", "Speed", 0, false);
	gusano_anim_controller->addTransition("idle", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "idle", "Attack", 0, true);
	gusano_anim_controller->addTransition("move", "attack", "Attack", 1, false);
	gusano_anim_controller->addTransition("attack", "move", "Attack", 0, true);
	gusano_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("move", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("attack", "death", "Dead", 1, false);
	gusano_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	gusano_anim_controller->setParamValue("Dead", 0);
	gusano_anim_controller->setParamValue("Speed", 0);
	gusano_anim_controller->setParamValue("Attack", 0);
	auto* trigger_gusano = gusano->addComponent<EnemyTrigger>(Vector2D(800.0f, 500.0f));
	trigger_gusano->addTriggerComponent<PompeyWormAttack>(gusano);
	gusano->addComponent<Enemy_Health>(175, 18 * reward_mod_, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	gusano->addComponent<ContactDamage>(gusano);

	return gusano;
}

Entity* EnemyGenerator::generateThornFish(Vector2D pos)
{
	Config thornfish{};
	thornfish.pos = Vector2D(300, sdlutils().height() * 1.7f);
	thornfish.vel = Vector2D(0, 0);
	thornfish.size = Vector2D(180.0f, 120.0f);
	thornfish.friction = 0.7;
	thornfish.physicType = DYNAMIC;
	thornfish.fixedRotation = true;
	thornfish.rotation = 0.0f;
	thornfish.col = ENEMY;
	thornfish.colMask = ENEMY_MASK;

	auto* thorn = createBasicEntity(pos, thornfish.size, thornfish.rotation, thornfish.vel);
	thorn->addComponent<BoxCollider>(thornfish.physicType, thornfish.col, thornfish.colMask, false,
		thornfish.friction, thornfish.fixedRotation, thornfish.rotation, Vector2D(thornfish.size.getX() * 0.3, thornfish.size.getY() * 0.7),
		Vector2D(), 20000);
	AnimBlendGraph* thorn_anim_controller = thorn->addComponent<AnimBlendGraph>();
	thorn_anim_controller->addAnimation("idle", &sdlutils().images().at("thornfish_idle"), 1, 12, 8, 12, -1, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("attack_ini", &sdlutils().images().at("thornfish_attack"), 1, 8, 8, 12, 0, 0, 4, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("attack_end", &sdlutils().images().at("thornfish_attack"), 1, 8, 8, 24, 0, 5, 7, Vector2D(0.5, 0.6));
	thorn_anim_controller->addAnimation("death", &sdlutils().images().at("thornfish_death"), 2, 3, 6, 24, 0, 0, 4, Vector2D(0.5, 0.6));
	//Proportion?
	thorn_anim_controller->keepProportion("idle", Vector2D(thorn->getComponent<Transform>()->getW(), thorn->getComponent<Transform>()->getH()));
	thorn_anim_controller->addTransition("idle", "attack_ini", "Attack", 1, false);
	thorn_anim_controller->addTransition("attack_end", "idle", "Attack", 0, true);
	thorn_anim_controller->addTransition("attack_ini", "idle", "Attack", 0, false);
	thorn_anim_controller->addTransition("attack_ini", "attack_end", "Attack", 2, true);
	thorn_anim_controller->addTransition("idle", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("attack_ini", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("attack_end", "death", "Dead", 1, false);
	thorn_anim_controller->addTransition("death", "idle", "Dead", 99, false);	//Necesario crear una transicion para crear un animstate
	thorn_anim_controller->setParamValue("Dead", 0);
	thorn_anim_controller->setParamValue("Attack", 0);
	thorn->addComponent<ThornFishAttack>();
	thorn->addComponent<Enemy_Health>(150, 15*reward_mod_, Vector2D(50, 5), build_sdlcolor(255, 0, 0, 255), 50);
	thorn->addComponent<ContactDamage>(thorn);

	return thorn;
}

Entity* EnemyGenerator::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel)
{
	auto* e = entity_->getMngr()->addEntity(); //entity_->manager_->addEntity();
	e->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return e;
}

