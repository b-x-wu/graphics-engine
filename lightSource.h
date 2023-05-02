#ifndef LIGHT_SOURCE_HEADER
#define LIGHT_SOURCE_HEADER

#include <stdint.h>
#include "math.h"

class LightSource
{
public:
    LightSource();
    LightSource(float intensity);
    LightSource(float intensity, uint8_t red, uint8_t green, uint8_t blue);

    float getIntensity() const;

    void setIntensity(float intensity);

    virtual Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const = 0;
private:
    float intensity;
    uint8_t red, green, blue;
};

class UnidirectionalLightSource : public LightSource
{
public:
    UnidirectionalLightSource();
    UnidirectionalLightSource(Math::Vector3 direction);
    UnidirectionalLightSource(Math::Vector3 direction, float intensity);
    UnidirectionalLightSource(Math::Vector3 direction, float intensity, uint8_t red, uint8_t green, uint8_t blue);

    Math::Vector3 getDirection() const;

    void setDirection(Math::Vector3 direction);

    Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const;
private:
    Math::Vector3 direction;
};

class PointLightSource : public LightSource
{
public:
    PointLightSource();
    PointLightSource(Math::Vector3 point);
    PointLightSource(Math::Vector3 point, float intensity);
    PointLightSource(Math::Vector3 point, float intensity, uint8_t red, uint8_t green, uint8_t blue);

    Math::Vector3 getPoint() const;

    void setPoint(Math::Vector3 point);

    Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const;
private:
    Math::Vector3 point;
};

#endif
