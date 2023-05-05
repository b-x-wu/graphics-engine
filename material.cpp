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

Util::Color StaticColorMaterial::computeColor(
    const std::vector<std::unique_ptr<LightSource>> & lightSources,
    std::unique_ptr<Util::HitRecord> hitRecord,
    Math::Vector3 viewDirection,
    const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
) const
{
    return this->surfaceColor;
}

LambertShaderMaterial::LambertShaderMaterial() {}

LambertShaderMaterial::LambertShaderMaterial(Util::Color surfaceColor)
    : StaticColorMaterial::StaticColorMaterial(surfaceColor) {}

Util::Color LambertShaderMaterial::computeColor(
    const std::vector<std::unique_ptr<LightSource>> & lightSources,
    std::unique_ptr<Util::HitRecord> hitRecord,
    Math::Vector3 viewDirection,
    const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
) const
{
    float scalingFactor = 0;
    for (auto & lightSource : lightSources)
    {
        scalingFactor += lightSource->getIntensity() * std::max((float) 0, Math::dot(hitRecord->unitNormal, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint)));
    }
    return {
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.red * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.green * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->surfaceColor.blue * scalingFactor))
    };
}

BlinnPhongShaderMaterial::BlinnPhongShaderMaterial()
{
    this->phongExponent = 1;
    this->specularColor = { 255, 255, 255 };
}

BlinnPhongShaderMaterial::BlinnPhongShaderMaterial(float phongExponent)
{
    this->phongExponent = phongExponent;
    this->specularColor = { 255, 255, 255 };
}

BlinnPhongShaderMaterial::BlinnPhongShaderMaterial(Util::Color specularColor)
{
    this->phongExponent = 1;
    this->specularColor = specularColor;
}

BlinnPhongShaderMaterial::BlinnPhongShaderMaterial(float phongExponent, Util::Color specularColor)
{
    this->phongExponent = phongExponent;
    this->specularColor = specularColor;
}

float BlinnPhongShaderMaterial::getPhongExponent() const
{
    return this->phongExponent;
}

Util::Color BlinnPhongShaderMaterial::getSpecularColor() const
{
    return this->specularColor;
}

void BlinnPhongShaderMaterial::setPhongExponent(float phongExponent)
{
    this->phongExponent = phongExponent;
}

void BlinnPhongShaderMaterial::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

Util::Color BlinnPhongShaderMaterial::computeColor(
    const std::vector<std::unique_ptr<LightSource>> &lightSources,
    std::unique_ptr<Util::HitRecord> hitRecord,
    Math::Vector3 viewDirection,
    const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
) const
{
    float scalingFactor = 0;
    Math::Vector3 v = viewDirection / viewDirection.norm();
    Math::Vector3 h;

    for (auto & lightSource : lightSources)
    {
        h = (-v - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
        scalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0f, Math::dot(hitRecord->unitNormal, h / h.norm())), this->getPhongExponent());
    }
    return {
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.red * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.green * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.blue * scalingFactor))
    };
}

StandardShaderMaterial::StandardShaderMaterial()
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = 1;
    this->ambientIntensity = 0;
    this->ambientColor = { 255, 255, 255 };
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor)
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = 1;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, float phongExponent)
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = phongExponent;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = { 255, 255, 255 };
}

StandardShaderMaterial::StandardShaderMaterial(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor)
{
    this->surfaceColor = surfaceColor;
    this->specularColor = specularColor;
    this->phongExponent = 1;
    this->ambientIntensity = 0;
    this->ambientColor = ambientColor;
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor, float phongExponent, Util::Color surfaceColor, Util::Color specularColor)
{
    this->surfaceColor = surfaceColor;
    this->specularColor = specularColor;
    this->phongExponent = phongExponent;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

float StandardShaderMaterial::getAmbientIntensity() const
{
    return this->ambientIntensity;
}

float StandardShaderMaterial::getPhongExponent() const
{
    return this->phongExponent;
}

Util::Color StandardShaderMaterial::getSurfaceColor() const
{
    return this->surfaceColor;
}

Util::Color StandardShaderMaterial::getSpecularColor() const
{
    return this->specularColor;
}

Util::Color StandardShaderMaterial::getAmbientColor() const
{
    return this->ambientColor;
}

void StandardShaderMaterial::setAmbientIntensity(float ambientIntensity)
{
    this->ambientIntensity = ambientIntensity;
}

void StandardShaderMaterial::setPhongExponent(float phongExponent)
{
    this->phongExponent = phongExponent;
}

void StandardShaderMaterial::setSurfaceColor(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

void StandardShaderMaterial::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

void StandardShaderMaterial::setAmbientColor(Util::Color ambientColor)
{
    this->ambientColor = ambientColor;
}

Util::Color StandardShaderMaterial::computeColor(
    const std::vector<std::unique_ptr<LightSource>> &lightSources,
    std::unique_ptr<Util::HitRecord> hitRecord,
    Math::Vector3 viewDirection,
    const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
) const
{
    float redAmbientColor = this->ambientColor.red * this->ambientIntensity;
    float greenAmbientColor = this->ambientColor.green * this->ambientIntensity;
    float blueAmbientColor = this->ambientColor.blue * this->ambientIntensity;

    int lightSourceIndex = 0;
    float lambertScalingFactor = 0;
    float blinnPhongScalingFactor = 0;
    Math::Vector3 unitViewDirection = viewDirection / viewDirection.norm();
    Math::Vector3 h;
    for (auto & lightSource : lightSources)
    {
        if (lightSourceHitRecords.at(lightSourceIndex)->intersectionTime < 0)
        {
            lambertScalingFactor += lightSource->getIntensity() * std::max((float) 0, Math::dot(hitRecord->unitNormal, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint)));
            
            h = (-unitViewDirection - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
            blinnPhongScalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0f, Math::dot(hitRecord->unitNormal, h / h.norm())), this->getPhongExponent());
        }
        lightSourceIndex++;
    }

    return {
        (uint8_t) std::min(255, (int) std::floor(redAmbientColor + (lambertScalingFactor * this->surfaceColor.red) + (blinnPhongScalingFactor * this->specularColor.red))),
        (uint8_t) std::min(255, (int) std::floor(greenAmbientColor + (lambertScalingFactor * this->surfaceColor.green) + (blinnPhongScalingFactor * this->specularColor.green))),
        (uint8_t) std::min(255, (int) std::floor(blueAmbientColor + (lambertScalingFactor * this->surfaceColor.blue) + (blinnPhongScalingFactor * this->specularColor.blue)))
    };
}
