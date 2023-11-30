#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Ogre.h>

using namespace Ogre;

class Player {
public:
	Player(Camera* camera, SceneNode* node, Entity* entity) {
		this->playerCamera = camera;
		this->playerNode = node;
		this->playerEntity = entity;
	};
	
	Camera* getPlayerCamera() {
		return this->playerCamera;
	}

	SceneNode* getPlayerNode() {
		return this->playerNode;
	}

	Entity* getPlayerEntity() {
		return this->playerEntity;
	}

    void setPlayerFisicBody(btRigidBody* fisicBody){
        this->playerFisicBody = fisicBody;
    }

    btRigidBody* getPlayerFisicBody(){
        return this->playerFisicBody;
    }

private:
	Camera* playerCamera;
	SceneNode* playerNode;
	Entity* playerEntity;
    btRigidBody* playerFisicBody; 


};
