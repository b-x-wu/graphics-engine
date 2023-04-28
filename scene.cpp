#include <memory>
#include <iostream>
#include <fstream>
#include "scene.h"
#include "camera.h"

// https://stackoverflow.com/a/47785639/21190150

const int BYTES_PER_PIXEL = 3;
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

Scene::Scene()
{
    this->camera = std::unique_ptr<Camera>(new ParallelOrthographicCamera());
}

Scene::Scene(std::unique_ptr<Camera> camera)
{
    this->camera = std::move(camera);
};

void Scene::setCamera(std::unique_ptr<Camera> camera)
{
    this->camera = std::move(camera);
};

void Scene::exportToFile(std::string filename) const
{
    int width = this->camera->getResolutionX();
    int height = this->camera->getResolutionY();
    int widthInBytes = width * BYTES_PER_PIXEL;
    char padding[3] = { 0, 0, 0 };
    int paddingSize = (4 - (widthInBytes % 4)) % 4;
    int scanLineSize = widthInBytes + paddingSize;
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (scanLineSize * this->camera->getResolutionY());

    // create file header
    std::string fileHeader = std::string(FILE_HEADER_SIZE, (char) 0);
    fileHeader.replace(0, 1, 1, 'B');
    fileHeader.replace(1, 1, 1, 'M');
    fileHeader.replace(2, 1, 1, (char) fileSize);
    fileHeader.replace(3, 1, 1, (char) (fileSize >> 8));
    fileHeader.replace(4, 1, 1, (char) (fileSize >> 16));
    fileHeader.replace(5, 1, 1, (char) (fileSize >> 24));
    fileHeader.replace(10, 1, 1, (char) (FILE_HEADER_SIZE + INFO_HEADER_SIZE));

    // create info header
    std::string infoHeader = std::string(INFO_HEADER_SIZE, 0);
    infoHeader.replace(0, 1, 1, INFO_HEADER_SIZE);
    infoHeader.replace(4, 1, 1, (char) width);
    infoHeader.replace(5, 1, 1, (char) (width >> 8));
    infoHeader.replace(6, 1, 1, (char) (width >> 16));
    infoHeader.replace(7, 1, 1, (char) (width >> 24));
    infoHeader.replace(8, 1, 1, (char) height);
    infoHeader.replace(9, 1, 1, (char) (height >> 8));
    infoHeader.replace(10, 1, 1, (char) (height >> 16));
    infoHeader.replace(11, 1, 1, (char) (height >> 24));
    infoHeader.replace(12, 1, 1, (char) 1);
    infoHeader.replace(14, 1, 1, (char) (BYTES_PER_PIXEL * 8));

    const std::string pixelArray = this->computePixelArray();

    // write out
    std::ofstream file(filename, std::ios::out | std::ios::binary | std::ios::trunc);

    if (file.is_open())
    {
        file.write(fileHeader.c_str(), FILE_HEADER_SIZE);
        file.write(infoHeader.c_str(), INFO_HEADER_SIZE);
        for (int i = 0; i < this->camera->getResolutionY(); i++)
        {
            file.write(pixelArray.c_str() + (i * widthInBytes), widthInBytes);
            file.write(padding, paddingSize);
        }
    }

    file.close();
}

GrayscaleScene::GrayscaleScene()
{
    this->initializeBitmap();
};

GrayscaleScene::GrayscaleScene(std::unique_ptr<Camera> camera): Scene(std::move(camera))
{
    this->initializeBitmap();
};

void GrayscaleScene::initializeBitmap()
{
    this->bitmap = std::vector<std::vector<uint8_t>>(this->camera->getResolutionY(), std::vector<uint8_t>(this->camera->getResolutionX(), 0));
}

void GrayscaleScene::setCamera(std::unique_ptr<Camera> camera)
{
    Scene::setCamera(std::move(camera));
    this->initializeBitmap();
}

uint8_t GrayscaleScene::computeValueAtPixelIndex(int pixelIndexX, int pixelIndexY) const
{
    // TODO: once surfaces are calculated, change this
    return 255;
}

void GrayscaleScene::render()
{
    for (int j = 0; j < this->camera->getResolutionY(); j++)
    {
        for (int i = 0; i < this->camera->getResolutionX(); i++)
        {
            this->bitmap.at(j).at(i) = this->computeValueAtPixelIndex(i, j);
        }
    }
}

std::string GrayscaleScene::computePixelArray() const
{
    std::string pixelArray(this->camera->getResolutionX() * this->camera->getResolutionY() * BYTES_PER_PIXEL, 0);
    int idx = 0;
    uint8_t bitmapValue = 0;
    for (int i = 0; i < this->camera->getResolutionY(); i++)
    {
        for (int j = 0; j < this->camera->getResolutionX(); j++)
        {
            idx = 3 * (i * this->camera->getResolutionX() + j);
            bitmapValue = this->bitmap.at(i).at(j);
            pixelArray.replace(idx, 3, 3, (char) bitmapValue);
        }
    }
    return pixelArray;
};
