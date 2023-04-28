#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "math.h"

class Camera
{
public:
    // TODO: probably requires a builder class
    Camera(); // orients camera facing into the screen at the origin
    Camera(Math::Vector3 viewPoint, Math::Vector3 u, Math::Vector3 v, Math::Vector3 w, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound);
    // places camera at viewPoint and orients it facing the viewing direction with v pointing up
    Camera(Math::Vector3 viewPoint, Math::Vector3 viewingDirection, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound);

    int getResolutionX() const;
    int getResolutionY() const;

    void setOrigin(Math::Vector3 origin);
    void setResolution(int resolutionX, int resolutionY);
    void setOrientation(Math::Vector3 u, Math::Vector3 v, Math::Vector3 w);
    void setOrientation(Math::Vector3 viewingDirection); // points the camera toward the viewing direction
    void setBounds(float leftBound, float rightBound, float topBound, float bottomBound);

    virtual Math::Ray computeViewingRay(int pixelIndexX, int pixelIndexY) const = 0;

protected:
    int resolutionX, resolutionY;
    Math::Vector3 viewPoint; // labeled e in the text
    // orthonormal basis of the camera
    // up points right, v upwards, and w against the direction of the camera
    Math:: Vector3 u, v, w;
    float leftBound, rightBound, topBound, bottomBound; // labeled l, r, u, d in the text
};

class ParallelOrthographicCamera: public Camera
{
public:
    Math::Ray computeViewingRay(int pixelIndexX, int pixelIndexY) const;
};

class PerspectiveCamera: public Camera
{
public:
    PerspectiveCamera();
    PerspectiveCamera(Math::Vector3 viewPoint, Math::Vector3 u, Math::Vector3 v, Math::Vector3 w, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound, float focalLength);
    PerspectiveCamera(Math::Vector3 viewPoint, Math::Vector3 viewingDirection, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound, float focalLength);

    void setFocalLength(float focalLength);

    Math::Ray computeViewingRay(int pixelIndexX, int pixelIndexY) const;

protected:
    float focalLength = 0;
};

#endif
