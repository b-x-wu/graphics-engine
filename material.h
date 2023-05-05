#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "util.h"
#include "lightSource.h"
#include <memory>
#include <vector>

class Material
{
public:
    virtual Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        std::unique_ptr<Util::HitRecord> hitRecord,
        Math::Vector3 viewDirection,
        const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
    ) const = 0;
};

class StaticColorMaterial : public Material
{
public:
    StaticColorMaterial();
    StaticColorMaterial(Util::Color surfaceColor);

    Util::Color getSurfaceColor() const;

    void setSurfaceColor(Util::Color color);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        std::unique_ptr<Util::HitRecord> hitRecord,
        Math::Vector3 viewDirection,
        const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
    ) const;
protected:
    Util::Color surfaceColor;
};

class LambertShaderMaterial : public StaticColorMaterial
{
public:
    LambertShaderMaterial();
    LambertShaderMaterial(Util::Color surfaceColor);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        std::unique_ptr<Util::HitRecord> hitRecord,
        Math::Vector3 viewDirection,
        const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
    ) const;
};

class BlinnPhongShaderMaterial : public Material
{
public:
    BlinnPhongShaderMaterial();
    BlinnPhongShaderMaterial(float phongExponent);
    BlinnPhongShaderMaterial(Util::Color specularColor);
    BlinnPhongShaderMaterial(float phongExponent, Util::Color specularColor);

    float getPhongExponent() const;
    Util::Color getSpecularColor() const;

    void setPhongExponent(float phongExponent);
    void setSpecularColor(Util::Color specularColor);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        std::unique_ptr<Util::HitRecord> hitRecord,
        Math::Vector3 viewDirection,
        const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
    ) const;
private:
    Util::Color specularColor;
    float phongExponent;
};

// combination of lambert, blinn-phong, and ambient shading
class StandardShaderMaterial : public Material
{
public:
    StandardShaderMaterial();
    StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor);
    StandardShaderMaterial(float ambientIntensity, float phongExponent);
    StandardShaderMaterial(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor);
    StandardShaderMaterial(float ambientIntensity, Util::Color ambientColor, float phongExponent, Util::Color surfaceColor, Util::Color specularColor);

    float getAmbientIntensity() const;
    float getPhongExponent() const;
    Util::Color getSurfaceColor() const;
    Util::Color getSpecularColor() const;
    Util::Color getAmbientColor() const;

    void setAmbientIntensity(float ambientIntensity);
    void setPhongExponent(float phongExponent);
    void setSurfaceColor(Util::Color surfaceColor);
    void setSpecularColor(Util::Color specularColor);
    void setAmbientColor(Util::Color ambientColor);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> & lightSources,
        std::unique_ptr<Util::HitRecord> hitRecord,
        Math::Vector3 viewDirection,
        const std::vector<std::unique_ptr<Util::HitRecord>> & lightSourceHitRecords
    ) const;

private:
    Util::Color surfaceColor, specularColor, ambientColor;
    float ambientIntensity, phongExponent;
};

#endif
