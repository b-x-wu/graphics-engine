#include "shader.h"
#include "util.h"
#include "hittable.h"
#include <cmath>
#include <iostream>

// TODO: make render distance settable
const float EPSILON = 0.0001;

StaticColorShader::StaticColorShader()
{
    this->surfaceColor = { 0, 0, 0 };
}

StaticColorShader::StaticColorShader(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

Util::Color StaticColorShader::getSurfaceColor() const
{
    return this->surfaceColor;
}

void StaticColorShader::setSurfaceColor(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

Util::Color StaticColorShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<float>::max(), hitRecord)) {
        hitRecord->intersectionTime = -1;
        return { 0, 0, 0};
    }
    return this->surfaceColor;
}

LambertShader::LambertShader() {}

LambertShader::LambertShader(Util::Color surfaceColor)
    : StaticColorShader::StaticColorShader(surfaceColor) {}

Util::Color LambertShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<float>::max(), hitRecord)) {
        hitRecord->intersectionTime = -1;
        return { 0, 0, 0 };
    }
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

BlinnPhongShader::BlinnPhongShader()
{
    this->phongExponent = 1;
    this->specularColor = { 255, 255, 255 };
}

BlinnPhongShader::BlinnPhongShader(float phongExponent)
{
    this->phongExponent = phongExponent;
    this->specularColor = { 255, 255, 255 };
}

BlinnPhongShader::BlinnPhongShader(Util::Color specularColor)
{
    this->phongExponent = 1;
    this->specularColor = specularColor;
}

BlinnPhongShader::BlinnPhongShader(float phongExponent, Util::Color specularColor)
{
    this->phongExponent = phongExponent;
    this->specularColor = specularColor;
}

float BlinnPhongShader::getPhongExponent() const
{
    return this->phongExponent;
}

Util::Color BlinnPhongShader::getSpecularColor() const
{
    return this->specularColor;
}

void BlinnPhongShader::setPhongExponent(float phongExponent)
{
    this->phongExponent = phongExponent;
}

void BlinnPhongShader::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

Util::Color BlinnPhongShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<float>::max(), hitRecord)) {
        hitRecord->intersectionTime = -1;
        return { 0, 0, 0 };
    }
    float scalingFactor = 0;
    Math::Vector3 v = viewRay.direction / viewRay.direction.norm();
    Math::Vector3 h;

    for (auto & lightSource : lightSources)
    {
        h = (-v - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
        scalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0f, Math::dot(hitRecord->unitNormal, h / h.norm())), this->phongExponent);
    }
    return {
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.red * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.green * scalingFactor)),
        (uint8_t) std::min(255, (int) std::floor(this->specularColor.blue * scalingFactor))
    };
}

StandardShader::StandardShader()
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = 1;
    this->ambientIntensity = 0;
    this->ambientColor = { 255, 255, 255 };
}

StandardShader::StandardShader(float ambientIntensity, Util::Color ambientColor)
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = 1;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

StandardShader::StandardShader(float ambientIntensity, float phongExponent)
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = phongExponent;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = { 255, 255, 255 };
}

StandardShader::StandardShader(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor)
{
    this->surfaceColor = surfaceColor;
    this->specularColor = specularColor;
    this->phongExponent = 1;
    this->ambientIntensity = 0;
    this->ambientColor = ambientColor;
}

StandardShader::StandardShader(float ambientIntensity, Util::Color ambientColor, float phongExponent, Util::Color surfaceColor, Util::Color specularColor)
{
    this->surfaceColor = surfaceColor;
    this->specularColor = specularColor;
    this->phongExponent = phongExponent;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

float StandardShader::getAmbientIntensity() const
{
    return this->ambientIntensity;
}

float StandardShader::getPhongExponent() const
{
    return this->phongExponent;
}

Util::Color StandardShader::getSurfaceColor() const
{
    return this->surfaceColor;
}

Util::Color StandardShader::getSpecularColor() const
{
    return this->specularColor;
}

Util::Color StandardShader::getAmbientColor() const
{
    return this->ambientColor;
}

void StandardShader::setAmbientIntensity(float ambientIntensity)
{
    this->ambientIntensity = ambientIntensity;
}

void StandardShader::setPhongExponent(float phongExponent)
{
    this->phongExponent = phongExponent;
}

void StandardShader::setSurfaceColor(Util::Color surfaceColor)
{
    this->surfaceColor = surfaceColor;
}

void StandardShader::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

void StandardShader::setAmbientColor(Util::Color ambientColor)
{
    this->ambientColor = ambientColor;
}

Util::Color StandardShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<float>::max(), hitRecord)) {
        hitRecord->intersectionTime = -1;
        return { 0, 0, 0 };
    }

    std::vector<std::shared_ptr<Util::HitRecord>> lightSourceHitRecords = std::vector<std::shared_ptr<Util::HitRecord>>();
    Math::Ray p;
    for (auto & lightSource : lightSources)
    {
        std::shared_ptr<Util::HitRecord> lightSourceHitRecord(new Util::HitRecord);
        p = { hitRecord->intersectionPoint, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint) };
        surface->hit(p, EPSILON, lightSource->timeToLightSource(p), lightSourceHitRecord); // TODO: if it's a single point light should not go to render distance, but to the light
        lightSourceHitRecords.push_back(lightSourceHitRecord);
    }
    float redAmbientColor = this->ambientColor.red * this->ambientIntensity;
    float greenAmbientColor = this->ambientColor.green * this->ambientIntensity;
    float blueAmbientColor = this->ambientColor.blue * this->ambientIntensity;

    int lightSourceIndex = 0;
    float lambertScalingFactor = 0;
    float blinnPhongScalingFactor = 0;
    Math::Vector3 unitViewDirection = viewRay.direction / viewRay.direction.norm();
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

MirrorShader::MirrorShader() {}

Util::Color MirrorShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<float>::max(), hitRecord)) {
        hitRecord->intersectionTime = -1;
        return { 0, 0, 0 };
    }

    const Math::Vector3 d = viewRay.direction / viewRay.direction.norm();
    const Math::Vector3 r = d - 2 * Math::dot(d, hitRecord->unitNormal) * hitRecord->unitNormal;
    const Math::Ray reflectionRay = { hitRecord->intersectionPoint + (EPSILON * r), r };
    return surface->computeColor(lightSources, reflectionRay, surface, hitRecord);
}
