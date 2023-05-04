#ifndef MATERIAL_HEADER
#define MATERIAL_HEADER

#include "util.h"
#include "lightSource.h"
#include <memory>
#include <vector>

class Material
{
public:
    virtual Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources,
                                     std::unique_ptr<Util::HitRecord> hitRecord,
                                     Math::Vector3 viewDirection) const = 0;
};

class StaticColorMaterial : public Material
{
public:
    StaticColorMaterial();
    StaticColorMaterial(Util::Color surfaceColor);

    Util::Color getSurfaceColor() const;

    void setSurfaceColor(Util::Color color);

    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const;
protected:
    Util::Color surfaceColor;
};

class LambertShaderMaterial : public StaticColorMaterial
{
public:
    LambertShaderMaterial();
    LambertShaderMaterial(Util::Color surfaceColor);

    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const;
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

    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord, Math::Vector3 viewDirection) const;
private:
    Util::Color specularColor;
    float phongExponent;
};

#endif
