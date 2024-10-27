/*
Source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef CONTACT_H
#define CONTACT_H

#include "Vec2.h"
#include "Body.h"

struct Contact {
    Body* a;
    Body* b;

    Vec2 start;
    Vec2 end;

    Vec2 normal;
    float depth;

    Contact() = default;
    ~Contact() = default;

    void resolvePenetration();	// the projection method
	void resolveCollision();	// the projection method plus the impulse method 
};

#endif
