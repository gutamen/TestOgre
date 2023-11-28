#include <Ogre.h>
#include <OgreApplicationContext.h>

using namespace std;
using namespace OgreBites;
using namespace Ogre;

class KeyHandler : public InputListener
{

public:
    KeyHandler(SceneManager* scene)
    {
        // Obtém o SceneManager associado à janela de renderização
        mSceneManager = scene;
        mCamera = scene->getCamera("Camera");

    }


    void frameRendered(const FrameEvent& evt) override {
        timer += evt.timeSinceLastFrame;
        //cout << timer << endl;


        if (timer >= 0.0166) {
            //fisica.dynamicsWorld->performDiscreteCollisionDetection();
            //cout << fisica.dispatcher->getNumManifolds() << endl;
            if (front) {
                mCamera->getParentNode()->translate(mCamera->getRealDirection());
            }

            if (rear) {
                mCamera->getParentNode()->translate(mCamera->getRealDirection() * -1);
            }

            timer = 0;
        }

    }

    bool keyReleased(const KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 119:
            // Move o objeto para cima
            front = false;
            //mNode->translate(direction);
            break;

        case 115:
            rear = false;
            break;
        }


        return true;
    }

    bool keyPressed(const KeyboardEvent& evt) override {
        switch (evt.keysym.sym) {
        case 119:
            // Move o objeto para cima
            front = true;
            //mNode->translate(direction);
            break;
            /*case 97:
                // Move o objeto para a esquerda
                mNode->translate(-1.0f, 0.0f, 0.0f);
                break;
            */
        case 115:
            // Move o objeto para baixo
            rear = true;
            //mNode->translate(direction*-1);
            break;
        case 100:
            // Move o objeto para a direita
            mSceneManager->getEntity("Suzanne")->getParentNode()->translate(Vector3(0, 1, 0));
            break;

        }

        if (evt.keysym.sym == SDLK_ESCAPE)
        {

            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const MouseMotionEvent& evt) override {
        mCamera->getParentNode()->yaw(Radian(-evt.xrel * 0.005), Node::TS_WORLD);
        mCamera->getParentNode()->pitch(Radian(-evt.yrel * 0.005));
        return true;
    }

private:
    Node* playerNode;
    Real MoveSpeed;
    Real timer = 0;
    bool rear = false;
    bool front = false;
    SceneManager* mSceneManager;
    Camera* mCamera;
    
    //public:
        //Physics fisica;
};