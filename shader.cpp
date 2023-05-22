#include "shader.h"
#include "util.h"
#include "hittable.h"
#include <cmath>
#include <iostream>

// TODO: make render distance settable
const double EPSILON = 0.0001;

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
    if (!surface->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord)) {
        hitRecord->isValid = false;
        return { 0, 0, 0};
    }
    return this->surfaceColor;
}

LambertShader::LambertShader() {}

LambertShader::LambertShader(Util::Color surfaceColor)
    : StaticColorShader::StaticColorShader(surfaceColor) {}

Util::Color LambertShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord)) {
        hitRecord->isValid = false;
        return { 0, 0, 0 };
    }
    double scalingFactor = 0;
    for (auto & lightSource : lightSources)
    {
        scalingFactor += lightSource->getIntensity() * std::max((double) 0, Math::dot(hitRecord->unitNormal, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint)));
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

BlinnPhongShader::BlinnPhongShader(double phongExponent)
{
    this->phongExponent = phongExponent;
    this->specularColor = { 255, 255, 255 };
}

BlinnPhongShader::BlinnPhongShader(Util::Color specularColor)
{
    this->phongExponent = 1;
    this->specularColor = specularColor;
}

BlinnPhongShader::BlinnPhongShader(double phongExponent, Util::Color specularColor)
{
    this->phongExponent = phongExponent;
    this->specularColor = specularColor;
}

double BlinnPhongShader::getPhongExponent() const
{
    return this->phongExponent;
}

Util::Color BlinnPhongShader::getSpecularColor() const
{
    return this->specularColor;
}

void BlinnPhongShader::setPhongExponent(double phongExponent)
{
    this->phongExponent = phongExponent;
}

void BlinnPhongShader::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

Util::Color BlinnPhongShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord)) {
        hitRecord->isValid = false;
        return { 0, 0, 0 };
    }
    double scalingFactor = 0;
    Math::Vector3 v = viewRay.direction / viewRay.direction.norm();
    Math::Vector3 h;

    for (auto & lightSource : lightSources)
    {
        h = (-v - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
        scalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0, Math::dot(hitRecord->unitNormal, h / h.norm())), this->phongExponent);
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

StandardShader::StandardShader(double ambientIntensity, Util::Color ambientColor)
{
    this->surfaceColor = { 255, 255, 255 };
    this->specularColor = { 255, 255, 255 };
    this->phongExponent = 1;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

StandardShader::StandardShader(double ambientIntensity, double phongExponent)
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

StandardShader::StandardShader(double ambientIntensity, Util::Color ambientColor, double phongExponent, Util::Color surfaceColor, Util::Color specularColor)
{
    this->surfaceColor = surfaceColor;
    this->specularColor = specularColor;
    this->phongExponent = phongExponent;
    this->ambientIntensity = ambientIntensity;
    this->ambientColor = ambientColor;
}

double StandardShader::getAmbientIntensity() const
{
    return this->ambientIntensity;
}

double StandardShader::getPhongExponent() const
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

void StandardShader::setAmbientIntensity(double ambientIntensity)
{
    this->ambientIntensity = ambientIntensity;
}

void StandardShader::setPhongExponent(double phongExponent)
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
    if (!surface->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord)) {
        hitRecord->isValid = false;
        return { 0, 0, 0 };
    }

    std::vector<std::shared_ptr<Util::HitRecord>> lightSourceHitRecords = std::vector<std::shared_ptr<Util::HitRecord>>();
    Math::Ray p;
    for (auto & lightSource : lightSources)
    {
        std::shared_ptr<Util::HitRecord> lightSourceHitRecord(new Util::HitRecord);
        p = { hitRecord->intersectionPoint, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint) };
        surface->hit(p, EPSILON, lightSource->timeToLightSource(p), lightSourceHitRecord);
        lightSourceHitRecords.push_back(lightSourceHitRecord);
    }
    double redAmbientColor = this->ambientColor.red * this->ambientIntensity;
    double greenAmbientColor = this->ambientColor.green * this->ambientIntensity;
    double blueAmbientColor = this->ambientColor.blue * this->ambientIntensity;

    int lightSourceIndex = 0;
    double lambertScalingFactor = 0;
    double blinnPhongScalingFactor = 0;
    Math::Vector3 unitViewDirection = viewRay.direction / viewRay.direction.norm();
    Math::Vector3 h;
    for (auto & lightSource : lightSources)
    {
        if (!lightSourceHitRecords.at(lightSourceIndex)->isValid)
        {
            lambertScalingFactor += lightSource->getIntensity() * std::max((double) 0, Math::dot(hitRecord->unitNormal, -lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint)));
        
            h = (-unitViewDirection - lightSource->getLightDirectionToSurfacePoint(hitRecord->intersectionPoint));
            blinnPhongScalingFactor += lightSource->getIntensity() * std::pow(std::max(0.0, Math::dot(hitRecord->unitNormal, h / h.norm())), this->getPhongExponent());
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

MirrorShader::MirrorShader(double specularWeight)
{
    this->setSpecularWeight(specularWeight);
}

MirrorShader::MirrorShader(Util::Color backgroundColor, Util::Color specularColor, double specularWeight)
{
    this->setSpecularWeight(specularWeight);
    this->specularColor = specularColor;
    this->backgroundColor = backgroundColor;
}

void MirrorShader::setBackgroundColor(Util::Color backgroundColor)
{
    this->backgroundColor = backgroundColor;
}

void MirrorShader::setSpecularColor(Util::Color specularColor)
{
    this->specularColor = specularColor;
}

void MirrorShader::setSpecularWeight(double specularWeight)
{
    if (specularWeight < 0) {
        this->specularWeight = 0;
        return;
    }
    if (specularWeight > 1) {
        this->specularWeight = 1;
        return;
    }
    this->specularWeight = specularWeight;
}

Util::Color MirrorShader::computeColor(const std::vector<std::unique_ptr<LightSource>> &lightSources, Math::Ray viewRay, std::shared_ptr<Renderable> surface, std::shared_ptr<Util::HitRecord> hitRecord) const
{
    if (!surface->hit(viewRay, 0, std::numeric_limits<double>::max(), hitRecord)) {
        hitRecord->isValid = false;
        return { 0, 0, 0 };
    }

    // TODO: check if the light bounces off the mirror and add that to lightSources
    const Math::Vector3 d = viewRay.direction / viewRay.direction.norm();
    const Math::Vector3 r = d - 2 * Math::dot(d, hitRecord->unitNormal) * hitRecord->unitNormal;
    const Math::Ray reflectionRay = { hitRecord->intersectionPoint + (EPSILON * r), r };
    Util::Color reflectionColor = surface->computeColor(lightSources, reflectionRay, surface, hitRecord);
    if (!hitRecord->isValid) {
        hitRecord->isValid = true; // TODO: must represent a valid hit. there's a better way to do this
        reflectionColor = this->backgroundColor;
    }
    return {
        (uint8_t) std::floor(this->specularWeight * this->specularColor.red + (1 - this->specularWeight) * reflectionColor.red),
        (uint8_t) std::floor(this->specularWeight * this->specularColor.green + (1 - this->specularWeight) * reflectionColor.green),
        (uint8_t) std::floor(this->specularWeight * this->specularColor.blue + (1 - this->specularWeight) * reflectionColor.blue)
    };
}
