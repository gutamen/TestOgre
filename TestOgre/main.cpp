#include "Ogre.h"
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPaging.h>
#include "OgreApplicationContext.h"
#include "OgrePageManager.h"
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

class DummyPageProvider : public Ogre::PageProvider
{
public:
    bool prepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
    bool loadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
    bool unloadProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
    bool unprepareProceduralPage(Ogre::Page* page, Ogre::PagedWorldSection* section) { return true; }
};
DummyPageProvider mDummyPageProvider;


int main(int argc, char* argv[])
{

//! [constructor]
    OgreBites::ApplicationContext ctx("Game Test");
    ctx.initApp();
    ctx.loadResources();
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
//! [constructor]
    
//! [setup]
    // get a pointer to the already created root
    
    // Objetos de Terreno
    Ogre::TerrainGlobalOptions* terrainGlobals = nullptr;
    Ogre::PageManager* pageManager;
    Ogre::TerrainGroup* principalTerrainGroup = nullptr;
    Ogre::TerrainPaging* terrainPaging = nullptr;

    Ogre::Root* root = ctx.getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);
    

    Ogre::SceneNode* attachmentNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    //carregamento da cena
    attachmentNode->loadChildren("untitled.scene");

    // Início do terreno
    terrainGlobals = new Ogre::TerrainGlobalOptions();
    terrainGlobals->setMaxPixelError(8);
    terrainGlobals->setCompositeMapSize(3000);

    principalTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
        scnMgr,                     // SceneManager
        Ogre::Terrain::ALIGN_X_Z,   // Orientação (normalmente XZ)
        513,                        // Quantos pontos tem em cada lado (513 é comum)
        12000.0f                     // Tamanho em metros no mundo
    );
    principalTerrainGroup->setOrigin(Ogre::Vector3(0, 0, 0));  

    // Heightmap do terreno
    Ogre::Image heightMap;
    try {
        heightMap.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        std::cout << "[OK] Heightmap carregado.\n";
    }
    catch (Ogre::Exception& e) {
        std::cout << "[ERRO] Falha ao carregar terrain.png: " << e.getFullDescription() << std::endl;
    }
    

    Ogre::Terrain::ImportData& importSettings = principalTerrainGroup->getDefaultImportSettings();
    importSettings.inputScale = 600.0f;     // força da altura
    importSettings.minBatchSize = 33;
    importSettings.maxBatchSize = 65;

    // ==================== TEXTURAS ====================
    importSettings.layerList.resize(1);        // 1 camada

    // Camada 0 (base)
    importSettings.layerList[0].worldSize = 200.0f;  // quanto a textura se repete no mundo

    // Textura difusa + especular
    importSettings.layerList[0].textureNames.push_back("test_diffspec.dds");

    // Textura Normal + Height (MUITO IMPORTANTE!)
    importSettings.layerList[0].textureNames.push_back("test_normheight.dds");

    //! [light]
    Ogre::Light* l = scnMgr->createLight();
    l->setType(Ogre::Light::LT_DIRECTIONAL);
    l->setDiffuseColour(Ogre::ColourValue::White);
    l->setSpecularColour(Ogre::ColourValue(0.6, 0.6, 0.6));

    Ogre::SceneNode* ln = scnMgr->getRootSceneNode()->createChildSceneNode();
    ln->setDirection(Ogre::Vector3(0.55, -0.3, 0.75).normalisedCopy());
    ln->setPosition(10.0, 3, 0);
    ln->attachObject(l);
    //! [light]
    scnMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
    principalTerrainGroup->defineTerrain(0, 0, &heightMap);
    pageManager = OGRE_NEW Ogre::PageManager();

    

    Ogre::SceneManager::CameraList cameras = scnMgr->getCameras();
    Ogre::Camera* camera = cameras.at("Camera");
    camera->setAutoAspectRatio(true);
    
    // Paginação
    pageManager->setPageProvider(&mDummyPageProvider);
    pageManager->addCamera(camera);
    terrainPaging = OGRE_NEW Ogre::TerrainPaging(pageManager);
    Ogre::PagedWorld* world = pageManager->createWorld();
    terrainPaging->createWorldSection(world, principalTerrainGroup, 2000, 3000, 0, 0, 0, 0);

    // Carregar o terreno
    principalTerrainGroup->loadAllTerrains(true);   // true = síncrono (espera carregar)
    //principalTerrainGroup->freeTemporaryResources();

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

    MyEngine::Controllers* controller = new MyEngine::Controllers(&ctx, scnMgr, camera, node, ent, true);
//    MyEngine::Physics* fisic = controller->getPhysicsController();
//    btRigidBody* playerBody = controller->getPlayerBody();
    controller->addCollisionObjectInNode(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE);
        
    root->addFrameListener(controller->getFrameController());
    ctx.addInputListener(controller->getInputController());

//    ctx.setup();
    ctx.setWindowGrab();
    ctx.getRoot()->startRendering();
    

    ctx.closeApp();
//! [main]

    return 0;

}

