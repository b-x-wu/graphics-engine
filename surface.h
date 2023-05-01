#ifndef SURFACE_HEADER
#define SURFACE_HEADER

#include "math.h"
#include <memory>

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


#endif
