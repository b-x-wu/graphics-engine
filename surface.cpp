#include <cmath>
#include "surface.h"
#include <iostream>

Sphere::Sphere()
{
    this->radius = 0;
    this->center = { 0, 0, 0 };
};

Sphere::Sphere(float radius)
{
    this->radius = radius;
    this->center = { 0, 0, 0 };
};

Sphere::Sphere(float radius, Math::Vector3 center)
{
    this->radius = radius;
    this->center = Math::Vector3(center);
};

float Sphere::getRadius() const
{
    return this->radius;
};

Math::Vector3 Sphere::getCenter() const
{
    return this->center;
};

void Sphere::setRadius(float radius)
{
    this->radius = radius;
};

void Sphere::setCenter(Math::Vector3 center)
{
    this->center = Math::Vector3(center);
};

bool Sphere::hit(Math::Ray ray, float t0, float t1, std::unique_ptr<HitRecord> & hitRecord) const
{
    float discriminant = std::pow((Math::dot(ray.direction, ray.origin - this->center)), 2) -
                            (Math::dot(ray.direction, ray.direction)) *
                            (Math::dot(ray.origin - this->center, ray.origin - this->center) - this->radius * this->radius);
    
    if (discriminant < 0) { return false; };

    if (discriminant == 0)
    {
        const float t = -(Math::dot(ray.direction, ray.origin - this->center)) / (Math::dot(ray.direction, ray.direction));
        if (t < t0 || t > t1) { return false; };
        
        hitRecord->intersectionTime = t;
        const Math::Vector3 p = ray.origin + t * ray.direction;
        hitRecord->unitNormal = (p - this->center) / this->radius;
        return true;
    }

    const float tPlus = (Math::dot(-ray.direction, ray.origin - this->center) + std::sqrt(discriminant)) / Math::dot(ray.direction, ray.direction);
    const float tMinus = (Math::dot(-ray.direction, ray.origin - this->center) - std::sqrt(discriminant)) / Math::dot(ray.direction, ray.direction);

    if ((tPlus < t0 || tPlus > t1) && (tMinus < t0 || tMinus > t1)) { return false; };
    
    // generally t will be non negative (ie the ray shoots forward and only forward from the origin). we want the first hit between t0 and t1
    const float t = std::min(tPlus, tMinus) >= t0 && std::min(tPlus, tMinus) <= t1 ? std::min(tPlus, tMinus) : std::max(tPlus, tMinus);
    hitRecord->intersectionTime = t;
    const Math::Vector3 p = ray.origin + t * ray.direction;
    hitRecord->unitNormal = (p - this->center) / this->radius;
    return true;
};

Math::Box Sphere::boundingBox() const
{
    Math::Vector3 min = { this->center.getX() - radius, this->center.getY() - radius, this->center.getZ() - radius };
    Math::Vector3 max = { this->center.getX() + radius, this->center.getY() + radius, this->center.getZ() + radius };
    return Math::Box(min, max);
};
