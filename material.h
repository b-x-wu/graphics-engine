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
                                     std::unique_ptr<Util::HitRecord> hitRecord) const = 0;
};

class StaticColorMaterial : public Material
{
public:
    StaticColorMaterial();
    StaticColorMaterial(Util::Color surfaceColor);

    Util::Color getSurfaceColor() const;

    void setSurfaceColor(Util::Color color);

    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord) const;
protected:
    Util::Color surfaceColor;
};

class LambertShaderMaterial : public StaticColorMaterial
{
public:
    LambertShaderMaterial();
    LambertShaderMaterial(Util::Color surfaceColor);

    Util::Color computeColor(const std::vector<std::unique_ptr<LightSource>> & lightSources, std::unique_ptr<Util::HitRecord> hitRecord) const;
};

#endif
