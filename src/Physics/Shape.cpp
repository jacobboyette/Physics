/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#include "Shape.h"
#include <iostream>

Disk::Disk(float radius) {
    this->radius = radius;
    std::cout << "Disk constructor called!" << std::endl;
}

Disk::~Disk() {
    std::cout << "Disk destructor called!" << std::endl;
}

Shape* Disk::clone() const {
    return new Disk(radius);
}

ShapeType Disk::getType() const {
    return DISK;
}

float Disk::getMomentOfInertia() const {
    // For solid circles, the moment of inertia is 1/2 * r^2
    // But this still needs to be multiplied by the rigidbody's mass
    return 0.5 * (radius * radius);
}

float Disk::getArea() const {
	return 3.14159265 * radius * radius;
}
