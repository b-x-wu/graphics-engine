#ifndef SCENE_HEADER
#define SCENE_HEADER

#include <string>
#include <vector>
#include "camera.h"

class Scene
{
public:
    Scene(); // by default uses a parallel orthographic camera
    Scene(std::unique_ptr<Camera> camera);

    void setCamera(std::unique_ptr<Camera>);

    virtual void render() = 0;
    virtual std::string computePixelArray() const = 0;
    void exportToFile(std::string filename) const;

protected:
    // Surface surfaces;
    std::unique_ptr<Camera> camera;
};

class GrayscaleScene: public Scene
{
public:
    GrayscaleScene();
    GrayscaleScene(std::unique_ptr<Camera> camera);

    void initializeBitmap();

    void setCamera(std::unique_ptr<Camera> camera);

    void render(); // updates the bitmap. note bitmap(0,0) is at the bottom left of the frame
    std::string computePixelArray() const;

private:
    std::vector<std::vector<uint8_t>> bitmap;

    uint8_t computeValueAtPixelIndex(int pixelIndexX, int pixelIndexY) const;
};

#endif
