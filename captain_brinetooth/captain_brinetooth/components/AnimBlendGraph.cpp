#include "AnimBlendGraph.h"

#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

AnimBlendGraph::~AnimBlendGraph(){
	//Limpiamos todas las animaciones
	for (Animation* anim : entityAnims_){
		delete anim;
	}

	for (AnimState* animS : animStates_){
		delete animS;
	}

	
	for (Parameter* param : params_){
		delete param;
	}
	

	entityAnims_.clear();
	animStates_.clear();
	params_.clear();


	currentAnim_ = nullptr;

	defaultAnim_ = nullptr;
	nextAnim_ = nullptr;
	tr_ = nullptr;
}

void AnimBlendGraph::init()
{
	tr_ = entity_->getComponent<Transform>();
	assert(tr_ != nullptr);

	waitOnComplete = false;
	currentAnim_ = nullptr;
	defaultAnim_ = nullptr;
	flip_horizontal_ = false;
	keepProportion_ = false;
}

void AnimBlendGraph::update(){
	updateAnim();
}

void AnimBlendGraph::render(){
	if (currentAnim_ != nullptr) {
		currentAnim_->anim_->flipX(flip_horizontal_);
		currentAnim_->anim_->render();
	}
}

void AnimBlendGraph::updateAnim(){

	if (currentAnim_ == nullptr) {
		if (animStates_.size() == 0 && entityAnims_.size() > 0) { //Si solo hay vertices en el grafo, coge el primer vertice
			animStates_.push_back(new AnimState({ entityAnims_[0] }));
			currentAnim_ = animStates_.back();
		}

		else
			currentAnim_ = defaultAnim_;
		scaleAnimation();
	}
	else {
		if (currentAnim_->anim_->getState() != Stop && nextAnim_ != nullptr) {  //No hacer nada si la animacion se para
			if (waitOnComplete) {   //Esperar a que la animacion se haya terminado
				if (currentAnim_->anim_->getState() == Complete) {
					currentAnim_ = nextAnim_;
					scaleAnimation();
					reset();    //resetea nextframe y waitOnComplete
				}
			}
			else {
				currentAnim_ = nextAnim_;
				scaleAnimation();
				reset();
			}
		}
	}
}

void AnimBlendGraph::checkAnimState(Parameter* p)
{
	int i = 0;
	//Busca que el parametro sea una condicion de transicion de la animacion actual
	//while (i < currentAnim_->edges_.size() && (currentAnim_->edges_[i].condition_.name_ != p->name_ || currentAnim_->edges_[i].condition_.value_ == p->value_))i++;
	if (currentAnim_ != nullptr) {
		while (i < currentAnim_->edges_.size() && (currentAnim_->edges_[i].condition_ != p))i++;
		if (i < currentAnim_->edges_.size()) {    //Si se ha encontrado que el parametro es una condicion de transicion de la animacion actual
			int j = 0;
			//Busca el AnimState con animacion fuente igual a la animacion destino de la transicion y la deja en cola para actualizarla
			while (j < animStates_.size() && animStates_[j]->anim_ != currentAnim_->edges_[i].dest_)j++;
			if (j < animStates_.size()) {
				waitOnComplete = currentAnim_->edges_[i].onComplete_;   //Si la transicion necesita completarse antes de cambiar a la siguiente
				nextAnim_ = animStates_[j];
			}
		}
	}
}

