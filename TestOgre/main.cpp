// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btVector3.h>
#include <OgreNode.h>
#include <OgreOverlaySystem.h>
#include <OgrePrerequisites.h>
#include <OgreResourceGroupManager.h>
#include <OgreResourceManager.h>
#include <iostream>
#include <fstream>
#include "OgreMesh.h"
#include "OgreSceneManager.h"
#include <OgreTrays.h>
#include <OgreBullet.h>
#include "myEngine.hpp"

using namespace std;


int main(int argc, char* argv[])
{

//! [constructor]
    OgreBites::ApplicationContext ctx("OgreTutorialApp");
    ctx.initApp();
    ctx.loadResources();
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
    Ogre::Entity* ent = scnMgr->createEntity("player", "Suzanne.mesh");
    Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode(camera->getDerivedPosition());
    node->attachObject(ent);
    camera->detachFromParent();
    node->attachObject(camera);
//! [setup]

//    node->setPosition(scnMgr->getEntity("Suzanne")->getParentNode()->getPosition() + Ogre::Vector3(0, 3.5, 0));


//! [main]
    // register for input events

    MyEngine::Controllers* controller = new MyEngine::Controllers(ctx, scnMgr, camera, node, ent, true);
//    MyEngine::Physics* fisic = controller->getPhysicsController();
//    btRigidBody* playerBody = controller->getPlayerBody();

    controller->addCollisionObjectInNode(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE);
        
    root->addFrameListener(controller->getFrameController());
    ctx.addInputListener(controller->getInputController());

    ctx.getRoot()->startRendering();
    
    std::cout << "teste" << std::endl;

    ctx.closeApp();
//! [main]

    return 0;

}
