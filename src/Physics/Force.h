/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef FORCE_H
#define FORCE_H

#include "./Vec2.h"
#include "./Particle.h"
#include "./Body.h"

struct Force {
    static Vec2 generateDragForce(const Particle& particle, float k);
    static Vec2 generateFrictionForce(const Particle& particle, float k);
    static Vec2 generateGravitationalForce(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance);

	static Vec2 generateDragForce(const Body& body, float k);
	static Vec2 generateFrictionForce(const Body& body, float k);
	static Vec2 generateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
};

#endif
