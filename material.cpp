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

StandardShaderMaterial::StandardShaderMaterial()
{
    this->lambertShaderMaterial = LambertShaderMaterial();
    this->blinnPhongShaderMaterial = BlinnPhongShaderMaterial();
    this->ambientIntensity = 0;
    this->ambientColor = { 255, 255, 255 };
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor)
{
    this->lambertShaderMaterial = LambertShaderMaterial();
    this->blinnPhongShaderMaterial = BlinnPhongShaderMaterial();
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, float phongExponent)
{
    this->lambertShaderMaterial = LambertShaderMaterial();
    this->blinnPhongShaderMaterial = BlinnPhongShaderMaterial(phongExponent);
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = { 255, 255, 255 };
}

StandardShaderMaterial::StandardShaderMaterial(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor)
{
    this->lambertShaderMaterial = LambertShaderMaterial(surfaceColor);
    this->blinnPhongShaderMaterial = BlinnPhongShaderMaterial(specularColor);
    this->ambientIntensity = 0;
    this->ambientColor = ambientColor;
}

StandardShaderMaterial::StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor, float phongExponent, Util::Color surfaceColor, Util::Color specularColor)
{
    this->lambertShaderMaterial = LambertShaderMaterial(surfaceColor);
    this->blinnPhongShaderMaterial = BlinnPhongShaderMaterial(phongExponent, specularColor);
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

float StandardShaderMaterial::getAmbientIntensity() const
{
    return this->ambientIntensity;
}

float StandardShaderMaterial::getPhongExponent() const
{
    return this->blinnPhongShaderMaterial.getPhongExponent();
}

Util::Color StandardShaderMaterial::getSurfaceColor() const
{
    return this->lambertShaderMaterial.getSurfaceColor();
}

Util::Color StandardShaderMaterial::getSpecularColor() const
{
    return this->blinnPhongShaderMaterial.getSpecularColor();
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
    this->blinnPhongShaderMaterial.setPhongExponent(phongExponent);
}

void StandardShaderMaterial::setSurfaceColor(Util::Color surfaceColor)
{
    this->lambertShaderMaterial.setSurfaceColor(surfaceColor);
}

void StandardShaderMaterial::setSpecularColor(Util::Color specularColor)
{
    this->blinnPhongShaderMaterial.setSpecularColor(specularColor);
}

void StandardShaderMaterial::setAmbientColor(Util::Color ambientColor)
{
    this->ambientColor = ambientColor;
}

Util::Color StandardShaderMaterial::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const
{
    // TOOD: wow this way of doing it is bad. maybe make HitRecord a class with a copy-ctor
    std::unique_ptr<Util::HitRecord> lambertHitRecord(new Util::HitRecord);
    lambertHitRecord->intersectionPoint = hitRecord->intersectionPoint;
    lambertHitRecord->intersectionTime = hitRecord->intersectionTime;
    lambertHitRecord->unitNormal = hitRecord->unitNormal; 
    const Util::Color lambertOutput = this->lambertShaderMaterial.computeColor(lightSources, std::move(lambertHitRecord), viewDirection);
    
    std::unique_ptr<Util::HitRecord> blinnPhongHitRecord(new Util::HitRecord);
    blinnPhongHitRecord->intersectionPoint = hitRecord->intersectionPoint;
    blinnPhongHitRecord->intersectionTime = hitRecord->intersectionTime;
    blinnPhongHitRecord->unitNormal = hitRecord->unitNormal; 
    const Util::Color blinnPhongOutput = this->blinnPhongShaderMaterial.computeColor(lightSources, std::move(blinnPhongHitRecord), viewDirection);

    // TODO: make color a class with ctors that make it so i don't need to convert every time like this
    const Util::Color ambientOutput = {
        (uint8_t) std::min(255, (int) std::floor(this->ambientColor.red * this->ambientIntensity)),
        (uint8_t) std::min(255, (int) std::floor(this->ambientColor.green * this->ambientIntensity)),
        (uint8_t) std::min(255, (int) std::floor(this->ambientColor.blue * this->ambientIntensity)),
    };

    return {
        (uint8_t) std::min(255, (int) lambertOutput.red + (int) blinnPhongOutput.red + (int) ambientOutput.red),
        (uint8_t) std::min(255, (int) lambertOutput.green + (int) blinnPhongOutput.green + (int) ambientOutput.green),
        (uint8_t) std::min(255, (int) lambertOutput.blue + (int) blinnPhongOutput.blue + (int) ambientOutput.blue),
    };
}
