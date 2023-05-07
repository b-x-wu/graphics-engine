#ifndef SHADER_HEADER
#define SHADER_HEADER

#include "util.h"
#include "lightSource.h"
#include "hittable.h"
#include <memory>
#include <vector>

class Shader
{
public:
    virtual Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const = 0;
};

class StaticColorShader : public Shader
{
public:
    StaticColorShader();
    StaticColorShader(Util::Color surfaceColor);

    Util::Color getSurfaceColor() const;

    void setSurfaceColor(Util::Color color);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
protected:
    Util::Color surfaceColor;
};

class LambertShader : public StaticColorShader
{
public:
    LambertShader();
    LambertShader(Util::Color surfaceColor);
    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
};

class BlinnPhongShader : public Shader
{
// TODO: add an option to make the reflective color the color of the light
public:
    BlinnPhongShader();
    BlinnPhongShader(float phongExponent);
    BlinnPhongShader(Util::Color specularColor);
    BlinnPhongShader(float phongExponent, Util::Color specularColor);

    float getPhongExponent() const;
    Util::Color getSpecularColor() const;

    void setPhongExponent(float phongExponent);
    void setSpecularColor(Util::Color specularColor);

    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color specularColor;
    float phongExponent;
};

// combination of lambert, blinn-phong, and ambient shading
class StandardShader : public Shader
{
public:
    StandardShader();
    StandardShader(float ambientIntensity, Util::Color ambientColor);
    StandardShader(float ambientIntensity, float phongExponent);
    StandardShader(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor);
    StandardShader(float ambientIntensity, Util::Color ambientColor, float phongExponent, Util::Color surfaceColor, Util::Color specularColor);

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
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color surfaceColor, specularColor, ambientColor;
    float ambientIntensity, phongExponent;
};

class MirrorShader : public Shader
{
public:
    MirrorShader();
    MirrorShader(float specularWeight);
    MirrorShader(Util::Color backgroundColor, Util::Color specularColor, float specularWeight);

    void setBackgroundColor(Util::Color backgroundColor);
    void setSpecularColor(Util::Color specularColor);
    void setSpecularWeight(float specularWeight);

    // TODO: figure out a way to add a specular component
    Util::Color computeColor(
        const std::vector<std::unique_ptr<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color backgroundColor = { 255, 255, 255 };
    Util::Color specularColor = { 0, 0, 0 };
    float specularWeight = 0; // number in [0, 1] that defines how much of the total is specular color and how much is reflected color
};

#endif
