#include "../ecs/Component.h"
#include "../components/Transform.h"
#include "../ecs/Manager.h"

#pragma once
class ExplanationMessage : public Component
{
public:
	ExplanationMessage();
	~ExplanationMessage();

	void init() override;
	void update() override;
	void render() override;

private:
	Transform* tr;
};