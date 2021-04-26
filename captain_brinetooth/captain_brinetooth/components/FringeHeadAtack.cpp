#include "FringeHeadAtack.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"
#include "ShootDetect.h"
using namespace ColLayers;

FringeHeadAtack::FringeHeadAtack()
{
	//Damos un tamaño al trigger de la zona de ataque 
	triggersize.setX(800.0);
	triggersize.setY(1000.0);

}
void FringeHeadAtack::init()
{
	trigger = entity_->getMngr()->addEntity(); //AÑADIMOS EL TRIGGER DEL ENEMIGO 
	tr = entity_->getComponent<Transform>();  //Obtenemos una referencia al transform del enemigo 
	assert(tr != nullptr);

	Vector2D triggerpos = tr->getPos();
	triggerpos.setY(triggerpos.getY() + 25.0);
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize.getX(), triggersize.getY(),  0.0f);
	trigger->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK,true);
	trigger->addComponent<ShootDetect>(tr,entity_);
	//AnimBlendGraph* anim_controller = trigger->addComponent<AnimBlendGraph>();
}
