#include "SkillTree.h"

SkillTree::SkillTree()
{
}

SkillTree::~SkillTree()
{
}

void SkillTree::init(){
	setSkillLimitPoints(Spines, 1500);
	setSkillLimitPoints(SpeedAttack, 1500);
	setSkillLimitPoints(StealLife, 1500);

	speedModifier = 1;
	counterattackSpines = 100;
	//El guardado tiene que venir de la siguiente manera:
	//p int
	//skills true false true false false true
}

void SkillTree::update()
{
}

void SkillTree::render()
{
}

bool SkillTree::hasSkill(Skill type){
	return playerSkills[(int)type].first;
}

bool SkillTree::hasSkill(int type){
	return playerSkills[type].first;
}

int SkillTree::getSkillLimitPoints(Skill type)
{
	return playerSkills[(int)type].second;
}

bool SkillTree::setSkill(Skill type, bool state, int points){
	if (state){
		if (points >= playerSkills[(int)type].second) {
			playerSkills[(int)type].first = state;
			return true;
		}
		else return false;
	}
	else playerSkills[(int)type].first = state;

	return true;
}

bool SkillTree::setSkillLimitPoints(Skill type, int p){
	if ((int)type > 3) {
		playerSkills[(int)type].second = p;
		return true;
	}

	return false;
}

void SkillTree::initSkillsFromMatch(std::array<bool, 6> skillsFromMatch){
	for (size_t i = 0; i < skillsFromMatch.size(); i++){
		playerSkills[i].first = skillsFromMatch[i];
	}
}

int& SkillTree::getSpeedModifier(){
	return speedModifier;
}

int SkillTree::getMaxSkills(){
	return playerSkills.size();
}

int SkillTree::getCounterAttack(){
	return counterattackSpines;
}
