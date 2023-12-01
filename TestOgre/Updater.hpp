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
    }

    bool frameStarted(const FrameEvent& frameRendered) override {
        
        tick += frameRendered.timeSinceLastFrame;
        if(tick >= 0.016){
            tick = 0;
        }
        return true;
    }

private:
    Physics* physics;
    KeyHandler* keyHandler;
    Player* player;
    Real tick = 0;
    Real tickSpeed = 1.0/60.0;
};

