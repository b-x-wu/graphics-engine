#include "lightSource.h"
#include "math.h"

LightSource::LightSource()
{
    this->intensity = 0;
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}

LightSource::LightSource(float intensity)
{
    this->intensity = intensity;
    this->red = 0;
    this->green = 0;
    this->blue = 0;
}

LightSource::LightSource(float intensity, uint8_t red, uint8_t green, uint8_t blue)
{
    this->intensity = intensity;
    this->red = red;
    this->green = green;
    this->blue = blue;
}

float LightSource::getIntensity() const
{
    return this->intensity;
}

void LightSource::setIntensity(float intensity)
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

UnidirectionalLightSource::UnidirectionalLightSource(Math::Vector3 direction, float intensity)
    : LightSource::LightSource(intensity)
{
    this->direction = direction;
}

UnidirectionalLightSource::UnidirectionalLightSource(Math::Vector3 direction, float intensity, uint8_t red, uint8_t green, uint8_t blue)
    : LightSource::LightSource(intensity, red, green, blue)
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

PointLightSource::PointLightSource(Math::Vector3 point, float intensity)
    : LightSource::LightSource(intensity)
{
    this->point = point;
}

PointLightSource::PointLightSource(Math::Vector3 point, float intensity, uint8_t red, uint8_t green, uint8_t blue)
    : LightSource::LightSource(intensity, red, green, blue)
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

Math::Vector3 PointLightSource::getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const
{
    const Math::Vector3 unnormalizedDirection = surfacePoint - this->point;
    return unnormalizedDirection / unnormalizedDirection.norm();
}
