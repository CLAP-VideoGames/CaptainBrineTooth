#include "Button.h"

Button::Button(Texture* t, CallBackOnClick* c, App* g, SoundManager* snd) : tex(t){
	cboq = c;
	game = g;
	soundController = snd;
	selected = false;
}

Button::~Button(){

}

void Button::init(){
	apply_offset = false;
}

void Button::update(){
	handleEvent();
	if (fade != nullptr){
		if(fade->getFadeOutComplete())
			cboq(game, soundController);
	}
}

void Button::render(){
	Transform* trans = entity_->getComponent<Transform>();

	Vector2D aux = trans->getPos();
	SDL_Rect dest;
	if (!apply_offset)
		dest = build_sdlrect(aux, trans->getW(), trans->getH());
	else   //offset del boton presionado
		dest = build_sdlrect(trans->getPos().getX() + offset_pressed * 0.5f, trans->getPos().getY() + offset_pressed*2.0,
			trans->getW() - offset_pressed, trans->getH() - offset_pressed);

	tex->render(dest);

	if (sdlutils().getDebug()){
		SDL_SetRenderDrawColor(sdlutils().renderer(), 255, 0, 0, 255);
		SDL_RenderDrawRect(sdlutils().renderer(), &dest);
	}
}

bool Button::handleEvent(){
	Transform* tr = entity_->getComponent<Transform>();
	SDL_Point mouseP = { ih().getMousePos().first, ih().getMousePos().second};
	SDL_Rect dest = build_sdlrect(tr->getPos(), tr->getW(), tr->getH());
	if (SDL_PointInRect(&mouseP, &dest) == SDL_TRUE) {
		setTexColor(192, 192, 192, 255);
		//Si ha habido algun evento de rat, o el usuario mantiene presionado algun button, entra
		if (ih().mouseButtonEvent() || ih().getMouseButtonHeld()) {
			//Si el boton fue el izquierdo o si el izquierdo se sigue presionando
			if (ih().getMouseButtonState(InputHandler::MOUSEBUTTON::LEFT) || ih().getLeftMouseButtonPressed()) {
				//"Animacion de boton" 
				apply_offset = true;
				//Necesito esto para saber en qu・momento selecciona la imagen del Slider
				selected = true;
			}
		}
		//Si el usuario sigue dentro del boton, 駸te ha sido seleccionado, y el boton izquierdo ya no est・mantenido
		if (!ih().getLeftMouseButtonPressed() && selected) {
			//En caso de haber un componente Fade, ejecutar su callback cuando haya terminado el fade OUt (Update)
			fade = entity_->getComponent<Fade>();
			if (fade != nullptr) {
				fade->setState(Fade::STATE_FADE::Out);
				soundController->playSoundEffect("sonido_barco", 0);
			}
			else {
				cboq(game, soundController);
			}
			//Ha soltado el button, pues ya no est・seleccionado
			selected = false;
		}

		return true;
	}//Si ha salido del button, deja de estar seleccionado ( para que si misclickea, pueda cancelar dicho click)
	else {
		//"Animacion de boton" reset
		apply_offset = false;
		setTexColor(255, 255, 255, 255);
		selected = false;
	}

	return false;
}

void Button::setTex(Texture* newTexture)
{
	tex = newTexture;
}

void Button::setTexColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	SDL_SetTextureColorMod(tex->getTexture(), r, g, b);
	SDL_SetTextureAlphaMod(tex->getTexture(), a);
}
