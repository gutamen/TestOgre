#include "myEngine.hpp"

namespace MyEngine {

    btCollisionObject* Physics::addCollisionObjectInNode(Ogre::Entity* ent, Ogre::Bullet::ColliderType ct, Ogre::Bullet::CollisionListener* cl, int group, int mask) {
        btCollisionObject* object = this->ogreAdapter->addCollisionObject(ent, ct, group, mask);
        object->setUserPointer(new EntityCollisionListener{ ent, cl });
        object->getWorldTransform().setOrigin(Ogre::Bullet::convert(ent->getParentNode()->getPosition()));
        return object;
    }

    

}
// namespace MyEngine
