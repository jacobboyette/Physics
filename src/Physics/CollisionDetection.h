/*
Source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "Body.h"
#include "Contact.h"

struct CollisionDetection {
    static bool isColliding(Body* a, Body* b, Contact& contact);
    static bool isCollidingDiskDisk(Body* a, Body* b, Contact& contact);
    // TODO: static bool IsCollidingPolygonPolygon(Body* a, Body* b);
    // TODO: static bool IsCollidingPolygonCircle(Body* a, Body* b);
};

#endif
