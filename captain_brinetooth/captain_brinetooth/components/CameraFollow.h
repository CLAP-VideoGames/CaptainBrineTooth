#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"
#include "..//game/App.h"

class App;

class CameraFollow : public Component 
{
public: 
	CameraFollow(const Vector2D& offS = Vector2D(0.0f, 0.0f), const float& percentage = 0.0f, bool limitless_H = false, bool limitless_V = false, Vector2D camMaxLimits = Vector2D(0.0f,0.0f), Vector2D camMinLimits = Vector2D(0.0f, 0.0f))
		:limitless_Horizontal_(limitless_H), limitless_Vertical_(limitless_V), camMinLimits_(camMinLimits),camMaxLimits_(camMaxLimits), percentage_(percentage),offset_(offS){}

	void init() override;
	void update() override;

	void actPosAdvanced();
	void checkCameraFollowLimits();

protected:
	Transform* target_;
	std::vector<Entity*> entities_;

	int differenceX, differenceY;
	float percentage_;
	Vector2D lastDiff, offset_, camMinLimits_,camMaxLimits_;
	//Booleanos de seguimiento ilimitado
	bool limitless_Horizontal_, limitless_Vertical_;
	//Cooldowns
	float cd_up_ = 25;
	float cd_down_ = 25;
	float up_time_, down_time_;


};