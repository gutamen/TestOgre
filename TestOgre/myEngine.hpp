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
#include <iterator>


struct playerCollision : public Ogre::Bullet::CollisionListener
{

    void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) override {
        std::cout << "teste" << std::endl;
    }
};

class Player {
public:
    Player(Ogre::Camera* camera, Ogre::SceneNode* node, Ogre::Entity* entity) {
        this->playerCamera = camera;
        this->playerNode = node;
        this->playerEntity = entity;
    };

    Player(Ogre::Camera* camera, Ogre::SceneNode* node, Ogre::Entity* entity, btRigidBody* fisicBody){
        this->playerCamera = camera;
        this->playerNode = node;
        this->playerEntity = entity;
        this->playerFisicBody = fisicBody;
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

    void setPlayerFisicBody(btRigidBody* fisicBody) {
        this->playerFisicBody = fisicBody;
    }

    btRigidBody* getPlayerFisicBody() {
        return this->playerFisicBody;
    }

    bool translate(Ogre::Vector3 movement) {
        this->playerNode->translate(movement);
        this->playerFisicBody->translate(Ogre::Bullet::convert(movement));

        return true;
    }

private:
    Ogre::Camera* playerCamera;
    Ogre::SceneNode* playerNode;
    Ogre::Entity* playerEntity;
    btRigidBody* playerFisicBody = nullptr;

};





class KeyHandler : public OgreBites::InputListener {

public:
    KeyHandler(Ogre::SceneManager* sceneManager)
    {
        // Obtém o SceneManager associado à janela de renderização
        this->sceneManager = sceneManager;
        this->playerCamera = sceneManager->getCamera("Camera");

    }

    KeyHandler(Ogre::SceneManager* sceneManager, Player* player) {
        this->sceneManager = sceneManager;
        this->player = player;
        this->playerCamera = player->getPlayerCamera();
    }


    bool keyReleased(const OgreBites::KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 103:
            gIsPressed = false;
            break;

        case 115:
            sIsPressed = false;
            break;

        case 119:
            wIsPressed = false;
            break;
        }

        return true;
    }

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 100:
            sceneManager->getEntity("Suzanne")->getParentNode()->translate(Ogre::Vector3(0, 1, 0));
            break;

        case 103:
            gIsPressed = true;
            break;

        case 119:
            wIsPressed = true;
            break;

        case 115:
            sIsPressed = true;
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

    bool pressedW() {
        return this->wIsPressed;
    }

    bool pressedS() {
        return this->sIsPressed;
    }

    bool pressedG() {
        return this->gIsPressed;
    }

private:
    Ogre::Node* playerNode;
    Ogre::Real MoveSpeed;
    Ogre::Real timer = 0;
    Player* player;
    bool sIsPressed = false;
    bool wIsPressed = false;
    bool gIsPressed = false;
    Ogre::SceneManager* sceneManager;
    Ogre::Camera* playerCamera;

};


class Physics {

private:
    //btDefaultCollisionConfiguration* collisionConfiguration;
    //btCollisionDispatcher* dispatcher;
    //btBroadphaseInterface* overlappingPairCache;
    //btSequentialImpulseConstraintSolver* solver;
    //btDiscreteDynamicsWorld* dynamicsWorld;
    //std::vector<btCollisionShape*> collisionShapes;
    //std::map<std::string, btRigidBody*> physicsAccessors;
    Ogre::Bullet::DynamicsWorld* ogreAdapter;
    btDynamicsWorld* dynamicsWorld;


public:
    Physics() {
        /*        collisionConfiguration = new btDefaultCollisionConfiguration();
                dispatcher = new btCollisionDispatcher(collisionConfiguration);
                overlappingPairCache = new btDbvtBroadphase();
                solver = new btSequentialImpulseConstraintSolver();
                dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
                ogreAdapter = new Ogre::Bullet::DynamicsWorld(dynamicsWorld);
        */
        ogreAdapter = new Ogre::Bullet::DynamicsWorld(Ogre::Vector3(0, 9.8, 0));
        dynamicsWorld = ogreAdapter->getBtWorld();
    }

public:
    btCollisionObject* addCollisionObjectInNode(Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, int group = 1, int mask = -1) {
        btCollisionObject* object = this->ogreAdapter->addCollisionObject(ent, ct, group, mask);
        object->getWorldTransform().setOrigin(Ogre::Bullet::convert(ent->getParentNode()->getPosition()));
        return object;
    }

    btRigidBody* addCollisionBodyInNode(float mass, Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1) {
        return this->ogreAdapter->addRigidBody(mass, ent, ct, cl, group, mask);
    }

    btDynamicsWorld* getWorld() {
        return this->dynamicsWorld;
    }

    btCollisionObjectArray getCollisionObjects() {
        return dynamicsWorld->getCollisionObjectArray();
    }


};

