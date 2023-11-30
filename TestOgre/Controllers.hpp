#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Ogre.h>
#include "Updater.hpp"

using namespace std;

class Controllers {


public:
	Controllers(SceneManager* scene, Player* player) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
		this->playerInstance = player;
	}

	Controllers(SceneManager* scene, Camera* playerCamera, SceneNode* playerNode, Entity* playerEntity) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
		this->playerInstance = new Player(playerCamera, playerNode, playerEntity);
	}

	KeyHandler* getInputController() {
		return this->inputController;
	}

	Updater* getFrameController() {
		return this->frameController;
	}

    void setPlayerFisicBody(btRigidBody* playerBody){
        this->playerInstance->setPlayerFisicBody(playerBody);
    }
	
private:
	KeyHandler* inputController;
	Updater* frameController;
	Player* playerInstance;

};