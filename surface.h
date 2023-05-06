#ifndef SURFACE_HEADER
#define SURFACE_HEADER

#include "math.h"
#include "shader.h"
#include "util.h"
#include "hittable.h"
#include <memory>
#include <vector>

class Surface : public Hittable
{
public:
    void setMaterial(std::unique_ptr<Shader> shader);

    virtual Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Hittable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;

    virtual bool hit(Math::Ray ray, float t0, float t1, std::shared_ptr<Util::HitRecord> & hitRecord) const = 0;
    virtual Math::Box boundingBox() const = 0;

    std::unique_ptr<Shader> shader = NULL;
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

    bool hit(Math::Ray ray, float t0, float t1, std::shared_ptr<Util::HitRecord> & hitRecord) const;
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

    // let the triangle have these three vertices but make it so that the normal faces toward the facingDirection
    Triangle(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3, Math::Vector3 facingDirection);

    std::vector<Math::Vector3> getVertices() const;
    Math::Vector3 getUnitNormal() const;

    void setVertices(Math::Vector3 vertex1, Math::Vector3 vertex2, Math::Vector3 vertex3);

    bool hit(Math::Ray ray, float t0, float t1, std::shared_ptr<Util::HitRecord> & hitRecord) const;
    Math::Box boundingBox() const;
private:
    Math::Vector3 vertex1, vertex2, vertex3;
};

class GroupSurface: public Surface
{
public:
    GroupSurface();

    void addSurface(std::unique_ptr<Surface> surface);
    
    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Hittable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const;

    // Util::Color computeColor(
    //     const std::vector<std::unique_ptr<LightSource>> & lightSources,
    //     std::shared_ptr<Util::HitRecord> hitRecord,
    //     Math::Vector3 viewDirection,
    //     const std::vector<std::shared_ptr<Util::HitRecord>> & lightSourceHitRecords
    // ) const;
    
    bool hit(Math::Ray ray, float t0, float t1, std::shared_ptr<Util::HitRecord> & hitRecord) const;
    Math::Box boundingBox() const;
private:
    std::vector<std::unique_ptr<Surface>> surfaces;
    Math::Vector3 minBound, maxBound;
};


#endif
