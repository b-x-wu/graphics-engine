#include <cmath>
#include "surface.h"
#include "util.h"
#include "shader.h"
#include <iostream>
#include <algorithm>

Sphere::Sphere()
{
    this->radius = 0;
    this->center = { 0, 0, 0 };
};

Sphere::Sphere(double radius)
{
    this->radius = radius;
    this->center = { 0, 0, 0 };
};

Sphere::Sphere(double radius, Math::Vector3 center)
{
    this->radius = radius;
    this->center = Math::Vector3(center);
};

double Sphere::getRadius() const
{
    return this->radius;
};

Math::Vector3 Sphere::getCenter() const
{
    return this->center;
};

void Sphere::setRadius(double radius)
{
    this->radius = radius;
};

void Sphere::setCenter(Math::Vector3 center)
{
    this->center = Math::Vector3(center);
};

bool Sphere::hit(Math::Ray ray, double t0, double t1, std::shared_ptr<Util::HitRecord> & hitRecord) const
{
    double discriminant = std::pow((Math::dot(ray.direction, ray.origin - this->center)), 2) -
                            (Math::dot(ray.direction, ray.direction)) *
                            (Math::dot(ray.origin - this->center, ray.origin - this->center) - this->radius * this->radius);
    
    if (discriminant < 0) { return false; };

    if (discriminant == 0)
    {
        const double t = -(Math::dot(ray.direction, ray.origin - this->center)) / (Math::dot(ray.direction, ray.direction));
        if (t < t0 || t > t1) { return false; };
        
        hitRecord->intersectionTime = t;
        hitRecord->isValid = true;
        const Math::Vector3 p = ray.origin + t * ray.direction;
        hitRecord->unitNormal = (p - this->center) / this->radius;
        hitRecord->intersectionPoint = p;
        return true;
    }

    const double tPlus = (Math::dot(-ray.direction, ray.origin - this->center) + std::sqrt(discriminant)) / Math::dot(ray.direction, ray.direction);
    const double tMinus = (Math::dot(-ray.direction, ray.origin - this->center) - std::sqrt(discriminant)) / Math::dot(ray.direction, ray.direction);

    if ((tPlus < t0 || tPlus > t1) && (tMinus < t0 || tMinus > t1)) { return false; };
    
    // generally t will be non negative (ie the ray shoots forward and only forward from the origin). we want the first hit between t0 and t1
    const double t = std::min(tPlus, tMinus) >= t0 && std::min(tPlus, tMinus) <= t1 ? std::min(tPlus, tMinus) : std::max(tPlus, tMinus);
    hitRecord->intersectionTime = t;
    hitRecord->isValid = true;
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

Triangle::Triangle(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3, Math::Vector3 facingDirection)
{
    if (Math::dot(Math::cross(vertex2 - vertex1, vertex3 - vertex2), facingDirection) >= 0)
    {
        this->vertex1 = vertex1;
        this->vertex2 = vertex2;
        this->vertex3 = vertex3;
        return;
    }

    this->vertex1 = vertex2;
    this->vertex2 = vertex1;
    this->vertex3 = vertex3;
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

bool Triangle::hit(Math::Ray ray, double t0, double t1, std::shared_ptr<Util::HitRecord> & hitRecord) const
{
    const double a = this->vertex1.getX() - this->vertex2.getX();
    const double b = this->vertex1.getY() - this->vertex2.getY();
    const double c = this->vertex1.getZ() - this->vertex2.getZ();
    const double d = this->vertex1.getX() - this->vertex3.getX();
    const double e = this->vertex1.getY() - this->vertex3.getY();
    const double f = this->vertex1.getZ() - this->vertex3.getZ();
    const double g = ray.direction.getX();
    const double h = ray.direction.getY();
    const double i = ray.direction.getZ();
    const double j = this->vertex1.getX() - ray.origin.getX();
    const double k = this->vertex1.getY() - ray.origin.getY();
    const double l = this->vertex1.getZ() - ray.origin.getZ();

    const double eiMinusHf = e * i - h * f;
    const double gfMinusDi = g * f - d * i;
    const double dhMinusEg = d * h - e * g;
    const double akMinusJb = a * k - j * b;
    const double jcMinusAl = j * c - a * l;
    const double blMinusKc = b * l - k * c;
    const double M = a * eiMinusHf + b * gfMinusDi + c * dhMinusEg;

    const double t = -((f * akMinusJb + e * jcMinusAl + d * blMinusKc) / M);
    if (t < t0 || t > t1) { return false; }

    const double gamma = (i * akMinusJb + h * jcMinusAl + g * blMinusKc) / M;
    if (gamma < 0 || gamma > 1) { return false; }

    const double beta = (j * eiMinusHf + k * gfMinusDi + l * dhMinusEg) / M;
    if (beta < 0 || beta > 1 - gamma) { return false; }

    hitRecord->intersectionTime = t;
    hitRecord->isValid = true;
    hitRecord->unitNormal = Math::Vector3(this->getUnitNormal());
    hitRecord->intersectionPoint = ray.origin + t * ray.direction;
    return true;
}


Math::Box Triangle::boundingBox() const
{
    double minX = std::min({ this->vertex1.getX(), this->vertex2.getX(), this->vertex3.getX() });
    double minY = std::min({ this->vertex1.getY(), this->vertex2.getY(), this->vertex3.getY() });
    double minZ = std::min({ this->vertex1.getZ(), this->vertex2.getZ(), this->vertex3.getZ() });

    double maxX = std::max({ this->vertex1.getX(), this->vertex2.getX(), this->vertex3.getX() });
    double maxY = std::max({ this->vertex1.getY(), this->vertex2.getY(), this->vertex3.getY() });
    double maxZ = std::max({ this->vertex1.getZ(), this->vertex2.getZ(), this->vertex3.getZ() });

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

bool GroupSurface::hit(Math::Ray ray, double t0, double t1, std::shared_ptr<Util::HitRecord> & hitRecord) const
{
    bool groupHit = false;
    bool surfaceHit;
    double tMax = t1;
    int surfaceIndex = 0;
    std::shared_ptr<Util::HitRecord> surfaceHitRecord = std::shared_ptr<Util::HitRecord>(new Util::HitRecord);
    for (auto & surface : this->surfaces)
    {
        surfaceHit = surface->hit(ray, t0, t1, surfaceHitRecord);
        if (surfaceHit && surfaceHitRecord->intersectionTime >= t0 && surfaceHitRecord->intersectionTime <= tMax)
        {
            groupHit = true;
            tMax = surfaceHitRecord->intersectionTime;
            hitRecord->intersectionTime = surfaceHitRecord->intersectionTime;
            hitRecord->isValid = true;
            hitRecord->unitNormal = surfaceHitRecord->unitNormal;
            hitRecord->intersectionPoint = surfaceHitRecord->intersectionPoint;
            hitRecord->hitObjectIndex = surfaceIndex;
        }
        surfaceIndex += 1;
    }
    return groupHit;
}


Math::Box GroupSurface::boundingBox() const
{
    return Math::Box(this->minBound, this->maxBound);
}

void Surface::setMaterial(std::unique_ptr<Shader> shader)
{
    this->shader = std::move(shader);
}

Util::Color Surface::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (this->shader == NULL) { return { 0, 0, 0 }; }
    return this->shader->computeColor(lightSources, viewRay, surface, hitRecord);
}

Util::Color GroupSurface::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    const bool hitsGroup = this->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord);

    if (!hitsGroup) {
        hitRecord->isValid = false;
        return { 0, 0, 0 };
    }
    if (this->surfaces.at(hitRecord->hitObjectIndex)->shader == NULL && this->shader == NULL) { return { 0, 0, 0 }; } // hitRecord shows a hit and no shader displays black
    if (this->surfaces.at(hitRecord->hitObjectIndex)->shader == NULL) { return this->shader->computeColor(lightSources, viewRay, surface, hitRecord); } // use the shader of the group surface
    return this->surfaces.at(hitRecord->hitObjectIndex)->shader->computeColor(lightSources, viewRay, surface, hitRecord);
}
