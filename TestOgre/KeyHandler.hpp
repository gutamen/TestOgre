#include <Ogre.h>
#include <OgreApplicationContext.h>
#include <OgreSceneManager.h>
#include "Player.hpp"
using namespace std;
using namespace OgreBites;
using namespace Ogre;

class KeyHandler : public InputListener
{

public:
    KeyHandler(SceneManager* sceneManager)
    {
        // Obtém o SceneManager associado à janela de renderização
        this->sceneManager = sceneManager;
        this->playerCamera = sceneManager->getCamera("Camera");

    }

    KeyHandler(SceneManager* sceneManager, Player* player){
        this->sceneManager = sceneManager;
        this->player = player;
        this->playerCamera = player->getPlayerCamera();
    }


    /*void frameRendered(const FrameEvent& evt) override {
        timer += evt.timeSinceLastFrame;

        if (timer >= 0.0166) {
            //fisica.dynamicsWorld->performDiscreteCollisionDetection();
            //cout << fisica.dispatcher->getNumManifolds() << endl;
            if (wIsPressed) {
                //playerCamera->getParentNode()->translate(playerCamera->getRealDirection());
            }

            if (sIsPressed) {
                //playerCamera->getParentNode()->translate(playerCamera->getRealDirection() * -1);
            }

            timer = 0;
        }

    }*/

    bool keyReleased(const KeyboardEvent& evt) override {
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

    bool keyPressed(const KeyboardEvent& evt) override {
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
            sceneManager->getEntity("Suzanne")->getParentNode()->translate(Vector3(0, 1, 0));
            break;

        }

        if (evt.keysym.sym == SDLK_ESCAPE)
        {

            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const MouseMotionEvent& evt) override {
        playerCamera->getParentNode()->yaw(Radian(-evt.xrel * 0.005), Node::TS_WORLD);
        playerCamera->getParentNode()->pitch(Radian(-evt.yrel * 0.005));
        return true;
    }

    bool pressedW(){
        return this->wIsPressed;
    }

    bool pressedS(){
        return this->sIsPressed;
    }

private:
    Node* playerNode;
    Real MoveSpeed;
    Real timer = 0;
    Player* player;
    bool sIsPressed =  false;
    bool wIsPressed = false;
    SceneManager* sceneManager;
    Camera* playerCamera;
    
    //public:
        //Physics fisica;
};
