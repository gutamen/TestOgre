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

private:
	Camera* playerCamera;
	SceneNode* playerNode;
	Entity* playerEntity;

};