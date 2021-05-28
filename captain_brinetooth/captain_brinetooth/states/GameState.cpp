#include "GameState.h"
#include "../ecs/Manager.h" //Manager ya incluye Entity
#include "../utils/Vector2D.h"
#include <fstream>

GameState::GameState(App* a, std::shared_ptr<b2World> mundo, SoundManager* snd) {
	app = a;
	popped = false;
	fader = nullptr;
	manager_ = new Manager(a,mundo, snd, &popped);
}

GameState::~GameState() {
	//Se tiene que borrar el manager de cada Estado
	delete manager_;
}

Entity* GameState::createBasicEntity(const Vector2D& pos, const Vector2D& size, const float& rotation, const Vector2D& vel){
	auto* entity_ = manager_->addEntity();
	entity_->addComponent<Transform>(pos, vel, size.getX(), size.getY(), rotation);
	return entity_;
}

void GameState::render() const {
	manager_->render();
	//Se tiene que dibujar siempre lo último
	if(fader != nullptr)
		fader->render();
}
void GameState::update() {
	manager_->update();
	
}
void GameState::refresh() {
	manager_->refresh();
}

void GameState::init(){
	fader = manager_->addEntity();
	fader->addComponent<Fade>(Vector2D(App::camera.w, App::camera.h), Vector2D(0, 0), 1000, 1000);
	manager_->setHandler<Fader>(fader);
}

SDL_Rect GameState::ScaleSDL_Rect(Texture* text, const Vector2D& pos, float zoomFactor, float sizeFactor, bool centered){
	SDL_Rect rect;

	rect.w = text->width() * zoomFactor * sizeFactor;
	rect.h = text->height() * zoomFactor * sizeFactor;

	if (centered){
		rect.x = pos.getX() - (rect.w / 2);
		rect.y = pos.getY();
	}
	else{
		rect.x = pos.getX();
		rect.y = pos.getY();
	}
	return rect;
}

void GameState::saveGame()
{
	//Inicializacion de variables auxiliares
	int money = manager_->getHandler<Player>()->getComponent<Inventory>()->getCoins();
	auto* skilltree = manager_->getHandler<Player>()->getComponent<SkillTree>();
	//Guardado de archivo
	std::ofstream output;
	string name = "data";
	string file = "assets//user_data//" + name + ".dat";
	output.open(file);
	if (!output.is_open()) throw string("Can't find file" + name + ".dat");
	output << money << endl;
	for (int i = 0; i < skilltree->getMaxSkills(); i++){
		int ab = (skilltree->hasSkill(i))? 1 : 0;
		output << ab << " "; //Guardamos la informacion de las habilidades en funcion de 0 1 para facilitar la conversion al leer el archivo
	}
	output.close();
}

void GameState::loadGame()
{
	if (manager_->getHandler<Player>()->hasComponent<SkillTree>()) {
		ifstream readtxt;
		int pointsRead = 0;
		string file = "data.dat";
		readtxt.open("assets//user_data//" + file);
		if (!readtxt) throw string("Can't find file" + file);
		readtxt >> pointsRead;
		std::array<bool, 5>infoabilities; //Guardamos las abilidades del txt en el documento de text
		int ability;
		for (int i = 0; i < infoabilities.size(); i++){
			//Vamos leyendo habilidad a habilidad  guardandola en el array
			readtxt >> ability;
			infoabilities[i] = (bool)ability;
		}
		//Tras leer las habilidades se las damos al usuario 
		manager_->getHandler<Player>()->getComponent<Inventory>()->addCoins(pointsRead);
		manager_->getHandler<Player>()->getComponent<SkillTree>()->initSkillsFromMatch(infoabilities);
	}
}
