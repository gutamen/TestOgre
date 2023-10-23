// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include "Ogre.h"
#include "OgreApplicationContext.h"
#include <iostream>
#include <fstream>
#include "OgreMesh.h"
#include "OgreSceneManager.h"
#include <iostream>
#include <OgreTrays.h>
#include <Windows.h>
#include <OgreBullet.h>


using namespace std;






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
    

    bool keyPressed(const OgreBites::KeyboardEvent& evt) override
    {

        Ogre::Node* mNode = mCamera->getParentNode();
        
        Ogre::Quaternion orientation = mNode->getOrientation();

        Ogre::Vector3 direction = mCamera->getRealDirection();
        
        //direction.normalise();
        //cout << direction << endl;

        //cout << evt.keysym.sym << endl;
        
        // Verifica qual tecla foi pressionada
        switch (evt.keysym.sym)
        {
        case 119:
            // Move o objeto para cima

            mNode->translate(direction);
            break;
        /*case 97:
            // Move o objeto para a esquerda
            mNode->translate(-1.0f, 0.0f, 0.0f);
            break;
        */
        case 115:
            // Move o objeto para baixo
            mNode->translate(direction*-1);
            break;
        /*case 100:
            // Move o objeto para a direita
            mNode->translate(1.0f, 0.0f, 0.0f);
            break;
        */
        }

        if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
        {
            
            Ogre::Root::getSingleton().queueEndRendering();
        }
        return true;
    }

    bool mouseMoved(const OgreBites::MouseMotionEvent &evt) 
    { 
        mCamera->getParentNode()->yaw(Ogre::Radian(-evt.xrel * 0.005), Ogre::Node::TS_WORLD);
        mCamera->getParentNode()->pitch(Ogre::Radian(-evt.yrel * 0.005));
        
        return true;
    }

    private: 
        Ogre::SceneManager* mSceneManager;
        Ogre::Camera* mCamera;
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

    Ogre::Bullet::createCylinderCollider(camera);

    ctx.getRenderWindow()->addViewport(camera);

    // finally something to render
    //Ogre::Entity* ent = scnMgr->createEntity("strongknight.mesh");
    //Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
    //node->attachObject(ent);
    

    //! [setup]
    
   

    //! [main]
        // register for input events
    
    
    Ogre::RenderWindow* tela = ctx.getRenderWindow();
    OgreBites::TrayManager* controlador = new OgreBites::TrayManager("Controlador", ctx.getRenderWindow());

    
    //cout << tela->getMetrics << endl;

    //controlador->hideCursor();
    

    KeyHandler keyHandler(scnMgr);
    ctx.addInputListener(&keyHandler);
    
    
    
    ctx.getRoot()->startRendering();
    

    

    ctx.closeApp();
    //! [main]
    return 0;

}
