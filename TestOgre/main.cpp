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
#include "Controllers.hpp"

using namespace std;

class playerCollision : public btCollisionWorld::ContactResultCallback
{
    btScalar addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1){
        btVector3 ptA = cp.getPositionWorldOnA();
        btVector3 ptB = cp.getPositionWorldOnB();
        return 0;
    }
};




//! [key_handler]



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
    //btVector3 body0 = fisic->ogreAdapter->addRigidBody(0, ent, Ogre::Bullet::CT_SPHERE)->getWorldTransform().getOrigin(); 
    fisic->addCollisionObjectInNode(ent, Ogre::Bullet::CT_SPHERE);
    fisic->addCollisionObjectInNode(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE);    

    btVector3 body0 = fisic->dynamicsWorld->getCollisionObjectArray().at(0)->getWorldTransform().getOrigin();
    btVector3 body1 = fisic->dynamicsWorld->getCollisionObjectArray().at(1)->getWorldTransform().getOrigin();
    
    cout << body0.x() << " " << body0.y() << " " << body0.z()  << endl << body1.x() << " " << body1.y() << " " << body1.z() << endl;
    
    Ogre::Bullet::DebugDrawer* debug = new Ogre::Bullet::DebugDrawer(node, fisic->dynamicsWorld); 

    //cout << fisic.dynamicsWorld->getNumCollisionObjects() << endl;
    
    Ogre::RenderWindow* tela = ctx.getRenderWindow();
    OgreBites::TrayManager* controlador = new OgreBites::TrayManager("Controlador", ctx.getRenderWindow());



    Controllers* controller = new Controllers(scnMgr, camera, node, ent);
    
    root->addFrameListener(controller->getFrameController());
    ctx.addInputListener(controller->getInputController());
    
    
    
    ctx.getRoot()->startRendering();
    

    

    ctx.closeApp();
    //! [main]
    return 0;

}
