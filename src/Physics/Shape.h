/*
Modification based on the the source code from Gustavo Pezzi, co-founder of Pikuma.com
*/

#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <vector>

enum ShapeType {
    DISK
	// other shapes will be added later
};

struct Shape {
    virtual ~Shape() = default;
    virtual ShapeType getType() const = 0;
    virtual Shape* clone() const = 0;
    virtual float getMomentOfInertia() const = 0;
};

struct Disk: public Shape {
    float radius;

    Disk(const float radius);
    virtual ~Disk();
    ShapeType getType() const override;
    Shape* clone() const override;
    float getMomentOfInertia() const override;

	float getArea() const;
};

// structures of other shapes will be added later
#endif
