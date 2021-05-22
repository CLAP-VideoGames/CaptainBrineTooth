#include "../ecs/Component.h"
#include <array>
#pragma once

//DLC Anzuelos, Agua Potable, Escamas de Arapaima, Dientes de Piraña, Armas de Salmuere, Infección de anémonas
enum Skill { ExtraLives, ExtraHeal, Spines, DoubleDamage, SpeedAttack, StealLife};

class SkillTree : public Component {
public:
	SkillTree();
	~SkillTree();

	void init() override;
	void update() override;
	void render() override;

	/// <summary>
	/// Pregunta si la habilidad type está activa o no
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool hasSkill(Skill type);
	
	/// <summary>
	/// Pregunta si la habilidad type está activa o no
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	bool hasSkill(int type);

	/// <summary>
	/// Devuelve el número límite de una habilidad para poder ser activada
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	int getSkillLimitPoints(Skill type);

	/// <summary>
	/// Devuelve el número máximo de habilidades
	/// </summary>
	/// <returns></returns>
	int getMaxSkills();

	/// <summary>
	/// Asigna a la entidad la habilidad de tipo Skill al estado deseado
	/// </summary>
	/// <param name="type"> habilidad</param>
	/// <param name="state"> estado de la habilidad (activo/desactivo)</param>
	/// <param name="points"> puntuación actual del jugador</param>
	/// <returns> devuelve false en caso de que la puntuación no sea suficiente para activar la habilidad. 
	/// De lo contrario devuelve true</returns>
	bool setSkill(Skill type, bool state, int points);

	/// <summary>
	/// Asigna a una habilidad un limite de puntos para activar dicha skill
	/// </summary>
	/// <param name="type"> tipo</param>
	/// <param name="p"> cantidad de puntos</param>
	/// <returns>devuelve false en caso de que el tipo de la habilidad no sea de la rama ofensiva</returns>
	bool setSkillLimitPoints(Skill type, int p);

	/// <summary>
	/// Inicializa los valores de las habilidades de la partida guardada
	/// </summary>
	/// <param name="skillsFromMatch"></param>
	void initSkillsFromMatch(std::array<bool, 6> skillsFromMatch);

private:
	std::array<std::pair<bool, int>, 6> playerSkills {};
};

