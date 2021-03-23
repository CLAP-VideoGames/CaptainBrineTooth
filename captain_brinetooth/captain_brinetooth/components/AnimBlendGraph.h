#pragma once

#include "../ecs/Component.h"
#include "../sdlutils/Texture.h"
#include "../ecs/Entity.h"
#include "Animation.h"

//Estructura de datos para guardar las aristas del grafo
struct Parameter{
	std::string name_;
	int value_;

	inline bool operator==(Parameter *a) {
		if (a->name_ == name_ && a->value_ == value_)
			return true;
		else
			return false;
	}
	
	inline bool operator!=(Parameter *a) {
		if (a->name_ != name_ || a->value_ != value_)
			return true;
		else
			return false;
	}
};

typedef struct{
	Animation* dest_;
	Parameter condition_;   //Condicion (parameter) de cambio
	bool onComplete_;   //Condicion de terminar la animacion antes de cambiar
}Edge;

typedef struct {
	Animation* anim_;   //Animacion fuente
	std::vector<Edge> edges_;    //Conexiones con demas componentes del grafo
}AnimState;

class AnimBlendGraph : public Component
{
public:
	AnimBlendGraph() {};
	virtual	~AnimBlendGraph() {};
	void init() override {
		tr_ = entity_->getComponent<Transform>();
		assert(tr_ != nullptr);

		waitOnComplete = false;
		currentAnim_ = nullptr;
		defaultAnim_ = nullptr;
	}

	void render() override {
		updateAnim();
		if(currentAnim_ != nullptr)
			currentAnim_->anim_->render();
	}

	void updateAnim() {
		if (currentAnim_ == nullptr) {
			if (animStates_.size() == 0 && entityAnims_.size() > 0)	//Si solo hay vertices en el grafo, coge el primer vertice
				currentAnim_ = new AnimState({ entityAnims_[0] });
			else
				currentAnim_ = defaultAnim_;
		}
		else {
			if (currentAnim_->anim_->getState() != Stop && nextAnim_ != nullptr) {  //No hacer nada si la animacion se para
				if (waitOnComplete) {   //Esperar a que la animacion se haya terminado
					if (currentAnim_->anim_->getState() == Complete) {
						currentAnim_ = nextAnim_;
						reset();    //resetea nextframe y waitOnComplete
					}
				}
				else {
					currentAnim_ = nextAnim_;
					reset();
				}
			}
		}
	}

	void checkAnimState(Parameter* p) { 
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

	template <typename ...Ts>
	void addAnimation(Ts &&... args) {
		Animation* anim_ = new Animation(std::forward<Ts>(args)...);
		anim_->setEntity(this->getEntity());
		anim_->init();
		//Guardado en el vector
		entityAnims_.emplace_back(anim_);
	}

	void addTransition(std::string animsrc_, std::string animdest_, std::string nameP, int valueP, bool onComplete) {
		Parameter* param = new Parameter({nameP, valueP});
		Animation *src = nullptr, *dest  = nullptr;
		int i = 0;
		//Asignacion de animaciones
		while (i< entityAnims_.size() && (src == nullptr || dest == nullptr)){
			if (entityAnims_[i]->getID() == animsrc_ && src == nullptr)
				src = entityAnims_[i];      //Animacion fuente
			else if (entityAnims_[i]->getID() == animdest_ && dest == nullptr)
				dest = entityAnims_[i];     //Animacion destino

			i++;
		}
		//Comprobar si existe un parametro ya existente, si no, lo crea y lo inicializa a -1
		i = searchParamValue(param->name_);
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
	}

	void setParamValue(std::string paramName, int value) {
		int i = searchParamValue(paramName);
		if (i != -1) {
			params_[i]->value_ = value;
			//Comprobar si cambia la animacion
			checkAnimState(params_[i]);
		}
	}

	/// <summary>
	/// Busca si el nombre del parámtro existe en el vector de parámetros
	/// </summary>
	/// <param name="paramName"></param>
	/// <returns></returns>
	int searchParamValue(std::string paramName)
	{
		int i = 0;
		while (i < params_.size() && params_[i]->name_ != paramName) i++; //Busqueda del parametro por su nombre
		return (i < params_.size() ? i : -1);
	}

	void reset() {
		if (currentAnim_ != nullptr) { 
			currentAnim_->anim_->reset(); 
			currentAnim_->anim_->play();
		}
		nextAnim_ = nullptr;
		waitOnComplete = false;
	}

	void play() { if(currentAnim_ != nullptr)currentAnim_->anim_->play(); };
	void stop() { if (currentAnim_ != nullptr)currentAnim_->anim_->stop(); };
	
	friend class Animation;

protected:
	Transform* tr_;
	std::vector<Parameter*> params_; //Parametros de transicion entre componentes del grafo
	std::vector<Animation*> entityAnims_;    //Componentes (Animation) del grafo
	std::vector<AnimState*> animStates_;
	AnimState *currentAnim_, *nextAnim_, *defaultAnim_;
	bool waitOnComplete;
};