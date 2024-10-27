/*
The source code is from Gustavo Pezzi, co-founder of Pikuma.com 
*/

#ifndef VEC2_H
#define VEC2_H

struct Vec2 {
    float x;
    float y;

    Vec2();
    Vec2(float x, float y);
    
    ~Vec2() = default;

    void add(const Vec2& v);                 // v1.add(v2)
    void sub(const Vec2& v);                 // v1.sub(v2)
    void scale(const float n);               // v1.scale(n)
    Vec2 rotate(const float angle) const;    // v1.rotate(angle)

    float magnitude() const;                 // v1.magnitude()
    float magnitudeSquared() const;          // v1.magnitudeSquared()

    Vec2& normalize();                       // v1.normalize()
    Vec2 unitVector() const;                 // v1.unitVector()
    Vec2 normal() const;                     // n = v1.normal()

    float dot(const Vec2& v) const;          // v1.Dot(v2)
    float cross(const Vec2& v) const;        // v1.Cross(v2)

    Vec2& operator = (const Vec2& v);        // v1 = v2
    bool operator == (const Vec2& v) const;  // v1 == v2
    bool operator != (const Vec2& v) const;  // v1 != v2
    
    Vec2 operator + (const Vec2& v) const;   // v1 + v2
    Vec2 operator - (const Vec2& v) const;   // v1 - v2
    Vec2 operator * (const float n) const;   // v1 * n
    Vec2 operator / (const float n) const;   // v1 / n
    Vec2 operator - ();                      // -v1

    Vec2& operator += (const Vec2& v);       // v1 += v2
    Vec2& operator -= (const Vec2& v);       // v1 -= v2
    Vec2& operator *= (const float n);       // v1 *= n
    Vec2& operator /= (const float n);       // v1 /= n
};

#endif
