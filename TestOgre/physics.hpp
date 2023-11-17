#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <OgreBullet.h>
#include <OgreEntity.h>

using namespace Ogre::Bullet;
using namespace Ogre;

class physics {
public:
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    std::vector<btCollisionShape*> collisionShapes;
    std::map<std::string, btRigidBody*> physicsAccessors;
    DynamicsWorld* ogreAdapter;


    
public:
    physics(){
        collisionConfiguration = new btDefaultCollisionConfiguration();
        dispatcher = new btCollisionDispatcher(collisionConfiguration);
        overlappingPairCache = new btDbvtBroadphase();
        solver = new btSequentialImpulseConstraintSolver();
        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        ogreAdapter = new DynamicsWorld(dynamicsWorld);
    }

public:
    btCollisionObject* addCollisionObjectInNode(Entity *ent, ColliderType ct, int group = 1, int mask = -1){
        btCollisionObject* object = this->ogreAdapter->addCollisionObject(ent, ct, group, mask);
        object->getWorldTransform().setOrigin(convert(ent->getParentNode()->getPosition()));
        return object;
    }

};
