#include "material.h"
#include "util.h"
#include <cmath>
#include <iostream>

StaticColorMaterial::StaticColorMaterial()
{
    this->surfaceColor = { 0, 0, 0 };
}

StaticColorMaterial::StaticColorMaterial(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

Util::Color StaticColorMaterial::getSurfaceColor() const
{
    return this->surfaceColor;
}

void StaticColorMaterial::setSurfaceColor(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

Util::Color StaticColorMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord) const
{
    return this->surfaceColor;
}

LambertShaderMaterial::LambertShaderMaterial() {}

LambertShaderMaterial::LambertShaderMaterial(Util::Color surfaceColor)
    : StaticColorMaterial::StaticColorMaterial(surfaceColor) {}

Util::Color LambertShaderMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord) const
{
    float scalingFactor = 0;
    for (auto & lightSource : lightSources)
    {
        scalingFactor += lightSource->getIntensity() * std::max((float) 0, Math::dot(hitRecord->unitNormal, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint)));
    }
    // std::cout << scalingFactor << std::endl;
    return {
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.red * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.green * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.blue * scalingFactor))
    };
}
