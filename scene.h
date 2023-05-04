#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <string>
#include <vector>
#include "util.h"
#include "camera.h"
#include "surface.h"
#include "material.h"
#include "lightSource.h"

class Scene
{
public:
    Scene(); // by default uses a parallel orthographic camera
    Scene(std::unique_ptr<Camera> camera);

    void setCamera(std::unique_ptr<Camera>);
    void setSurface(std::unique_ptr<Surface>);
    void addLightSource(std::unique_ptr<LightSource> lightSource);

    virtual void render() = 0;
    virtual std::string computePixelArray() const = 0;
    void exportToFile(std::string filename) const;

protected:
    std::unique_ptr<Surface> surface;
    std::unique_ptr<Camera> camera;
    std::vector<std::unique_ptr<LightSource>> lightSources;
};

class GrayscaleScene : public Scene
{
public:
    static uint8_t colorToGrayscale(Util::Color color);

    GrayscaleScene();
    GrayscaleScene(std::unique_ptr<Camera> camera);

    void initializeBitmap();

    void setCamera(std::unique_ptr<Camera> camera);
    void setBackgroundColor(uint8_t backgroundColor);

    void render(); // updates the bitmap. note bitmap(0,0) is at the bottom left of the frame
    std::string computePixelArray() const;

private:
    std::vector<std::vector<uint8_t>> bitmap;
    uint8_t backgroundColor = 0;
    uint8_t computeValueAtPixelIndex(int pixelIndexX, int pixelIndexY) const;
};

class RGBScene : public Scene
{
public:
    RGBScene();
    RGBScene(std::unique_ptr<Camera> camera);

    void initializeBitmap();

    void setCamera(std::unique_ptr<Camera> camera);
    void setBackgroundColor(Util::Color backgroundColor);

    void render();
    std::string computePixelArray() const;

private:
    std::vector<std::vector<Util::Color>> bitmap;
    Util::Color backgroundColor = { 0, 0, 0 };
    Util::Color computeValueAtPixelIndex(int pixelIndexX, int pixelIndexY) const;
};

#endif
