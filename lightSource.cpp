#include "lightSource.h"
#include "math.h"
#include "util.h"

LightSource::LightSource()
{
    this->intensity = 0;
    this->color = { 0, 0, 0 };
}

LightSource::LightSource(double intensity)
{
    this->intensity = intensity;
    this->color = { 0, 0, 0 };
}

LightSource::LightSource(double intensity, Util::Color color)
{
    this->intensity = intensity;
    this->color = color;
}

double LightSource::getIntensity() const
{
    return this->intensity;
}

void LightSource::setIntensity(double intensity)
{
    this->intensity = intensity;
}

UnidirectionalLightSource::UnidirectionalLightSource()
{
    this->direction = { 0, 0, -1 };
}

UnidirectionalLightSource::UnidirectionalLightSource(Math::Vector3 direction)
{
    this->direction = direction;
}

UnidirectionalLightSource::UnidirectionalLightSource(Math::Vector3 direction, double intensity)
    : LightSource::LightSource(intensity)
{
    this->direction = direction;
}

UnidirectionalLightSource::UnidirectionalLightSource(Math::Vector3 direction, double intensity, Util::Color color)
    : LightSource::LightSource(intensity, color)
{
    this->direction = direction;
}

Math::Vector3 UnidirectionalLightSource::getDirection() const
{
    return this->direction;
}

void UnidirectionalLightSource::setDirection(Math::Vector3 direction)
{
    this->direction = direction;
}

void UnidirectionalLightSource::setMaxRenderDistance(double maxRenderDistance)
{
    this->maxRenderDistance = maxRenderDistance;
}

double UnidirectionalLightSource::timeToLightSource(Math::Ray ray) const
{
    return maxRenderDistance;
}

Math::Vector3 UnidirectionalLightSource::getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const
{
    return this->direction / this->direction.norm();
}

PointLightSource::PointLightSource()
{
    this->point = { 0, 0, -1 };
}

PointLightSource::PointLightSource(Math::Vector3 point)
{
    this->point = point;
}

PointLightSource::PointLightSource(Math::Vector3 point, double intensity)
    : LightSource::LightSource(intensity)
{
    this->point = point;
}

PointLightSource::PointLightSource(Math::Vector3 point, double intensity, Util::Color color)
    : LightSource::LightSource(intensity, color)
{
    this->point = point;
}

Math::Vector3 PointLightSource::getPoint() const
{
    return this->point;
}

void PointLightSource::setPoint(Math::Vector3 point)
{
    this->point = point;
}

double PointLightSource::timeToLightSource(Math::Ray ray) const
{
    double raySpeed = ray.direction.norm();
    return std::abs(this->point.getX() - ray.origin.getX()) / raySpeed;
}

Math::Vector3 PointLightSource::getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const
{
    const Math::Vector3 unnormalizedDirection = surfacePoint - this->point;
    return unnormalizedDirection / unnormalizedDirection.norm();
}
