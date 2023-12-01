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
        this->physicController = new Physics(); 
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
    
    btCollisionObject* addCollisionObjectInNode(Entity *ent, ColliderType ct, int group = 1, int mask = -1){
        return physicController->addCollisionObjectInNode(ent, ct, group, mask);
    }

    btRigidBody* addCollisionBodyInNode(float mass, Entity* ent, ColliderType ct, CollisionListener* cl = nullptr, int group = 1, int mask = -1){
        return this->physicController->addCollisionBodyInNode(mass, ent, ct, cl, group, mask);
    }
    
    Physics* getPhysicsController(){
        return this->physicController;
    }
	
private:
	KeyHandler* inputController;
	Updater* frameController;
    Physics* physicController;
	Player* playerInstance;
};
