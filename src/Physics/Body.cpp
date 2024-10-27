/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorques = 0.0;
	this->restitution = 1.0;	// set the elasticity of the body
    this->mass = mass;
    if (mass > 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;	// a static body has 0 invMass
    }
    I = shape.getMomentOfInertia() * mass;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    } else {
        this->invI = 0.0;
    }
    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body() {
    delete shape;
    std::cout << "Body destructor called!" << std::endl;
}

//---------- to create a static body, just set its mass to a negative value ------------
bool Body::isStatic() const {
	if (mass < 0.0)
		return true;
	else
		return false;
}

void Body::addForce(const Vec2& force) {
    sumForces += force;
}

void Body::addTorque(float torque) {
    sumTorques += torque;
}

void Body::clearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::clearTorques() {
    sumTorques = 0.0;
}

void Body::integrateLinear(float dt) {
	if (isStatic())		// intergration is not for static bodies
		return;

    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the new position
    position += velocity * dt;

    // Clear all the forces acting on the object before the next physics step
    clearForces();
}

//--------- the first body applies J, and the second body applies -J -------------
void Body::applyImpulse(const Vec2& J) {
	if (isStatic()) {
		return;
	}
	velocity += J * invMass;
}

void Body::integrateAngular(float dt) {
	if (isStatic())		// integration is not for static bodies
		return;

    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorques * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;

    // Integrate the angular velocity to find the new rotation angle
    rotation += angularVelocity * dt;

    // Clear all the torque acting on the object before the next physics step
    clearTorques();
}
