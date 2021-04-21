#pragma once
#include "..//ecs/Component.h"
#include "Transform.h"
#include "..//ecs/Entity.h"
#include "..//game/App.h"

class App;

class CameraFollow : public Component 
{
public: 
	CameraFollow(Transform* p, const Vector2D& offS = Vector2D(0.0f, 0.f), const float& percentage_ = 0.0f,
		int* Wlimit_ = &(App::camera.w), int* Hlimit_ = &(App::camera.h), const float zoom_ = 2.0f, bool limitless_H = false, bool limitless_V = false) : lastDiff(), differenceX(), differenceY() {
		entityT = p;
		percentage = percentage_;
		offset_ = offS;
		limitless_Horizontal = limitless_H;
		limitless_Vertical = limitless_V;

		wLimit = Wlimit_;
		hLimit = Hlimit_;

		zoom = zoom_;
	}

	void init() override;

	void update() override;
	void actPos();
	void actPosAdvanced();

protected:
	Transform* entityT;
	std::vector<Entity*> entities_;

	int differenceX, differenceY;
	float percentage;
	Vector2D lastDiff, offset_;

	//Booleanos de seguimiento ilimitado
	int *wLimit, *hLimit;
	bool limitless_Horizontal, limitless_Vertical;

	float zoom = 0;
};