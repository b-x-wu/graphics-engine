#ifndef LIGHT_SOURCE_HEADER
#define LIGHT_SOURCE_HEADER

#include <stdint.h>
#include "math.h"
#include "util.h"

class LightSource
{
public:
    LightSource();
    LightSource(float intensity);
    LightSource(float intensity, Util::Color color);

    float getIntensity() const;

    void setIntensity(float intensity);

    virtual Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const = 0;
private:
    float intensity;
    Util::Color color;
};

class UnidirectionalLightSource : public LightSource
{
public:
    UnidirectionalLightSource();
    UnidirectionalLightSource(Math::Vector3 direction);
    UnidirectionalLightSource(Math::Vector3 direction, float intensity);
    UnidirectionalLightSource(Math::Vector3 direction, float intensity, Util::Color color);

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
    PointLightSource(Math::Vector3 point, float intensity, Util::Color color);

    Math::Vector3 getPoint() const;

    void setPoint(Math::Vector3 point);

    Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const;
private:
    Math::Vector3 point;
};

#endif
