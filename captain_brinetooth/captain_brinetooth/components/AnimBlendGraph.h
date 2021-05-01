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
	/// <summary>
	/// Grafo de animaciones vacio
	/// </summary>
	AnimBlendGraph(){};
	virtual	~AnimBlendGraph();

	void init() override;
	void update() override;
	void render() override;
	void updateAnim();

	template<typename ...Ts>
	inline void addAnimation(Ts && ...args)
	{
		Animation* anim_ = new Animation(std::forward<Ts>(args)...);
		anim_->setEntity(this->getEntity());
		anim_->init();
		//Guardado en el vector
		entityAnims_.emplace_back(anim_);
	}
	/// <summary>
	/// Conexion entre 2 animaciones ya definidas segun el parametro que se le de
	/// </summary>
	void addTransition(std::string animsrc_, std::string animdest_, std::string nameP, int valueP, bool onComplete);
	
	void setParamValue(std::string paramName, int value);

	void checkAnimState(Parameter* p);
	/// <summary>
	/// Busca si el nombre del parametro existe en el vector de parámetros
	/// </summary>
	/// <param name="paramName"></param>
	/// <returns></returns>
	const int& getParamIndex(std::string paramName);
	/// <summary>
	/// Mantiene las proporciones entre animaciones que no comparten el mismo tamaño
	/// Se necesita el nombre de la animacion por defecto y el tamaño que se quiere representar
	/// </summary>
	void keepProportion(std::string animsrc, Vector2D destSize);
	void scaleAnimation();
	
	/// <summary>
	/// Reinicia la animacion
	/// </summary>
	void reset();
	void play();
	void stop();
	void flipX(bool state);
	void setAlpha(Uint8 alpha);
	bool isComplete();
	const bool& isFlipX();
	const int& getParamValue(std::string paramName);
	const Animation* getCurrentAnimation();
	
	friend class Animation;

protected:
	Transform* tr_;
	std::vector<Parameter*> params_; //Parametros de transicion entre componentes del grafo
	std::vector<Animation*> entityAnims_;    //Componentes (Animation) del grafo
	std::vector<AnimState*> animStates_;
	AnimState *currentAnim_, *nextAnim_, *defaultAnim_;
	bool waitOnComplete;
	bool flip_horizontal_;
	bool keepProportion_;
	Vector2D destSize_;
	Vector2D proportion_;
};
