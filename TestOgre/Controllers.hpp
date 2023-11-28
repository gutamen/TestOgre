#include <Ogre.h>
#include "Updater.hpp"
#include "KeyHandler.hpp"
#include "Player.hpp"

using namespace std;

class Controllers {


public:
	Controllers(SceneManager* scene, Player* player) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
		this->playerInstances = player;
	}

	Controllers(SceneManager* scene, Camera* playerCamera, SceneNode* playerNode, Entity* playerEntity) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
		this->playerInstances = new Player(playerCamera, playerNode, playerEntity);
	}

	KeyHandler* getInputController() {
		return this->inputController;
	}

	Updater* getFrameController() {
		return this->frameController;
	}
	
private:
	KeyHandler* inputController;
	Updater* frameController;
	Player* playerInstances;

};