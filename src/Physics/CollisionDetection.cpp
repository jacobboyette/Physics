/*
Modification based on the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "CollisionDetection.h"

bool CollisionDetection::isColliding(Body* a, Body* b, Contact& contact) {
    bool aIsDisk = a->shape->getType() == DISK;
    bool bIsDisk = b->shape->getType() == DISK;

    if (aIsDisk && bIsDisk) {
        return isCollidingDiskDisk(a, b, contact);
    }

    return false;
}

bool CollisionDetection::isCollidingDiskDisk(Body* a, Body* b, Contact& contact) {
    Disk* diskA = (Disk*) a->shape;
    Disk* diskB = (Disk*) b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = diskA->radius + diskB->radius;

    bool isColliding = ab.magnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) {
        return false;
    }

    contact.a = a;
    contact.b = b;

    contact.normal = ab;
    contact.normal.normalize();

    contact.start = b->position - contact.normal * diskB->radius;
    contact.end = a->position + contact.normal * diskA->radius;

    contact.depth = (contact.end - contact.start).magnitude();
    
    return true;
}

// TODO: Implement other methods to check collision between different shapes
