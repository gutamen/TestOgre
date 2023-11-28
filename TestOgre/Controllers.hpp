#include <Ogre.h>
#include "Updater.hpp"
#include "KeyHandler.hpp"
#include "Player.hpp"

using namespace std;

class Controllers {


public:
	Controllers(SceneManager* scene) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
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