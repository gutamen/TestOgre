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
#include "OgreTerrainMaterialGeneratorA.h"

using namespace std;

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
    Ogre::TerrainGroup* principalTerrainGroup = nullptr;
    Ogre::TerrainPaging* terrainPaging = nullptr;

    // === DEBUG: Liste todos os recursos encontrados ===
    /*std::cout << "\n=== RECURSOS ENCONTRADOS ===\n";
    Ogre::FileInfoListPtr files = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo("General", false);
    for (auto& f : *files) {
        std::cout << "   " << f.filename << " (" << f.path << ")\n";
    }
    std::cout << "==========================\n\n";*/

    Ogre::Root* root = ctx.getRoot();
    Ogre::SceneManager* scnMgr = root->createSceneManager();

    // register our scene with the RTSS
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED);
    shadergen->addSceneManager(scnMgr);
    

    Ogre::SceneNode* attachmentNode = scnMgr->getRootSceneNode()->createChildSceneNode();

    //carregamento da cena
    attachmentNode->loadChildren("untitled.scene");

    // Início do terreno
    terrainGlobals = new Ogre::TerrainGlobalOptions();
    terrainGlobals->setMaxPixelError(8);
    terrainGlobals->setCompositeMapSize(6000);

    principalTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
        scnMgr,                     // SceneManager
        Ogre::Terrain::ALIGN_X_Z,   // Orientação (normalmente XZ)
        513,                        // Quantos pontos tem em cada lado (513 é comum)
        12000.0f                     // Tamanho em metros no mundo
    );
    principalTerrainGroup->setOrigin(Ogre::Vector3(0.0, -500.0, 0.0));  

    // Heightmap do terreno
    Ogre::Image heightMap;
    try {
        heightMap.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        std::cout << "[OK] Heightmap carregado.\n";
    }
    catch (Ogre::Exception& e) {
        std::cout << "[ERRO] Falha ao carregar terrain.png: " << e.getFullDescription() << std::endl;
    }

    //! [linearFog]
    Ogre::ColourValue fadeColour(0.7, 0.7, 0.8);
    scnMgr->setFog(Ogre::FOG_LINEAR, fadeColour, 0, 2000, 10000);
    //! [linearFog]
    
    //! [light]
    Ogre::Light* sunLight = scnMgr->createLight("SunLight");
    sunLight->setType(Ogre::Light::LT_DIRECTIONAL);
    sunLight->setDiffuseColour(Ogre::ColourValue::White);   
    sunLight->setSpecularColour(Ogre::ColourValue(0.4f, 0.4f, 0.4f));

    Ogre::SceneNode* sunLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    sunLightNode->setDirection(Ogre::Vector3(0.55, -0.3, 0.75).normalisedCopy());       // direção do sol
    sunLightNode->attachObject(sunLight);
    
    scnMgr->setAmbientLight(Ogre::ColourValue(0.4, 0.4, 0.4));
    //! [light]
    
    
    // Important to set these so that the terrain knows what to use for baked (non-realtime) data
    terrainGlobals->setLightMapDirection(sunLight->getDerivedDirection());
    terrainGlobals->setCompositeMapAmbient(scnMgr->getAmbientLight());
    terrainGlobals->setCompositeMapDiffuse(sunLight->getDiffuseColour());
    

    Ogre::Terrain::ImportData& importSettings = principalTerrainGroup->getDefaultImportSettings();
    importSettings.inputScale = 800.0f;     // força da altura
    importSettings.minBatchSize = 33;
    importSettings.maxBatchSize = 65;

    // ==================== TEXTURAS ====================
    importSettings.layerList.resize(1);        // 1 camada

    // Camada 0 (base)
    importSettings.layerList[0].worldSize = 150.0f;  // quanto a textura se repete no mundo

    // Textura difusa + especular
    importSettings.layerList[0].textureNames.push_back("Ground37_diffspec.dds");

    // Textura Normal + Height (MUITO IMPORTANTE!)
    importSettings.layerList[0].textureNames.push_back("Ground37_normheight.dds");
    
    // Carregar o terreno
    principalTerrainGroup->defineTerrain(0, 0, &heightMap);
    principalTerrainGroup->loadAllTerrains(true);   // true = síncrono (espera carregar)
    //principalTerrainGroup->freeTemporaryResources();
    
    
    //! [camera]    
    Ogre::Camera* camera = scnMgr->createCamera("principalCamera");
    camera->setProjectionType(Ogre::PT_PERSPECTIVE);
    camera->setFOVy(Ogre::Radian(0.45));
    camera->setNearClipDistance(1);
    camera->setFarClipDistance(0);
    camera->setAutoAspectRatio(true);
 
    Ogre::SceneNode* cameraNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    cameraNode->setPosition(7, 10, 7);
    cameraNode->setOrientation(0.912, -0.228, 0.338, 0);
    cameraNode->attachObject(camera);
    
    ctx.getRenderWindow()->addViewport(camera);
    //! [camera]
    
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

    MyEngine::Controllers* controller = new MyEngine::Controllers(&ctx, scnMgr, camera, node, ent, true, principalTerrainGroup);
//    MyEngine::Physics* fisic = controller->getPhysicsController();
//    btRigidBody* playerBody = controller->getPlayerBody();
    controller->addCollisionObjectInNode(scnMgr->getEntity("Suzanne"), Ogre::Bullet::CT_SPHERE);
        
    root->addFrameListener(controller->getFrameController());
    ctx.addInputListener(controller->getInputController());

    ctx.setWindowGrab();
    ctx.getRoot()->startRendering();
    


    // Fechamento de programa
    if (principalTerrainGroup)
    {
        
        principalTerrainGroup->freeTemporaryResources();

        // Opcional: deleta o grupo
        OGRE_DELETE principalTerrainGroup;
        principalTerrainGroup = nullptr;
    }

    if (terrainGlobals)
    {
        OGRE_DELETE terrainGlobals;
    }

    ctx.closeApp();
//! [main]

    return 0;

}

