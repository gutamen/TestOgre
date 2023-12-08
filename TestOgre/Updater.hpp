#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btTransform.h>
#include <LinearMath/btVector3.h>
#include <Ogre.h>
#include <OgreFrameListener.h>
#include "KeyHandler.hpp"
#include <iostream>
#include <OgreBullet.h>
#include "Physics.hpp"

using namespace std;
using namespace Ogre;

class Updater : public FrameListener
{
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

    bool frameStarted(const FrameEvent& frameRendered) override {
        
        tick += frameRendered.timeSinceLastFrame;
        if(tick >= 0.016){

            if(keyHandler->pressedW()){
                player->getPlayerNode()->translate(player->getPlayerCamera()->getRealDirection());
                btVector3 teste = convert(player->getPlayerNode()->getPosition());
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
    Real tick = 0;
    Real tickSpeed = 1.0/60.0;
};

