#include "Slider.h"

#include <iostream>

Slider::Slider(const Vector2D& pos_, const std::pair<Vector2D, Vector2D>& sizes_, Texture* texts[NUM_TEXTURES], void(*callback)(float, Entity*), Texture* text_, App* ap, float sFactor, float pY){
	pos = pos_;
	sizes = sizes_;
	for (int i = 0; i < NUM_TEXTURES; i++) textures[i] = texts[i];

	callback_ = callback;

	textureText = text_;
	a = ap;

	sizeFactor = sFactor;
	posY = pY;

	selected = false;
}

Slider::~Slider() {
}

void Slider::init() {
	mngr = entity_->getMngr();
	float factor_ = a->getCameraZooOut();

	//FondoSlider
	background = mngr->addEntity();
	backgroundSlideRct.w = 600 * factor_;
	backgroundSlideRct.h = 90 * factor_;

	backgroundSlideRct.x = pos.getX() - (backgroundSlideRct.w / 2);
	backgroundSlideRct.y = pos.getY();
	background->addComponent<Image>(textures[0], backgroundSlideRct, "fondo");


	//Titulo : No cambiar de orden,  tiene que ir antes el FondoSlider
	titleDest.w = textureText->width() * factor_ * sizeFactor;
	titleDest.h = textureText->height() * factor_ * sizeFactor;

	titleDest.x = (backgroundSlideRct.x + (backgroundSlideRct.w / 2)) - (titleDest.w / 2);
	titleDest.y = backgroundSlideRct.y - titleDest.h - (posY * factor_);


	//Slider 
	slide = mngr->addEntity();
	Vector2D posSlide = Vector2D(pos.getX() * 1.1f, pos.getY() - (backgroundSlideRct.h * 0.5f));

	SDL_Rect slideRect;

	slideRect.w = sizes.second.getX() * factor_;
	slideRect.h = sizes.second.getY() * factor_;

	slideRect.x = posSlide.getX() - (slideRect.w / 2);
	slideRect.y = posSlide.getY();

	//Guardamos el rectangulo del slider
	sliderImage = slide->addComponent<Image>(textures[1], slideRect, "fondo");
	Sliderdest = (sliderImage->getDestRect());
}

void Slider::update() {
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second };
	if (SDL_PointInRect(&mouseP, Sliderdest) == SDL_TRUE) {
		//Si ha habido un evento de raton o si se sigue presionando
		if (ih().mouseButtonEvent() || ih().getMouseButtonHeld()) {
			//Si el boton fue el izquierdo o si el izquierdo se sigue presionando
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().getLeftMouseButtonPressed()) {
				//std::cout << "pressed" << std::endl;
				//Necesito esto para saber en qué momento selecciona la imagen del Slider
				selected = true;
			}
		}

	}

	//Compruebo que ha soltado el boton y que si se ha seleccionado la imagen
	if (!ih().getLeftMouseButtonPressed() && selected) selected = false;
	else if (selected) { //No me sirve con verificar que sólo mantiene el boton izquierdo presionado. Necesito comprobar que también la imagen sigue seleccionada
		int newPos = mouseP.x - (Sliderdest->w / 2);
		if (newPos >= backgroundSlideRct.x - (Sliderdest->w / 2) && newPos <= backgroundSlideRct.x + backgroundSlideRct.w - (Sliderdest->w / 2)) {
			//Actualizamos la posición del Slider
			sliderImage->moveRect(newPos, Sliderdest->y);
			//Calculamos el valor con respecto al tamaño total del Slider
			//Cancelamos la posicion de la mitad de la imagen
			int value = newPos - backgroundSlideRct.x + (Sliderdest->w / 2);
			float valuePercentage = (float)value / (float)backgroundSlideRct.w;
			callback_(valuePercentage, entity_);
		}
	}
}

/// <summary>
/// Posicion el slider dado un valor de 0 a 1
/// </summary>
/// <param name="value"> de 0 a 1</param>
void Slider::setSlider(float& value) {
	int valor = backgroundSlideRct.w * value;
	int newPos = backgroundSlideRct.x + valor - (Sliderdest->w / 2);
	sliderImage->moveRect(newPos, Sliderdest->y);
}

void Slider::render() {
	textureText->render(titleDest);
	if (sdlutils().getDebug()) {
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &titleDest);
	}
}

void Slider::desactivateSlider() {
	background->setActive(false);
	slide->setActive(false);
}

