#pragma once
#include "../ecs/Component.h"
#include "../ecs/Manager.h"
#include <functional>
#include "../sdlutils/SDLUtils.h"
#include "Image.h"

const int NUM_TEXTURES = 2;

class Slider : public Component {
public:
	/// <summary>
	/// Crea un slider dada una posición y tamaño.
	/// </summary>
	/// <param name="pos">posición en pixeles</param>
	/// <param name="sizes_">tamaño en pixeles del slider y su barra </param>
	/// <param name="texts">texturas del slider y su barra</param>
	/// <param name="callback">tarea que realizar al mover el slider que recibe un valor de 0 a 1 y la entidad</param>
	Slider(const Vector2D& pos, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void (*callback)(float, Entity*), Texture* t);
	~Slider();

	void init() override;
	void update() override;
	void render() override;
	void desactivateSlider();
	void setSlider(float& value);

private:
	Vector2D pos;
	Manager* mngr;
	Entity* background, *slide;
	std::pair<Vector2D, Vector2D> sizes;
	std::function<void(float, Entity*)> callback_;
	SDL_Rect backgroundSlideRct = {};
	SDL_Rect* Sliderdest = {};

	Texture* textures[NUM_TEXTURES] = {};
	Texture* textureText = nullptr;

	Image* sliderImage;

	bool selected = true;
};