void AnimBlendGraph::addTransition(std::string animsrc_, std::string animdest_, std::string nameP, int valueP, bool onComplete)
{
	Parameter* param = new Parameter({ nameP, valueP });
	Animation* src = nullptr, * dest = nullptr;
	int i = 0;
	//Asignacion de animaciones
	while (i < entityAnims_.size() && (src == nullptr || dest == nullptr)) {
		if (entityAnims_[i]->getID() == animsrc_ && src == nullptr)
			src = entityAnims_[i];      //Animacion fuente
		else if (entityAnims_[i]->getID() == animdest_ && dest == nullptr)
			dest = entityAnims_[i];     //Animacion destino

		i++;
	}
	//Comprobar si existe un parametro ya existente, si no, lo crea y lo inicializa a -1
	i = getParamIndex(param->name_);
	if (i == -1) {
		Parameter* newP = new Parameter({ param->name_, -1 });
		params_.push_back(newP);
	}

	//Comprobar si existe un AnimState con la animacion fuente
	i = 0;
	while (i < animStates_.size() && animStates_[i]->anim_ != src)i++;
	if (i < animStates_.size()) //Si ya existe añade una arista mas al vector
		animStates_[i]->edges_.push_back(Edge({ dest,*param,onComplete }));
	else {   //Crea un nuevo AnimState
		AnimState* a = new AnimState({ src });
		animStates_.push_back(a);
		animStates_[animStates_.size() - 1]->edges_.push_back(Edge({ dest,*param,onComplete }));
		if (animStates_.size() == 1)
			defaultAnim_ = animStates_[0];
	}

	//Param al parecer es solo una variable auxiliar, ya que solo la crea para verificar si ya existe ese parametro y ademas
	//Cuando lo pasa a un push back, pasa el dato y no la referencia. Asi que se puede borrar.
	delete param;
}

void AnimBlendGraph::setParamValue(std::string paramName, int value)
{
	int i = getParamIndex(paramName);
	if (i != -1) {
		params_[i]->value_ = value;
		//Comprobar si cambia la animacion
		checkAnimState(params_[i]);
	}
}

const int& AnimBlendGraph::getParamIndex(std::string paramName)
{
	int i = 0;
	while (i < params_.size() && params_[i]->name_ != paramName) i++; //Busqueda del parametro por su nombre
	return (i < params_.size() ? i : -1);
}

void AnimBlendGraph::keepProportion(std::string animsrc, Vector2D destSize)
{
	int i = 0;
	//Busca la animacion fuente
	while (i < entityAnims_.size() && entityAnims_[i]->getID() != animsrc)i++;
	if (i < entityAnims_.size()) {	//Establece la proporcion dividiendo el tamaño fuente entre el destino
		Vector2D srcSize = entityAnims_[i]->srcSize();
		proportion_ = Vector2D(srcSize.getX() / destSize.getX(), srcSize.getY() / destSize.getY());
	}
	else { proportion_ = Vector2D(1.0, 1.0); }	//Proporcion por defecto

	keepProportion_ = true;	//Manten la proporcion entre transiciones
}

void AnimBlendGraph::scaleAnimation() {
	Vector2D srcSize = currentAnim_->anim_->srcSize();
	if(keepProportion_)
		currentAnim_->anim_->destSize(srcSize.getX() / proportion_.getX(), srcSize.getY() / proportion_.getY());
}

void AnimBlendGraph::reset()
{
	if (currentAnim_ != nullptr) {
		currentAnim_->anim_->reset();
		currentAnim_->anim_->play();
	}
	nextAnim_ = nullptr;
	waitOnComplete = false;
}

void AnimBlendGraph::play()
{
	if (currentAnim_ != nullptr)currentAnim_->anim_->play();
}

void AnimBlendGraph::stop()
{
	if (currentAnim_ != nullptr)currentAnim_->anim_->stop();
}

void AnimBlendGraph::flipX(bool state)
{
	flip_horizontal_ = state;
}

bool AnimBlendGraph::isComplete()
{
	if (currentAnim_ != nullptr) {
		if (currentAnim_->anim_->getState() == Complete)
			return true;
		else 
			return false;
	}
	else { return false; }
}

const bool& AnimBlendGraph::isFlipX()
{
	return flip_horizontal_;
}

void AnimBlendGraph::setAlpha(Uint8 alpha)
{
	if (currentAnim_ != nullptr)
	currentAnim_->anim_->setAlpha(alpha);
}

const int& AnimBlendGraph::getParamValue(std::string paramName)
{
	int val = -1;
	if (getParamIndex(paramName) >= 0) {
		val = params_[getParamIndex(paramName)]->value_;
	}
	return val;
}

const Animation* AnimBlendGraph::getCurrentAnimation() {
	if (currentAnim_ != nullptr)
		return currentAnim_->anim_;
	else
		return entityAnims_[0];
}

