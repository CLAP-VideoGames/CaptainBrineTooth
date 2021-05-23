#include "SkillTree.h"

SkillTree::SkillTree()
{
}

SkillTree::~SkillTree()
{
}

void SkillTree::init(){
	setSkillLimitPoints(ExtraDamage, 1600);
	setSkillLimitPoints(SpeedAttack, 1400);
	setSkillLimitPoints(StealLife, 1500);
	setSkillLimitPoints(ExtraHeal, 1200);
	setSkillLimitPoints(Spines, 2000);

	speedModifier = 1;
	attackModifier = 1;
	counterattackSpines = 100;

	invent_ = entity_->getComponent<Inventory>();

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
			if (invent_) invent_->substractBaits(playerSkills[(int)type].second);
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

int* SkillTree::getSpeedModifier(){
	return &speedModifier;
}

int SkillTree::getMaxSkills(){
	return playerSkills.size();
}

float SkillTree::getCounterAttackPercentage(){
	return counterattackSpines;
}

int SkillTree::getAttackModifier(){
	return attackModifier;
}

void SkillTree::setAttackModifier(int attack){
	attackModifier = attack;
} 

void SkillTree::setCounterAttackPercentage(float points)
{
	if (points < 0) counterattackSpines = 0;
	else if (points > 1) counterattackSpines = 1;
	else counterattackSpines = points;
}

void SkillTree::setSpeedModifier(int n){
	speedModifier = n;
}
