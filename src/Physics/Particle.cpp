/*
Minor modifications were made in fall 2022, based on the original source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    std::cout << "Particle constructor 1 called!" << std::endl;
}

Particle::Particle(float x, float y, float mass, float radius) {
	this->position = Vec2(x, y);
	this->mass = mass;
	this->radius = radius;
	if (mass != 0.0) {
		this->invMass = 1.0 / mass;
	}
	else {
		this->invMass = 0.0;
	}

	std::cout << "Particle constructor 2 called!" << std::endl;
}

Particle::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}

void Particle::addForce(const Vec2& force) {
    sumForces += force;
}

void Particle::clearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Particle::integrate(float dt) {
    // Find the acceleration based on the forces that are being applied and the mass
	acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the acceleration
    position += velocity * dt;

    clearForces();
}

void Particle::integrate(float dt, Vec2 initAccelaration) {
	// Find the acceleration based on the forces that are being applied and the mass
	acceleration = sumForces * invMass + initAccelaration;

	// Integrate the acceleration to find the new velocity
	velocity += acceleration * dt;

	// Integrate the velocity to find the acceleration
	position += velocity * dt;

	clearForces();
}
