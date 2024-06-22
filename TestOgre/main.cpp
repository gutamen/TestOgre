// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btVector3.h>
#include <OgreNode.h>
#include <OgrePrerequisites.h>
#include <iostream>
#include <fstream>
#include "OgreMesh.h"
#include "OgreSceneManager.h"
#include <OgreTrays.h>
#include <OgreBullet.h>
#include "myEngine.hpp"

using namespace std;

class playerCollision : public Ogre::Bullet::CollisionListener
{
public:
    playerCollision() {

    }

    void contact(const Ogre::MovableObject* other, const btManifoldPoint& manifoldPoint) override {
        cout << "teste" << endl;
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

    playerCollision* teste = new playerCollision();
    Controllers* controller = new Controllers(scnMgr, camera, node, ent);
    Physics* fisic = controller->getPhysicsController();
    btRigidBody* playerBody = controller->addCollisionBodyInNode(0, ent, Ogre::Bullet::CT_SPHERE, teste);
    controller->setPlayerFisicBody(playerBody);

    cout << playerBody << endl << fisic->getCollisionObjects().at(0) << endl;
    cout << controller->getPlayerBody() << endl;

    controller->addCollisionObjectInNode(ent, Ogre::Bullet::CT_SPHERE);
    controller->addCollisionObjectInNode(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE);


//    btVector3 body0 = fisic->getWorld()->getCollisionObjectArray().at(0)->getWorldTransform().getOrigin();
//    btVector3 body1 = playerBody->getWorldTransform().getOrigin();

//    cout << body0.x() << " " << body0.y() << " " << body0.z() << endl;
//    cout << body1.x() << " " << body1.y() << " " << body1.z() << endl;


    Ogre::RenderWindow* tela = ctx.getRenderWindow();
    //OgreBites::TrayManager* controlador = new OgreBites::TrayManager("Controlador", ctx.getRenderWindow());


    root->addFrameListener(controller->getFrameController());
    ctx.addInputListener(controller->getInputController());



    ctx.getRoot()->startRendering();


    ctx.closeApp();
    //! [main]

    return 0;

}