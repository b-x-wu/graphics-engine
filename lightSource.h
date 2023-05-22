#ifndef LIGHT_SOURCE_HEADER
#define LIGHT_SOURCE_HEADER

#include <stdint.h>
#include "math.h"
#include "util.h"
#include <limits>

class LightSource
{
public:
    LightSource();
    LightSource(double intensity);
    LightSource(double intensity, Util::Color color);

    double getIntensity() const;

    void setIntensity(double intensity);

    virtual double timeToLightSource(Math::Ray ray) const = 0;
    virtual Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const = 0;
private:
    double intensity;
    Util::Color color; // TODO: wait when is this used?
};

class UnidirectionalLightSource : public LightSource
{
public:
    UnidirectionalLightSource();
    UnidirectionalLightSource(Math::Vector3 direction);
    UnidirectionalLightSource(Math::Vector3 direction, double intensity);
    UnidirectionalLightSource(Math::Vector3 direction, double intensity, Util::Color color);

    Math::Vector3 getDirection() const;

    void setDirection(Math::Vector3 direction);
    void setMaxRenderDistance(double maxRenderDistance);

    double timeToLightSource(Math::Ray ray) const;
    Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const;
private:
    Math::Vector3 direction;
    double maxRenderDistance = std::numeric_limits<double>::max();
};

class PointLightSource : public LightSource
{
public:
    PointLightSource();
    PointLightSource(Math::Vector3 point);
    PointLightSource(Math::Vector3 point, double intensity);
    PointLightSource(Math::Vector3 point, double intensity, Util::Color color);

    Math::Vector3 getPoint() const;

    void setPoint(Math::Vector3 point);

    double timeToLightSource(Math::Ray ray) const;
    Math::Vector3 getLightDirectionToSurfacePoint(Math::Vector3 surfacePoint) const;
private:
    Math::Vector3 point;
};

#endif
