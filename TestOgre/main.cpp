// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <LinearMath/btVector3.h>
#include <OgreNode.h>
#include <OgrePrerequisites.h>
#include <iostream>
#include <fstream>
#include "OgreMesh.h"
#include "OgreSceneManager.h"
#include <OgreTrays.h>
#include <OgreBullet.h>
#include "Physics.hpp"

using namespace std;

class playerCollision : public btCollisionWorld::ContactResultCallback
{
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1){
        btVector3 ptA = cp.getPositionWorldOnA();
        btVector3 ptB = cp.getPositionWorldOnB();
        return 0;
    }
};

class frame : public Ogre::FrameListener
{
    public:
        frame() {

        }


};

class collide : public Ogre::Bullet::CollisionListener
{
    public:
        collide(Ogre::MovableObject* in) {

            object = in;
        }

    void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) {
        cout << "tocou" << endl;
    }

    Ogre::MovableObject* object;
};



//! [key_handler]
class KeyHandler : public OgreBites::InputListener
{

    public:
    KeyHandler(Ogre::SceneManager* scene)
    {
        // Obtém o SceneManager associado à janela de renderização
        mSceneManager = scene;
        mCamera = scene->getCamera("Camera");
        
    }
    

    void frameRendered(const Ogre::FrameEvent& evt) override{
        timer += evt.timeSinceLastFrame;
        //cout << timer << endl;
        
        
        if (timer >= 0.0166) {
            //fisica.dynamicsWorld->performDiscreteCollisionDetection();
            //cout << fisica.dispatcher->getNumManifolds() << endl;
            if (front) {
                mCamera->getParentNode()->translate(mCamera->getRealDirection()); 
            }

            if(rear) {
                mCamera->getParentNode()->translate(mCamera->getRealDirection()*-1);
            }

            timer = 0;
        }

    }

    bool keyReleased(const OgreBites::KeyboardEvent& evt) override{
        switch (evt.keysym.sym){
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

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override{
        switch (evt.keysym.sym){
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
            mSceneManager->getEntity("Suzanne")->getParentNode()->translate(Ogre::Vector3(0,1,0));
            break;
        
        }

        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            
            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const OgreBites::MouseMotionEvent &evt) override{ 
        mCamera->getParentNode()->yaw(Ogre::Radian(-evt.xrel * 0.005), Ogre::Node::TS_WORLD);
        mCamera->getParentNode()->pitch(Ogre::Radian(-evt.yrel * 0.005));
        return true;
    }

    private:
        Ogre::Node* playerNode;
        Ogre::Real MoveSpeed;
        Ogre::Real timer = 0;
        bool rear = false;
        bool front = false;
        Ogre::SceneManager* mSceneManager;
        Ogre::Camera* mCamera;

    //public:
        //Physics fisica;
};


//! [key_handler]

int main(int argc, char* argv[])
{

    //! [constructor]
    OgreBites::ApplicationContext ctx("OgreTutorialApp");
    ctx.initApp();
    //! [constructor]

    //! [setup]
        // get a pointer to the already created root
    Ogre::Root* root = ctx.getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);


    Ogre::SceneNode* attachmentNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    attachmentNode->loadChildren("untitled.scene");
    
    // without light we would just get a black screen    
    //Ogre::Light* light = scnMgr->createLight("MainLight");
    //Ogre::SceneNode* lightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    //lightNode->setPosition(0, 10, 0);
    //lightNode->attachObject(light);

    Ogre::SceneManager::CameraList cameras = scnMgr->getCameras();
    Ogre::Camera* camera = cameras.at("Camera");
    camera->setAutoAspectRatio(true);
    
    //cout << camera->getName();

    //scnMgr->getEntity("Suzanne")->getParentNode()->setPosition(1, 1, 1);

    
    
    //ctx.getRenderWindow()->addViewport(camera);

    // also need to tell where we are
    //Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    //camNode->setPosition(7.358891, 4.958309, 6.925791);
    //camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

    // create the camera
    //Ogre::Camera* cam = scnMgr->createCamera("myCam");
    //cam->setNearClipDistance(5); // specific to this sample
    //cam->setAutoAspectRatio(true);
    //camNode->attachObject(cam);


    // and tell it to render into the main window
    //cout << scnMgr->getCameras().size() << endl;
    //cout << scnMgr->getCameras().at("myCam")->getParentNode()->getOrientation() << endl;
    //cout << camera->getParentNode()->getOrientation() << endl;

    //camera->getParentNode()->setOrientation(0.912, -0.228, 0.338, 0);

    

    
    

    ctx.getRenderWindow()->addViewport(camera);

    // finally something to render
    Ogre::Entity* ent = scnMgr->createEntity("Suzanne.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode(camera->getDerivedPosition());
    node->attachObject(ent);
    camera->detachFromParent();
    node->attachObject(camera);
    //! [setup]
    
   

    //! [main]
        // register for input events
    
    
    Physics* fisic = new Physics();
    
    
    Ogre::Bullet::DynamicsWorld* colider = new Ogre::Bullet::DynamicsWorld(fisic->dynamicsWorld);
     
    colider->addCollisionObject(ent, Ogre::Bullet::CT_SPHERE)->getWorldTransform().setOrigin(Ogre::Bullet::convert(ent->getParentNode()->getPosition()));
    colider->addCollisionObject(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE)->getWorldTransform().setOrigin(Ogre::Bullet::convert(scnMgr->getEntity("Suzanne")->getParentNode()->getPosition()));
    btVector3 body0 = fisic->dynamicsWorld->getCollisionObjectArray().at(0)->getWorldTransform().getOrigin();
    btVector3 body1 = fisic->dynamicsWorld->getCollisionObjectArray().at(1)->getWorldTransform().getOrigin();
    
    cout << body0.x() << " " << body0.y() << " " << body0.z()  << endl << body1.x() << " " << body1.y() << " " << body1.z() << endl;
    
    Ogre::Bullet::DebugDrawer* debug = new Ogre::Bullet::DebugDrawer(node, fisic->dynamicsWorld); 

    //cout << body0.getX() << " " << body0.getY() << " " << body0.getZ() << endl;
    //cout << body1.getX() << " " << body1.getY() << " " << body1.getZ() << endl;
    

    //fisic.dynamicsWorld->getCollisionObjectArray().at(0)->getWorldTransform();

    //cout << fisic.dynamicsWorld->getNumCollisionObjects() << endl;
    
    Ogre::RenderWindow* tela = ctx.getRenderWindow();
    OgreBites::TrayManager* controlador = new OgreBites::TrayManager("Controlador", ctx.getRenderWindow());

    
    
    
    

    KeyHandler keyHandler(scnMgr);
    //keyHandler.fisica = fisic;

    ctx.addInputListener(&keyHandler);
    
    
    
    ctx.getRoot()->startRendering();
    

    

    ctx.closeApp();
    //! [main]
    return 0;

}
