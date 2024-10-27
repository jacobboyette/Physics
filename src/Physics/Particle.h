/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle {
    //int radius;
	float radius;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    Vec2 sumForces;

    float mass;
    float invMass;

    Particle(float x, float y, float mass);
	Particle(float x, float y, float mass, float radius);
    ~Particle();

    void addForce(const Vec2& force);
    void clearForces();

    void integrate(float dt);
	void integrate(float dt, Vec2 initAccelaration);	// the initial accelaration such as the gravity is involved
};

#endif
