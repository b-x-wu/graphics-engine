#ifndef CAMERA_HEADER
#define CAMERA_HEADER

#include "math.h"

class Camera
{
public:
    Camera(Math::Vector3 viewPoint, Math::Vector3 u, Math::Vector3 v, Math::Vector3 w);
    // places camera at viewPoint and orients it facing the viewing direction with v pointing up
    Camera(Math::Vector3 viewPoint, Math::Vector3 viewingDirection);

    void setOrigin(Math::Vector3 origin);
    void setOrientation(Math::Vector3 u, Math::Vector3 v, Math::Vector3 w);
    void setOrientation(Math::Vector3 viewingDirection); // points the camera toward the viewing direction
    void setBounds(float leftBound, float rightBound, float upBound, float downBound);

    Math::Vector3 computePixelPosition(int pixelIndexX, int pixelIndexY) const;
    virtual Math::Ray computeViewingRay(int pixelIndexX, int pixelIndexY) const;    

private:
    int resolutionX, resolutionY;
    Math::Vector3 viewPoint; // labeled e in the text
    // orthonormal basis of the camera
    // up points right, v upwards, and w against the direction of the camera
    Math:: Vector3 u, v, w;
    float leftBound, rightBound, upBound, downBound; // labeled l, r, u, d in the text
};

#endif
