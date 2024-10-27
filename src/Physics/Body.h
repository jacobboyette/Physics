/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body {
    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    
    // Forces and torque
    Vec2 sumForces;
    float sumTorques;

    // Mass and Moment of Inertia
    float mass;
    float invMass;
    float I;
    float invI;
	float restitution;	// Coefficient of elasticity. Value is between 0 and 1.

    Shape* shape = nullptr;	// Pointer to the shape/geometry of this rigid body
	int color;				// the color of the body for rendering

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

	bool isStatic() const;		// check whether the body is static or not

    void addForce(const Vec2& force);
    void addTorque(float torque);
    void clearForces();
    void clearTorques();

	void applyImpulse(const Vec2& J);	// apply impulse to the body

    void integrateLinear(float dt);
    void integrateAngular(float dt);
};

#endif
