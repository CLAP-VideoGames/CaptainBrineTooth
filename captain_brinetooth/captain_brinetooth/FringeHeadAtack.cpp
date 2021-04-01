#include "FringeHeadAtack.h"
#include "components/Transform.h"
#include "components/BoxCollider.h"
#include "ecs/Manager.h"
#include "CollisionLayers.h"
#include "components/AnimBlendGraph.h"
using namespace ColLayers;

FringeHeadAtack:: FringeHeadAtack()
{
	
	

	//Damos un tamaño al trigger de la zona de ataque 
	triggersize.setX(470.0);
	triggersize.setY(20.0);

}
void FringeHeadAtack::init()
{
	trigger = entity_->getMngr()->addEntity(); //AÑADIMOS EL TRIGGER DEL ENEMIGO 
	tr = entity_->getComponent<Transform>();  //Obtenemos una referencia al transform del enemigo 
	assert(tr != nullptr);

	Vector2D triggerpos = tr->getPos();
	triggerpos.setY(triggerpos.getY() + 25.0);
	trigger->addComponent<Transform>(triggerpos, Vector2D(0, 0), triggersize.getX(), triggersize.getY(),  0.0f);
	trigger->addComponent<BoxCollider>(STATIC, ENEMY_ATTACK, ENEMY_ATTACK_MASK,true);
	AnimBlendGraph* anim_controller = trigger->addComponent<AnimBlendGraph>();
	anim_controller->addAnimation("iddle", &sdlutils().images().at("Square"), 1, 1, 1, 1, 1);
}
