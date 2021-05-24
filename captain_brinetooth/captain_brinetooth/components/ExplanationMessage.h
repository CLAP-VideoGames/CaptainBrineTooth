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

	void push_backTexture(Texture* t);

	void setCurrentTexture(int i);

	int getSizeTextures();



private:
	Transform* tr;
	Vector2D size = {};
	std::vector<Texture*> textures;
	Texture* currentTexture;

	SDL_Point mouseP;
	bool render_;
};