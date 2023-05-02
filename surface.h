#ifndef SURFACE_HEADER
#define SURFACE_HEADER

#include "math.h"
#include <memory>
#include <vector>

struct HitRecord
{
    float intersectionTime;
    Math::Vector3 unitNormal;
};

class Surface
{
public:
    virtual bool hit(Math::Ray ray, float t0, float t1, std::unique_ptr<HitRecord> & hitRecord) const = 0;
    virtual Math::Box boundingBox() const = 0;
};

class Sphere: public Surface
{
public:
    Sphere();
    Sphere(float radius);
    Sphere(float radius, Math::Vector3 center);

    float getRadius() const;
    Math::Vector3 getCenter() const;

    void setRadius(float radius);
    void setCenter(Math::Vector3 center);

    bool hit(Math::Ray ray, float t0, float t1, std::unique_ptr<HitRecord> & hitRecord) const;
    Math::Box boundingBox() const;
private:
    float radius;
    Math::Vector3 center;
};

class Triangle: public Surface
{
public:
    Triangle();
    // let vertex1, vertex2, and vertex3 be in counterclockwise order
    Triangle(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3);

    std::vector<Math::Vector3> getVertices() const;
    Math::Vector3 getUnitNormal() const;

    void setVertices(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3);

    bool hit(Math::Ray ray, float t0, float t1, std::unique_ptr<HitRecord> & hitRecord) const;
    Math::Box boundingBox() const;
private:
    Math::Vector3 vertex1, vertex2, vertex3;
};


#endif
