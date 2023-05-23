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
    virtual ~Shader() = default;
    virtual Util::Color computeColor(
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
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
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
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
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
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
    BlinnPhongShader(double phongExponent);
    BlinnPhongShader(Util::Color specularColor);
    BlinnPhongShader(double phongExponent, Util::Color specularColor);

    double getPhongExponent() const;
    Util::Color getSpecularColor() const;

    void setPhongExponent(double phongExponent);
    void setSpecularColor(Util::Color specularColor);

    Util::Color computeColor(
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color specularColor;
    double phongExponent;
};

// combination of lambert, blinn-phong, and ambient shading
class StandardShader : public Shader
{
public:
    StandardShader();
    StandardShader(double ambientIntensity, Util::Color ambientColor);
    StandardShader(double ambientIntensity, double phongExponent);
    StandardShader(Util::Color surfaceColor, Util::Color specularColor, Util::Color ambientColor);
    StandardShader(double ambientIntensity, Util::Color ambientColor, double phongExponent, Util::Color surfaceColor, Util::Color specularColor);

    double getAmbientIntensity() const;
    double getPhongExponent() const;
    Util::Color getSurfaceColor() const;
    Util::Color getSpecularColor() const;
    Util::Color getAmbientColor() const;

    void setAmbientIntensity(double ambientIntensity);
    void setPhongExponent(double phongExponent);
    void setSurfaceColor(Util::Color surfaceColor);
    void setSpecularColor(Util::Color specularColor);
    void setAmbientColor(Util::Color ambientColor);

    Util::Color computeColor(
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color surfaceColor, specularColor, ambientColor;
    double ambientIntensity, phongExponent;
};

class MirrorShader : public Shader
{
public:
    MirrorShader();
    MirrorShader(double specularWeight);
    MirrorShader(Util::Color backgroundColor, Util::Color specularColor, double specularWeight);

    void setBackgroundColor(Util::Color backgroundColor);
    void setSpecularColor(Util::Color specularColor);
    void setSpecularWeight(double specularWeight);

    Util::Color computeColor(
        const std::vector<std::reference_wrapper<LightSource>> &lightSources,
        Math::Ray viewRay,
        std::shared_ptr<Renderable> surface,
        std::shared_ptr<Util::HitRecord> hitRecord
    ) const;
private:
    Util::Color backgroundColor = { 255, 255, 255 };
    Util::Color specularColor = { 0, 0, 0 };
    double specularWeight = 0; // number in [0, 1] that defines how much of the total is specular color and how much is reflected color
};

#endif
