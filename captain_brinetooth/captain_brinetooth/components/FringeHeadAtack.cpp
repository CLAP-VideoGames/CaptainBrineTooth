#include "FringeHeadAtack.h"
#include "../ecs/Manager.h"
#include "../game/CollisionLayers.h"
#include "AnimBlendGraph.h"
#include "ShootDetect.h"
using namespace ColLayers;

FringeHeadAtack::FringeHeadAtack()
{
	//Damos un tama�o al trigger de la zona de ataque 
	triggersize.setX(700.0);
	triggersize.setY(300.0);

}
void FringeHeadAtack::init()
{
	trigger = entity_->getMngr()->addEntity(); //A�ADIMOS EL TRIGGER DEL ENEMIGO 
	tr = entity_->getComponent<Transform>();  //Obtenemos una referencia al transform del enemigo 
	assert(tr != nullptr);

	Vector2D triggerpos = Vector2D(tr->getPos().getX() + tr->getW() * 0.5f, tr->getPos().getY() + tr->getH() *0.5f);
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize.getX(), triggersize.getY(),  0.0f);
	trigger->addComponent<BoxCollider>(KINEMATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK,true);
	trigger->addComponent<ShootDetect>(tr,entity_);
	//AnimBlendGraph* anim_controller = trigger->addComponent<AnimBlendGraph>();
}
