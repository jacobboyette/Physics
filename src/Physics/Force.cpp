/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
Minor modifications were made in fall, 2022
*/

#include "./Force.h"
#include <algorithm>

Vec2 Force::generateDragForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0, 0);
    //if (particle.velocity.magnitude() > 0.01) {
        // Calculate the drag direction (inverse of velocity unit vector)
	Vec2 dragDirection = particle.velocity.unitVector()*-1.0;

        // Calculate the drag magnitude, k * |v|^2
        float dragMagnitude = k * particle.velocity.magnitudeSquared();

        // Generate the final drag force with direction and magnitude
        dragForce = dragDirection * dragMagnitude;
    //}
    return dragForce;
}

Vec2 Force::generateFrictionForce(const Particle& particle, float k) {
    // Calculate the friction direction (inverse of velocity unit vector)
    Vec2 frictionDirection = particle.velocity.unitVector() * -1.0;

    // Calculate the friction magnitude (just k, for now)
    float frictionMagnitude = k;

    // Calculate the final resulting friction force vector
    Vec2 frictionForce = frictionDirection * frictionMagnitude;

    return frictionForce;
}

Vec2 Force::generateGravitationalForce(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance) {
    // Calculate the distance between the two objects
    Vec2 d = (b.position - a.position);

	float distance = d.magnitude();
    float distanceSquared = d.magnitudeSquared();

	//-------------------- clamp the distance -------------------
	if (distance < minDistance)
		distanceSquared = minDistance * minDistance;
	else if (distance > maxDistance)
		distanceSquared = maxDistance * maxDistance;
	//-------------------------------

    // Calculate the direction of the attraction force
    Vec2 attractionDirection = d.unitVector();

    // Calculate the strength of the attraction force
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

    // Calculate the final resulting attraction force vector
    Vec2 attractionForce = attractionDirection * attractionMagnitude;
    return attractionForce;
}


Vec2 Force::generateDragForce(const Body& body, float k) {
	Vec2 dragForce = Vec2(0, 0);
	if (body.velocity.magnitudeSquared() > 0) {
		// Calculate the drag direction (inverse of velocity unit vector)
		Vec2 dragDirection = body.velocity.unitVector() * -1.0;

		// Calculate the drag magnitude, k * |v|^2
		float dragMagnitude = k * body.velocity.magnitudeSquared();

		// Generate the final drag force with direction and magnitude
		dragForce = dragDirection * dragMagnitude;
	}
	return dragForce;
}

Vec2 Force::generateFrictionForce(const Body& body, float k) {
	// Calculate the friction direction (inverse of velocity unit vector)
	Vec2 frictionDirection = body.velocity.unitVector() * -1.0;

	// Calculate the friction magnitude (just k, for now)
	float frictionMagnitude = k;

	// Calculate the final resulting friction force vector
	Vec2 frictionForce = frictionDirection * frictionMagnitude;
	return frictionForce;
}

Vec2 Force::generateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance) {
	// Calculate the distance between the two objects
	Vec2 d = (b.position - a.position);

	float distanceSquared = d.magnitudeSquared();

	// Clamp the values of the distance (to allow for some insteresting visual effects)
	if (distanceSquared < minDistance)
		distanceSquared = minDistance * minDistance;
	else if (distanceSquared > maxDistance)
		distanceSquared = maxDistance * maxDistance;

	// Calculate the direction of the attraction force
	Vec2 attractionDirection = d.unitVector();

	// Calculate the strength of the attraction force
	float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

	// Calculate the final resulting attraction force vector
	Vec2 attractionForce = attractionDirection * attractionMagnitude;
	return attractionForce;
}

