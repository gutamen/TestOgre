#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgrePrerequisites.h>
#include <OgreSceneManager.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <OgreBullet.h>
#include <OgreEntity.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <iostream>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <OgreFrameListener.h>

class Player{
public:
	Player(Ogre::Camera* camera, Ogre::SceneNode* node, Ogre::Entity* entity) {
		this->playerCamera = camera;
		this->playerNode = node;
		this->playerEntity = entity;
	};
	
	Ogre::Camera* getPlayerCamera() {
		return this->playerCamera;
	}

	Ogre::SceneNode* getPlayerNode() {
		return this->playerNode;
	}

	Ogre::Entity* getPlayerEntity() {
		return this->playerEntity;
	}

    void setPlayerFisicBody(btRigidBody* fisicBody){
        this->playerFisicBody = fisicBody;
    }

    btRigidBody* getPlayerFisicBody(){
        return this->playerFisicBody;
    }

private:
	Ogre::Camera* playerCamera;
	Ogre::SceneNode* playerNode;
	Ogre::Entity* playerEntity;
    btRigidBody* playerFisicBody; 


};





class KeyHandler : public OgreBites::InputListener{

public:
    KeyHandler(Ogre::SceneManager* sceneManager)
    {
        // Obtém o SceneManager associado à janela de renderização
        this->sceneManager = sceneManager;
        this->playerCamera = sceneManager->getCamera("Camera");

    }

    KeyHandler(Ogre::SceneManager* sceneManager, Player* player){
        this->sceneManager = sceneManager;
        this->player = player;
        this->playerCamera = player->getPlayerCamera();
    }


    bool keyReleased(const OgreBites::KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 119:
            // Move o objeto para cima
            wIsPressed  = false;
            //mNode->translate(direction);
            break;

        case 115:
            sIsPressed  = false;
            break;
        }


        return true;
    }

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 119:
            // Move o objeto para cima
            wIsPressed  = true;
            //mNode->translate(direction);
            break;
            /*case 97:
                // Move o objeto para a esquerda
                mNode->translate(-1.0f, 0.0f, 0.0f);
                break;
            */
        case 115:
            // Move o objeto para baixo
            sIsPressed  = true;
            //mNode->translate(direction*-1);
            break;
        case 100:
            // Move o objeto para a direita
            sceneManager->getEntity("Suzanne")->getParentNode()->translate(Ogre::Vector3(0, 1, 0));
            break;

        }

        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {

            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override {
        playerCamera->getParentNode()->yaw(Ogre::Radian(-evt.xrel * 0.005), Ogre::Node::TS_WORLD);
        playerCamera->getParentNode()->pitch(Ogre::Radian(-evt.yrel * 0.005));
        return true;
    }

    bool pressedW(){
        return this->wIsPressed;
    }

    bool pressedS(){
        return this->sIsPressed;
    }

private:
    Ogre::Node* playerNode;
    Ogre::Real MoveSpeed;
    Ogre::Real timer = 0;
    Player* player;
    bool sIsPressed =  false;
    bool wIsPressed = false;
    Ogre::SceneManager* sceneManager;
    Ogre::Camera* playerCamera;
    
};


class Physics {

public:
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    std::vector<btCollisionShape*> collisionShapes;
    std::map<std::string, btRigidBody*> physicsAccessors;
    Ogre::Bullet::DynamicsWorld* ogreAdapter;

    
public:
    Physics(){
/*        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);       
        ogreAdapter = new Ogre::Bullet::DynamicsWorld(dynamicsWorld);
*/        
        ogreAdapter = new Ogre::Bullet::DynamicsWorld(Ogre::Vector3(0, 9.8, 0));
        btDynamicsWorld* dynamicsWorld = ogreAdapter->getBtWorld();
    }

public:
    btCollisionObject* addCollisionObjectInNode(Ogre::Entity *ent, Ogre::Bullet::ColliderType ct, int group = 1, int mask = -1){
        btCollisionObject* object = this->ogreAdapter->addCollisionObject(ent, ct, group, mask);
        object->getWorldTransform().setOrigin(Ogre::Bullet::convert(ent->getParentNode()->getPosition()));
        return object;
    }

    btRigidBody* addCollisionBodyInNode(float mass, Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1){
        return this->ogreAdapter->addRigidBody(mass, ent, ct, cl, group, mask);
    }


};

class Updater : public Ogre::FrameListener{
public:
    Updater() {

    }

    Updater(KeyHandler* keyHandler, Player* player, Physics* physics){
        this->player = player;
        this->keyHandler = keyHandler;
        this->physics = physics;
        if(player->getPlayerFisicBody() != nullptr){
            this->playerBody = player->getPlayerFisicBody();
        }

    }

    bool frameStarted(const Ogre::FrameEvent& frameRendered) override {
        
        tick += frameRendered.timeSinceLastFrame;
        if(tick >= 0.016){

            if(keyHandler->pressedW()){
                player->getPlayerNode()->translate(player->getPlayerCamera()->getRealDirection());
                btVector3 teste = Ogre::Bullet::convert(player->getPlayerNode()->getPosition());
                playerBody->getWorldTransform();
                //origin = teste;
                //origin = playerBody->getWorldTransform().getOrigin();
                //cout << origin.getX() << " " << origin.getY() << " " << origin.getZ() << endl;
            } 

            if(keyHandler->pressedS()){
                player->getPlayerNode()->translate(player->getPlayerCamera()->getRealDirection()*-1);
                //playerBody->getWorldTransform().setOrigin(convert(player->getPlayerNode()->getPosition()));
            }


            tick = 0;
        }
        return true;
    }

private:
    Physics* physics;
    btRigidBody* playerBody;
    KeyHandler* keyHandler;
    Player* player;
    Ogre::Real tick = 0;
    Ogre::Real tickSpeed = 1.0/60.0;
};

class Controllers {

public:
	Controllers(Ogre::SceneManager* scene, Player* player) {
		this->frameController = new Updater();
		this->inputController = new KeyHandler(scene);
		this->playerInstance = player;
	}

	Controllers(Ogre::SceneManager* scene, Ogre::Camera* playerCamera, Ogre::SceneNode* playerNode, Ogre::Entity* playerEntity) {
        this->playerInstance = new Player(playerCamera, playerNode, playerEntity);
        this->physicController = new Physics(); 
		this->inputController = new KeyHandler(scene);
        this->frameController = new Updater(inputController, playerInstance, physicController);
	}

    // Construtor Principal
	Controllers(Ogre::SceneManager* scene, Ogre::Camera* playerCamera, Ogre::SceneNode* playerNode, Ogre::Entity* playerEntity, bool autoFill){
        this->playerInstance = new Player(playerCamera, playerNode, playerEntity);
        this->physicController = new Physics(); 
		this->inputController = new KeyHandler(scene);
        this->frameController = new Updater(inputController, playerInstance, physicController);
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
    
    btCollisionObject* addCollisionObjectInNode(Ogre::Entity *ent, Ogre::Bullet::ColliderType ct, int group = 1, int mask = -1){
        return physicController->addCollisionObjectInNode(ent, ct, group, mask);
    }

    btRigidBody* addCollisionBodyInNode(float mass, Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1){
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


