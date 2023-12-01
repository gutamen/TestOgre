#include <BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <OgreBullet.h>
#include <OgreEntity.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <iostream>

using namespace Ogre::Bullet;
using namespace Ogre;
using namespace std;

class Physics {

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
    Physics(){
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

    btRigidBody* addCollisionBodyInNode(float mass, Entity* ent, ColliderType ct, CollisionListener* cl = nullptr, int group = 1, int mask = -1){
        return this->ogreAdapter->addRigidBody(mass, ent, ct, cl, group, mask);
    }


};

struct ContactTestCallback: public btCollisionWorld::ContactResultCallback
{
    btScalar addSingleResult(btManifoldPoint& cp,
        const btCollisionObjectWrapper* colObj0Wrap,
        int partId0,
        int index0,
        const btCollisionObjectWrapper* colObj1Wrap,
        int partId1,
        int index1){
       
        cout << "teste" << endl;
        return 1;

        // your callback code here
    }
};


