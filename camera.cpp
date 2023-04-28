#include "camera.h"
#include "math.h"
#include <assert.h>

using namespace Math;

Camera::Camera()
{
    this->resolutionX = 64;
    this->resolutionY = 64;
    this->viewPoint = { 0, 0, 0 };
    this->u = { 1,  0, 0 };
    this->v = { 0,  0, 1 };
    this->w = { 0, -1, 0 };
    this->leftBound = -1;
    this->rightBound = 1;
    this->topBound = 1;
    this->bottomBound = -1;
};

Camera::Camera(Vector3 viewPoint, Vector3 u, Vector3 v, Vector3 w, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound)
{
    assert (leftBound < 0 && rightBound > 0);
    assert (bottomBound < 0 && topBound > 0);
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
    this->viewPoint = viewPoint;
    this->u = u;
    this->v = v;
    this->w = w;
    this->leftBound = leftBound;
    this->rightBound = rightBound;
    this->topBound = topBound;
    this->bottomBound = bottomBound;
};

Camera::Camera(Vector3 viewPoint, Vector3 viewingDirection, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound)
{
    assert (leftBound < 0 && rightBound > 0);
    assert (bottomBound < 0 && topBound > 0);
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
    this->viewPoint = viewPoint;
    this->setOrientation(viewingDirection);
    this->leftBound = leftBound;
    this->rightBound = rightBound;
    this->topBound = topBound;
    this->bottomBound = bottomBound;
};

int Camera::getResolutionX() const
{
    return this->resolutionX;
};

int Camera::getResolutionY() const
{
    return this->resolutionY;
};

void Camera::setOrigin(Math::Vector3 origin)
{
    this->viewPoint = origin;
};

void Camera::setResolution(int resolutionX, int resolutionY)
{
    this->resolutionX = resolutionX;
    this->resolutionY = resolutionY;
};

void Camera::setOrientation(Math::Vector3 u, Math::Vector3 v, Math::Vector3 w)
{
    this->u = u;
    this->v = v;
    this->w = w;
};

void Camera::setOrientation(Math::Vector3 viewingDirection)
{
    const Vector3 up = { 0, 0, 1 };

    const Vector3 w = viewingDirection / (-viewingDirection.norm());
    const Vector3 u = cross(up, w) / (cross(up, w)).norm();
    const Vector3 v = cross(w, u);

    this->u = u;
    this->v = v;
    this->w = w;
};

void Camera::setBounds(float leftBound, float rightBound, float topBound, float bottomBound)
{
    assert (leftBound < 0 && rightBound > 0);
    assert (bottomBound < 0 && topBound > 0);
    this->leftBound = leftBound;
    this->rightBound = rightBound;
    this->topBound = topBound;
    this->bottomBound = bottomBound;
};

Ray ParallelOrthographicCamera::computeViewingRay(int pixelIndexX, int pixelIndexY) const
{
    assert ((pixelIndexX >= 0) && (pixelIndexX < this->resolutionX));
    assert ((pixelIndexY >= 0) && (pixelIndexY < this->resolutionY));

    const float uCoordinate = this->leftBound + (this->rightBound - this->leftBound) * (pixelIndexX + 0.5) / this->resolutionX;
    const float vCoordinate = this->bottomBound + (this->topBound - this->bottomBound) * (pixelIndexY + 0.5) / this->resolutionY;

    Ray ray;
    ray.direction = -this->w;
    ray.origin = this->viewPoint + (uCoordinate * this->u) + (vCoordinate * this->v);

    return ray;
};

PerspectiveCamera::PerspectiveCamera(){};

PerspectiveCamera::PerspectiveCamera(Vector3 viewPoint, Vector3 u, Vector3 v, Vector3 w, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound, float focalLength)
    : Camera(viewPoint, u, v, w, resolutionX, resolutionY, leftBound, rightBound, topBound, bottomBound)
{
    this->focalLength = focalLength;
};

PerspectiveCamera::PerspectiveCamera(Vector3 viewPoint, Vector3 viewingDirection, int resolutionX, int resolutionY, float leftBound, float rightBound, float topBound, float bottomBound, float focalLength)
    : Camera(viewPoint, viewingDirection, resolutionX, resolutionY, leftBound, rightBound, topBound, bottomBound)
{
    this->focalLength = focalLength;
};

void PerspectiveCamera::setFocalLength(float focalLength)
{
    this->focalLength = focalLength;
};

Ray PerspectiveCamera::computeViewingRay(int pixelIndexX, int pixelIndexY) const
{
    assert ((pixelIndexX >= 0) && (pixelIndexX < this->resolutionX));
    assert ((pixelIndexY >= 0) && (pixelIndexY < this->resolutionY));

    const float uCoordinate = this->leftBound + (this->rightBound - this->leftBound) * (pixelIndexX + 0.5) / this->resolutionX;
    const float vCoordinate = this->bottomBound + (this->topBound - this->bottomBound) * (pixelIndexY + 0.5) / this->resolutionY;

    Ray ray;
    ray.origin = this->viewPoint;
    ray.direction = -(this->focalLength * this->w) + (uCoordinate * this->u) + (vCoordinate * this->v);

    return ray;
}
