#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreInput.h>
#include <OgrePrerequisites.h>
#include <OgreRay.h>
#include <OgreSceneManager.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <OgreBullet.h>
#include <OgreEntity.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <OgreTrays.h>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <OgreFrameListener.h>
#include <iterator>
#include <limits>
#include <memory>
#include <OgreTerrainGroup.h>

#ifndef MYENGINE_H
#define MYENGINE_H

namespace MyEngine {

    struct playerRay : public Ogre::Bullet::RayResultCallback {
        float distance = std::numeric_limits<float>::max();

        void addSingleResult(const Ogre::MovableObject* other, float distance) override {
            this->distance = distance;
        }

        float getDistanceInDirection() {
            float aux = distance;
            distance = std::numeric_limits<float>::max();
            return aux;
        }
    };

    struct playerCollision : public Ogre::Bullet::CollisionListener {


        playerCollision(Ogre::MovableObject* player) {
            this->player = player;
        }
        Ogre::MovableObject* player;
        void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) override {
            //        std::cout << other->getName() << std::endl;
            //        std::cout << player->getName() << std::endl;
        }

    };

    struct testCollision : public Ogre::Bullet::CollisionListener {


        testCollision(Ogre::MovableObject* object) {
            this->object = object;
        }
        Ogre::MovableObject* object;
        void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) override {
            //        std::cout << other->getName() << std::endl;
            std::cout << object->getName() << std::endl;
        }

    };

    // Faz referência ao jogador, contém o nodo, a entidade e camera para o jogador
    class Player {
    public:
        Player(Ogre::Camera* camera, Ogre::SceneNode* node, Ogre::Entity* entity) {
            this->playerCamera = camera;
            this->playerNode = node;
            this->playerEntity = entity;
        };

        // Construtor principal, define os principais objetos do jogador
        Player(Ogre::Camera* camera, Ogre::SceneNode* node, Ogre::Entity* entity, btRigidBody* fisicBody) {
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




    // Gerenciador de entradas
    struct KeyHandler : public OgreBites::InputListener {

    public:
        KeyHandler(){}

        ~KeyHandler() {}

        bool keyReleased(const OgreBites::KeyboardEvent& evt) override {
            switch (evt.keysym.sym) {
                case 100:
                    dIsPressed = false;
                    break;

                case 103:
                    gIsPressed = false;
                    break;

                case 115:
                    sIsPressed = false;
                    break;

                case 119:
                    wIsPressed = false;
                    break;
            
                // Alt solto
                case 1073742050:
                    altIsPressed = false;
            }
            return true;
            
        }

        bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
            switch (evt.keysym.sym) {
                case 100:
                    dIsPressed = true;
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
            
                // Alt pressionado
                case 1073742050:    
                    altIsPressed = true;

            }

            if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
            {
                Ogre::Root::getSingleton().queueEndRendering();
            }
            return true;
        }

        bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override {
            this->mouseMovement.x += -evt.xrel;
            this->mouseMovement.y += -evt.yrel;
            return true;
        }

        Ogre::Vector2 mouseMovementRel() {
            return this->mouseMovement;
        }

        void mouseMovementReset() {
            this->mouseMovement = Ogre::Vector2::ZERO;
        }

        bool pressedD() {
            return this->dIsPressed;
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

        bool pressedAlt(){
            return this->altIsPressed;
        }

    private:
        Ogre::Vector2 mouseMovement = Ogre::Vector2::ZERO;
        Ogre::Vector2 cursorPosition = Ogre::Vector2::ZERO;
        bool dIsPressed = false;
        bool sIsPressed = false;
        bool wIsPressed = false;
        bool gIsPressed = false;
        bool altIsPressed = false;
    };

    // Classe usa a biblioteca Bullet para gerenciar física
    class Physics {

    private:
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btBroadphaseInterface* overlappingPairCache;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
        std::vector<btCollisionShape*> collisionShapes;
        std::map<std::string, btRigidBody*> physicsAccessors;
        Ogre::Bullet::DynamicsWorld* ogreAdapter;


    public:
        Physics() {
            collisionConfiguration = new btDefaultCollisionConfiguration();
            dispatcher = new btCollisionDispatcher(collisionConfiguration);
            overlappingPairCache = new btDbvtBroadphase();
            solver = new btSequentialImpulseConstraintSolver();
            dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
            ogreAdapter = new Ogre::Bullet::DynamicsWorld(dynamicsWorld);
        }

    public:

        btCollisionObject* addCollisionObjectInNode(Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1);

        btRigidBody* addCollisionBodyInNode(float mass, Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1) {
            return this->ogreAdapter->addRigidBody(mass, ent, ct, cl, group, mask);
        }

        btDiscreteDynamicsWorld* getWorld() {
            return this->dynamicsWorld;
        }

        Ogre::Bullet::DynamicsWorld* getOgreWorld() {
            return this->ogreAdapter;
        }

        btCollisionObjectArray getCollisionObjects() {
            return dynamicsWorld->getCollisionObjectArray();
        }


    };

    // Monitorador da geração de quadros, principal local de controle
    class Updater : public Ogre::FrameListener {
    public:
        Updater() {

        }

        Updater(KeyHandler* keyHandler, Player* player, Physics* physics, 
            OgreBites::TrayManager* trays, Ogre::TerrainGroup* terrainGroup = nullptr) {
            this->player = player;
            this->keyHandler = keyHandler;
            this->physics = physics;
            this->trays = trays;
            if (player->getPlayerFisicBody() != nullptr) {
                this->playerBody = player->getPlayerFisicBody();
            }
            this->playerDirectionTester = new playerRay();
            this->terrainGroup = terrainGroup;

        }

        bool frameStarted(const Ogre::FrameEvent& frameRendered) override {

            tick += frameRendered.timeSinceLastFrame;
            if (tick >= 0.016) {
                if ((keyHandler->pressedW() && !keyHandler->pressedS()) || (keyHandler->pressedS() && !keyHandler->pressedW())) {
                    if (keyHandler->pressedW()) {
                        // Comando de movimentação para frente
                        Ogre::Vector3 cameraDirection = player->getPlayerCamera()->getRealDirection();
                        physics->getOgreWorld()->rayTest(Ogre::Ray(player->getPlayerNode()->getPosition(), player->getPlayerCamera()->getRealDirection()), playerDirectionTester);
                        float distance = playerDirectionTester->getDistanceInDirection();
//                        std::cout << distance << std::endl;
                        if (distance > player->getPlayerEntity()->getBoundingRadius()) {
                            cameraDirection.normalise();
                            player->translate(cameraDirection);
                        }
                    }
                    else {
                        // Comando de movimentação para trás
                        Ogre::Vector3 cameraDirection = player->getPlayerCamera()->getRealDirection();
                        physics->getOgreWorld()->rayTest(Ogre::Ray(player->getPlayerNode()->getPosition(), player->getPlayerCamera()->getRealDirection() * -1), playerDirectionTester);
                        float distance = playerDirectionTester->getDistanceInDirection();
                        if (distance > player->getPlayerEntity()->getBoundingRadius()) {
                            cameraDirection.normalise();
                            player->translate(cameraDirection * -1);
                        }
                    }
                }

                if (keyHandler->pressedG()) {
//                    std::cout << playerDirectionTester->getDistanceInDirection() << std::endl;

                }

                if (!keyHandler->pressedAlt()) {
                    Ogre::Vector2 mouseMovement = this->keyHandler->mouseMovementRel();
                    this->player->getPlayerCamera()->getParentNode()->yaw(Ogre::Radian(mouseMovement.x * 0.005), Ogre::Node::TS_WORLD);
                    this->player->getPlayerCamera()->getParentNode()->pitch(Ogre::Radian(mouseMovement.y * 0.005));
                    this->keyHandler->mouseMovementReset();
                    this->trays->hideCursor();
                }
                else {
                    this->trays->showCursor();
                    Ogre::Vector2 mouseMovement = this->keyHandler->mouseMovementRel();
                    OgreBites::MouseMotionEvent* cursorMovement = new OgreBites::MouseMotionEvent();
                    cursorMovement->x = this->cursorPosition.x;
                    cursorMovement->y = this->cursorPosition.y;
                    cursorMovement->xrel = mouseMovement.x;
                    cursorMovement->yrel = mouseMovement.y;
                    cursorMovement->type = OgreBites::EventType::MOUSEMOTION;

                    // Utilizando janela 1, pode causar problemas futuramente
                    cursorMovement->windowID = 1;

                    this->trays->mouseMoved(*cursorMovement);
                    this->cursorPosition -= mouseMovement;
                    this->keyHandler->mouseMovementReset();
                    delete cursorMovement;
                    if (this->keyHandler->pressedD()) {
                        // fire ray
                        Ogre::Ray ray;
                        // ray = mCamera->getCameraToViewportRay(0.5, 0.5);
                        ray = this->trays->getCursorRay(this->player->getPlayerCamera());

                        Ogre::TerrainGroup::RayResult rayResult = this->terrainGroup->rayIntersects(ray);

                        if (rayResult.hit)
                        {
                            std::cout << rayResult.position << std::endl;
                        }
                    }
                    
                    
                }

                physics->getWorld()->stepSimulation(0.166);
                tick = 0;
                trays->refreshCursor();

            }
            return true;
        }

    private:
        playerRay* playerDirectionTester;
        Physics* physics;
        btRigidBody* playerBody = nullptr;
        KeyHandler* keyHandler;
        Player* player;
        Ogre::Vector2 cursorPosition = Ogre::Vector2::ZERO;
        Ogre::Real tick = 0;
        Ogre::Real tickSpeed = 1.0 / 60.0;
        OgreBites::TrayManager* trays;
        Ogre::TerrainGroup* terrainGroup;
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
        //    std::cout << world->getDispatcher() << std::endl;
        for (int i = 0; i < numManifolds; i++)
        {
            btPersistentManifold* manifold = manifolds[i];

            for (int j = 0; j < manifold->getNumContacts(); j++)
            {
                const btManifoldPoint& mp = manifold->getContactPoint(i);
                auto body0 = static_cast<EntityCollisionListener*>(manifold->getBody0()->getUserPointer());
                auto body1 = static_cast<EntityCollisionListener*>(manifold->getBody1()->getUserPointer());
                if (body0 != 0 && body0->listener)
                    body0->listener->contact(body1 == 0 ? nullptr : body1->entity, mp);
                if (body1 != 0 && body1->listener)
                    body1->listener->contact(body0 == 0 ? nullptr : body0->entity, mp);
            }
        }
    }

    // União entre os objetos de classes principais
    class Controllers {

    public:
        // Construtor Principal
        Controllers(OgreBites::ApplicationContext* application, Ogre::SceneManager* scene, 
            Ogre::Camera* playerCamera, Ogre::SceneNode* playerNode, Ogre::Entity* playerEntity, 
            bool autoFill, Ogre::TerrainGroup* terrainGroup = nullptr) {

            scene->addRenderQueueListener(application->getOverlaySystem());

            this->physicController = new Physics();
            btRigidBody* playerBody = this->addCollisionBodyInNode(0, playerEntity, Ogre::Bullet::CT_SPHERE, new playerCollision(playerEntity));
            physicController->getWorld()->setInternalTickCallback(localTick);
            this->playerInstance = new Player(playerCamera, playerNode, playerEntity, playerBody);

            this->application = application;
            this->trays = new OgreBites::TrayManager("Tray Controller", this->application->getRenderWindow());

            this->inputController = new KeyHandler();
            this->trays->hideCursor();
            instaceTrays(); 

            this->frameController = new Updater(inputController, playerInstance, physicController, this->trays, terrainGroup);

        }

        Player* getPlayer(){
            return this->playerInstance;
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

        btCollisionObject* addCollisionObjectInNode(Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl = nullptr, int group = 1, int mask = -1) {
            return this->physicController->addCollisionObjectInNode(ent, ct, cl, group, mask);
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
        OgreBites::ApplicationContext* application;
        OgreBites::TrayManager* trays;

        void instaceTrays(){
            this->trays->createButton(OgreBites::TrayLocation::TL_CENTER, "botao", "press");
        }
    };

}
// namespace MyEngine
#endif
