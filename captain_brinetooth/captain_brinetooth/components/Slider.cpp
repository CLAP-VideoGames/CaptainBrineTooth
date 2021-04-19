#include "Slider.h"

#include <iostream>

Slider::Slider(const Vector2D& pos_, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void(*callback)(float, Entity*), Texture* text_){
	pos = pos_;
	sizes = sizes_;
	for (int i = 0; i < NUM_TEXTURES; i++) textures[i] = texts[i];

	callback_ = callback;

	textureText = text_;
}

Slider::~Slider(){
}

void Slider::init(){
	mngr = entity_->getMngr();

	background = mngr->addEntity();
	backgroundSlideRct = build_sdlrect(pos, sizes.first.getX(), sizes.first.getY());
	background->addComponent<Image>(textures[0], backgroundSlideRct, "fondo");


	slide = mngr->addEntity();
	Vector2D posSlide = Vector2D(pos.getX()* 1.1f, pos.getY() - (sizes.second.getY()*0.5f));
	SDL_Rect slideRect = build_sdlrect(posSlide, sizes.second.getX(), sizes.second.getY());
	sliderImage = slide->addComponent<Image>(textures[1], slideRect, "fondo");

	Sliderdest = (sliderImage->getDestRect());
}

void Slider::update(){
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second };
	if (SDL_PointInRect(&mouseP, Sliderdest) == SDL_TRUE) {
		//Si ha habido un evento de raton o si se sigue presionando
		if (ih().mouseButtonEvent() || ih().getMouseButtonHeld()){
			//Si el boton fue el izquierdo o si el izquierdo se sigue presionando
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().getLeftMouseButtonPressed()){
				//std::cout << "pressed" << std::endl;
				int newPos = mouseP.x - (Sliderdest->w/2);

				if (newPos >= backgroundSlideRct.x - (Sliderdest->w / 2) && newPos <= backgroundSlideRct.x + backgroundSlideRct.w - (Sliderdest->w / 2)){
					//Actualizamos la posición del Slider
					sliderImage->moveRect(newPos, Sliderdest->y);
					//Calculamos el valor con respecto al tamaño total del Slider
					//Cancelamos la posicion de la mitad de la imagen
					int value = newPos - backgroundSlideRct.x + (Sliderdest->w / 2);
					float valuePercentage = (float)value/(float)backgroundSlideRct.w;
					callback_(valuePercentage, entity_);
				}
			}
		}
	}
}

/// <summary>
/// Posicion el slider dado un valor de 0 a 1
/// </summary>
/// <param name="value"> de 0 a 1</param>
void Slider::setSlider(float& value){
	int valor = backgroundSlideRct.w * value;
	int newPos = backgroundSlideRct.x + valor - (Sliderdest->w / 2);

	sliderImage->moveRect(newPos, Sliderdest->y);
}

void Slider::render(){
	textureText->render(((backgroundSlideRct.x + (backgroundSlideRct.w/2)) - (textureText->width()/2)), backgroundSlideRct.y - 200);
}

