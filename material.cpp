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

Util::Color StaticColorMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const
{
    return this->surfaceColor;
}

LambertShaderMaterial::LambertShaderMaterial() {}

LambertShaderMaterial::LambertShaderMaterial(Util::Color surfaceColor)
    : StaticColorMaterial::StaticColorMaterial(surfaceColor) {}

Util::Color LambertShaderMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const
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

Util::Color BlinnPhongShaderMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const
{
    float scalingFactor = 0;
    Math::Vector3 v = viewDirection / viewDirection.norm();
    Math::Vector3 h;

    for (auto & lightSource : lightSources)
    {
        h = (-v - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
        // std::cout << h << std::endl;
        scalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0f, Math::dot(hitRecord->unitNormal, h / h.norm())), this->getPhongExponent());
    }
    return {
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.red * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.green * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.blue * scalingFactor))
    };
}