class Updater : public Ogre::FrameListener {
public:
    Updater() {

    }

    Updater(KeyHandler* keyHandler, Player* player, Physics* physics) {
        this->player = player;
        this->keyHandler = keyHandler;
        this->physics = physics;
        if (player->getPlayerFisicBody() != nullptr) {
            this->playerBody = player->getPlayerFisicBody();
        }

    }

    bool frameStarted(const Ogre::FrameEvent& frameRendered) override {

        tick += frameRendered.timeSinceLastFrame;
        if (tick >= 0.016) {

            if (keyHandler->pressedW()) {
                player->translate(player->getPlayerCamera()->getRealDirection());
            }

            if (keyHandler->pressedS()) {
                player->translate(player->getPlayerCamera()->getRealDirection() * -1);
            }

            if (keyHandler->pressedG()){
//                btVector3 body0 = playerBody->getWorldTransform().getOrigin();
//                std::cout << body0.x() << " " << body0.y() << " " << body0.z() << std::endl; 
                
                std::cout << playerBody->getUserPointer() << std::endl;
                std::cout << physics->getCollisionObjects().at(0)->getUserPointer() << std::endl << std::endl;

//                btVector3 body1 = physics->getCollisionObjects().at(1)->getWorldTransform().getOrigin();
//                std::cout << body1.x() << " " << body1.y() << " " << body1.z() << std::endl << std::endl; 
                
//                std::cout << player->getPlayerNode()->getPosition().x << " " << player->getPlayerNode()->getPosition().y << " " << player->getPlayerNode()->getPosition().z << std::endl << std::endl;


//                std::cout << physics->getCollisionObjects().at(0)->getUserPointer() << std::endl;
//                std::cout << player->getPlayerFisicBody()->getUserPointer() << std::endl << std::endl;

//                std::cout << physics->getCollisionObjects().at(0) << std::endl;
//                std::cout << player->getPlayerFisicBody() << std::endl << std::endl;

//                std::cout << physics << std::endl << std::endl;

            }

            physics->getWorld()->stepSimulation(0.166);

            tick = 0;
        }
        return true;
    }

private:
    Physics* physics;
    btRigidBody* playerBody = nullptr;
    KeyHandler* keyHandler;
    Player* player;
    Ogre::Real tick = 0;
    Ogre::Real tickSpeed = 1.0 / 60.0;
};

struct EntityCollisionListener
{
    const Ogre::MovableObject* entity;
    Ogre::Bullet::CollisionListener* listener;
};

static void localTick(btDynamicsWorld* world, btScalar timeStep)
{
    int numManifolds = world->getDispatcher()->getNumManifolds();
    auto manifolds = world->getDispatcher()->getInternalManifoldPointer();
    for (int i = 0; i < numManifolds; i++)
    {
        btPersistentManifold* manifold = manifolds[i];

        for (int j = 0; j < manifold->getNumContacts(); j++)
        {
            const btManifoldPoint& mp = manifold->getContactPoint(i);
            auto body0 = static_cast<EntityCollisionListener*>(manifold->getBody0()->getUserPointer());
            auto body1 = static_cast<EntityCollisionListener*>(manifold->getBody1()->getUserPointer());
            std::cout << body0 << std::endl << body1 << std::endl << std::endl;
//            if (body0->listener)
//                body0->listener->contact(body1->entity, mp);
//            if (body1->listener)
//                body1->listener->contact(body0->entity, mp);

        }
    }
}

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
        this->physicController = new Physics();
        this->inputController = new KeyHandler(scene);
        btRigidBody* playerBody = this->addCollisionBodyInNode(0, playerEntity, Ogre::Bullet::CT_SPHERE, new playerCollision());
        physicController->getWorld()->setInternalTickCallback(localTick);
        this->playerInstance = new Player(playerCamera, playerNode, playerEntity, playerBody);
        this->frameController = new Updater(inputController, playerInstance, physicController);
    }


    KeyHandler* getInputController() {
        return this->inputController;
    }

    Updater* getFrameController() {
        return this->frameController;
    }

    void setPlayerFisicBody(btRigidBody* playerBody) {
        this->playerInstance->setPlayerFisicBody(playerBody);
    }

    btRigidBody* getPlayerBody() {
        return this->playerInstance->getPlayerFisicBody();
    }

    btCollisionObject* addCollisionObjectInNode(Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, int group = 1, int mask = -1) {
        return physicController->addCollisionObjectInNode(ent, ct, group, mask);
    }

    btRigidBody* addCollisionBodyInNode(float mass, Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1) {
        return this->physicController->addCollisionBodyInNode(mass, ent, ct, cl, group, mask);
    }

    Physics* getPhysicsController() {
        return this->physicController;
    }

private:
    KeyHandler* inputController;
    Updater* frameController;
    Physics* physicController;
    Player* playerInstance;
};

