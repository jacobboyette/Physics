/*
Modifications based on the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Contact.h"

void Contact::resolvePenetration() {
	float da = 0, db = 0;

	if (a->isStatic()) {
		da = 0;
		db = depth;
	}
	else if (b->isStatic()) {
		da = depth;
		db = 0;
	}
	else {
		da = depth * b->mass / (a->mass + b->mass);
		db = depth * a->mass / (a->mass + b->mass);
	}

    a->position -= normal * da;
    b->position += normal * db;
}

void Contact::resolveCollision() {
	// Apply positional correction using the projection method
	resolvePenetration();

	// Define elasticity (coefficient of restitution e)
	//float e = std::fmin(a->restitution, b->restitution);
	float e = std::min(a->restitution, b->restitution);

	// Calculate the relative velocity between the two objects
	const Vec2 vRel = (a->velocity - b->velocity);

	// Calculate the relative velocity along the normal collision vector
	float approachingSpeed = vRel.dot(normal);

	// Now we proceed to calculate the collision impulse
	const Vec2 impulseDirection = normal;
	const float impulseMagnitude = -(1 + e) * approachingSpeed / (a->invMass + b->invMass);

	Vec2 J = impulseDirection * impulseMagnitude;

	// Apply the impulse vector to both objects in opposite direction
	a->applyImpulse(J);
	b->applyImpulse(-J);
}