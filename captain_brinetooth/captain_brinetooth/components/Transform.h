// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include "../ecs/Component.h"
#include "../utils/Vector2D.h"

class Transform: public Component {
public:
	Transform();

	Transform(Vector2D pos, Vector2D vel, float width, float height,
		float rotation);

	virtual ~Transform();

	Vector2D& getPos();

	Vector2D& getVel();

	float getH() const;

	void setH(float height);

	float getW() const;

	void setW(float width);

	float getRot() const;

	void setRot(float rot);

	void update() override;

private:
	Vector2D pos_;
	Vector2D vel_;
	float width_;
	float height_;
	float rotation_;
};

