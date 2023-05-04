#include <cmath>
#include "surface.h"
#include "util.h"
#include "material.h"
#include <iostream>
#include <algorithm>

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

bool Sphere::hit(Math::Ray ray, float t0, float t1, std::unique_ptr<Util::HitRecord> & hitRecord) const
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
        hitRecord->intersectionPoint = p;
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
    hitRecord->intersectionPoint = p;
    return true;
};

Math::Box Sphere::boundingBox() const
{
    Math::Vector3 min = { this->center.getX() - radius, this->center.getY() - radius, this->center.getZ() - radius };
    Math::Vector3 max = { this->center.getX() + radius, this->center.getY() + radius, this->center.getZ() + radius };
    return Math::Box(min, max);
};

Triangle::Triangle()
{
    this->vertex1 = { 0, 0, 0 };
    this->vertex2 = { 0, 0, 0 };
    this->vertex3 = { 0, 0, 0 };
}

Triangle::Triangle(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3)
{
    this->vertex1 = Math::Vector3(vertex1);
    this->vertex2 = Math::Vector3(vertex2);
    this->vertex3 = Math::Vector3(vertex3);
}

std::vector<Math::Vector3> Triangle::getVertices() const
{
    std::vector<Math::Vector3> vertices = std::vector<Math::Vector3>();
    vertices.push_back(this->vertex1);
    vertices.push_back(this->vertex2);
    vertices.push_back(this->vertex3);
    return vertices;
}

Math::Vector3 Triangle::getUnitNormal() const
{
    const Math::Vector3 normal = Math::cross(this->vertex2 - this->vertex1, this->vertex3 - this->vertex2);
    return normal / normal.norm();
}

void Triangle::setVertices(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3)
{
    this->vertex1 = vertex1;
    this->vertex2 = vertex2;
    this->vertex3 = vertex3;
}

bool Triangle::hit(Math::Ray ray, float t0, float t1, std::unique_ptr<Util::HitRecord> & hitRecord) const
{
    const float a = this->vertex1.getX() - this->vertex2.getX();
    const float b = this->vertex1.getY() - this->vertex2.getY();
    const float c = this->vertex1.getZ() - this->vertex2.getZ();
    const float d = this->vertex1.getX() - this->vertex3.getX();
    const float e = this->vertex1.getY() - this->vertex3.getY();
    const float f = this->vertex1.getZ() - this->vertex3.getZ();
    const float g = ray.direction.getX();
    const float h = ray.direction.getY();
    const float i = ray.direction.getZ();
    const float j = this->vertex1.getX() - ray.origin.getX();
    const float k = this->vertex1.getY() - ray.origin.getY();
    const float l = this->vertex1.getZ() - ray.origin.getZ();

    const float eiMinusHf = e * i - h * f;
    const float gfMinusDi = g * f - d * i;
    const float dhMinusEg = d * h - e * g;
    const float akMinusJb = a * k - j * b;
    const float jcMinusAl = j * c - a * l;
    const float blMinusKc = b * l - k * c;
    const float M = a * eiMinusHf + b * gfMinusDi + c * dhMinusEg;

    const float t = -((f * akMinusJb + e * jcMinusAl + d * blMinusKc) / M);
    if (t < t0 || t > t1) { return false; }

    const float gamma = (i * akMinusJb + h * jcMinusAl + g * blMinusKc) / M;
    if (gamma < 0 || gamma > 1) { return false; }

    const float beta = (j * eiMinusHf + k * gfMinusDi + l * dhMinusEg) / M;
    if (beta < 0 || beta > 1 - gamma) { return false; }

    hitRecord->intersectionTime = t;
    hitRecord->unitNormal = Math::Vector3(this->getUnitNormal());
    hitRecord->intersectionPoint = ray.origin + t * ray.direction;
    return true;
}


Math::Box Triangle::boundingBox() const
{
    float minX = std::min({ this->vertex1.getX(), this->vertex2.getX(), this->vertex3.getX() });
    float minY = std::min({ this->vertex1.getY(), this->vertex2.getY(), this->vertex3.getY() });
    float minZ = std::min({ this->vertex1.getZ(), this->vertex2.getZ(), this->vertex3.getZ() });

    float maxX = std::max({ this->vertex1.getX(), this->vertex2.getX(), this->vertex3.getX() });
    float maxY = std::max({ this->vertex1.getY(), this->vertex2.getY(), this->vertex3.getY() });
    float maxZ = std::max({ this->vertex1.getZ(), this->vertex2.getZ(), this->vertex3.getZ() });

    return Math::Box({ minX, minY, minZ }, { maxX, maxY, maxZ });
}

GroupSurface::GroupSurface()
{
    this->surfaces = std::vector<std::unique_ptr<Surface>>();
    this->minBound = { 0, 0, 0 };
    this->maxBound = { 0, 0, 0 };
}

void GroupSurface::addSurface(std::unique_ptr<Surface> surface)
{
    this->minBound = {
        std::min(this->minBound.getX(), surface->boundingBox().min.getX()),
        std::min(this->minBound.getY(), surface->boundingBox().min.getY()),
        std::min(this->minBound.getZ(), surface->boundingBox().min.getZ())
    };
    this->maxBound = {
        std::max(this->maxBound.getX(), surface->boundingBox().max.getX()),
        std::max(this->maxBound.getY(), surface->boundingBox().max.getY()),
        std::max(this->maxBound.getZ(), surface->boundingBox().max.getZ())
    };
    this->surfaces.push_back(std::move(surface));
}

bool GroupSurface::hit(Math::Ray ray, float t0, float t1, std::unique_ptr<Util::HitRecord> & hitRecord) const
{
    bool groupHit = false;
    bool surfaceHit;
    float tMax = t1;
    std::unique_ptr<Util::HitRecord> surfaceHitRecord = std::unique_ptr<Util::HitRecord>(new Util::HitRecord);
    for (auto & surface : this->surfaces)
    {
        surfaceHit = surface->hit(ray, t0, t1, surfaceHitRecord);
        if (surfaceHit && surfaceHitRecord->intersectionTime >= t0 && surfaceHitRecord->intersectionTime <= tMax)
        {
            groupHit = true;
            tMax = surfaceHitRecord->intersectionTime;
            hitRecord->intersectionTime = surfaceHitRecord->intersectionTime;
            hitRecord->unitNormal = surfaceHitRecord->unitNormal;
            hitRecord->intersectionPoint = surfaceHitRecord->intersectionPoint;
        }
    }
    return groupHit;
}


Math::Box GroupSurface::boundingBox() const
{
    return Math::Box(this->minBound, this->maxBound);
}

void Surface::setMaterial(std::unique_ptr<Material> material)
{
    this->material = std::move(material);
}
