#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <string>
#include <vector>
#include "util.h"
#include "camera.h"
#include "surface.h"
#include "shader.h"
#include "lightSource.h"

class Scene
{
public:
    Scene(); // by default uses a parallel orthographic camera
    Scene(std::shared_ptr<Camera> camera);

    virtual ~Scene() = default;

    void setCamera(std::shared_ptr<Camera>);
    void setSurface(std::shared_ptr<Surface>);

    void addLightSource(LightSource& lightSource);
    LightSource& getLightSource(size_t idx);
    void removeLightSource(size_t idx);

    virtual void render() = 0;
    virtual std::string computePixelArray() const = 0;
    void exportToFile(std::string filename) const;

protected:
    std::shared_ptr<Surface> surface;
    std::shared_ptr<Camera> camera;
    std::vector<std::reference_wrapper<LightSource>> lightSources;
};

class GrayscaleScene : public Scene
{
public:
    static uint8_t colorToGrayscale(Util::Color color);

    GrayscaleScene();
    GrayscaleScene(std::shared_ptr<Camera> camera);

    void initializeBitmap();

    void setCamera(std::shared_ptr<Camera> camera);
    void setBackgroundColor(uint8_t backgroundColor);

    std::vector<std::vector<uint8_t>> getBitmap() const;

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
    RGBScene(std::shared_ptr<Camera> camera);

    void initializeBitmap();

    void setCamera(std::shared_ptr<Camera> camera);
    void setBackgroundColor(Util::Color backgroundColor);

    std::vector<std::vector<Util::Color>> getBitmap() const;

    void render();
    std::string computePixelArray() const;

private:
    std::vector<std::vector<Util::Color>> bitmap;
    Util::Color backgroundColor = { 0, 0, 0 };
    Util::Color computeValueAtPixelIndex(int pixelIndexX, int pixelIndexY) const;
};

#endif
