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

Slider::~Slider(){
}

void Slider::init(){
	mngr = entity_->getMngr();

	background = mngr->addEntity();

	SDL_Rect rectPos;
	rectPos.x = 0;
	rectPos.y = 0;
	rectPos.w = App::camera.w;
	rectPos.h = App::camera.h;

	Texture* imageTexture = textures[0];
	sizeFactor = 0.5;
	float factor_ = App::camera_Zoom_Out;

	rectPos = a->getStateMachine()->currentState()->ScaleSDL_Rect(imageTexture, Vector2D(App::camera.w / 2, App::camera.h * 0.2), factor_, sizeFactor, false);
	rectPos.x += rectPos.x / 4;
	rectPos.y += rectPos.y * posY;

	backgroundSlideRct = build_sdlrect(pos, sizes.first.getX(), sizes.first.getY());
	background->addComponent<Image>(textures[0], rectPos, "fondo");	
	

	slide = mngr->addEntity();

	sizeFactor = 0.5;
	imageTexture = textures[1];
	rectPos = a->getStateMachine()->currentState()->ScaleSDL_Rect(imageTexture, Vector2D(App::camera.w , App::camera.h * 0.2), factor_, sizeFactor, true);
	rectPos.x += rectPos.x / 2; // Esto por alguna razon no funciona muy bien
	rectPos.y += (rectPos.y * (posY/1.1));

	sliderImage = slide->addComponent<Image>(textures[1], rectPos, "fondo");

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
				//Necesito esto para saber en qué momento selecciona la imagen del Slider
				selected = true;
			}
		}
		
	}

	//Compruebo que ha soltado el boton y que si se ha seleccionado la imagen
	if (!ih().getLeftMouseButtonPressed() && selected) selected = false;
	else if(selected) { //No me sirve con verificar que sólo mantiene el boton izquierdo presionado. Necesito comprobar que también la imagen sigue seleccionada
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
void Slider::setSlider(float& value){
	int valor = backgroundSlideRct.w * value;
	int newPos = backgroundSlideRct.x + valor - (Sliderdest->w / 2);
	sliderImage->moveRect(newPos, Sliderdest->y);
}

void Slider::render(){
	textureText->render(((backgroundSlideRct.x + (backgroundSlideRct.w/2)) - (textureText->width()/2)), backgroundSlideRct.y - 200);
}

void Slider::desactivateSlider(){
	background->setActive(false);
	slide->setActive(false);
}

