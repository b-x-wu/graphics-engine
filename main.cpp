#include <iostream>
#include <assert.h>
#include <cmath>
#include <memory>

#include "math.h"
#include "camera.h"
#include "scene.h"
#include "surface.h"

using namespace Math;

int testMath()
{
    Vector3 v1 = { 1, 2, 3 };
    assert (v1.getX() == 1);
    assert (v1.getY() == 2);
    assert (v1.getZ() == 3);
    assert (std::abs(v1.norm() - 3.74165) < 0.001);

    Vector3 v2 = { -1, -2, -3 };
    Vector3 vSum = v1 + v2;
    assert (vSum.getX() == 0);
    assert (vSum.getY() == 0);
    assert (vSum.getZ() == 0);

    Vector3 vDifference = v1 - v2;
    assert (vDifference.getX() == 2);
    assert (vDifference.getY() == 4);
    assert (vDifference.getZ() == 6);

    Vector3 vNegative = -v1;
    assert (vNegative.getX() == -1);
    assert (vNegative.getY() == -2);
    assert (vNegative.getZ() == -3);

    v1 += { 1, 2, 3 };
    assert (v1.getX() == 2);
    assert (v1.getY() == 4);
    assert (v1.getZ() == 6);

    v2 -= { 1, 2, 3 };
    assert (v2.getX() == -2);
    assert (v2.getY() == -4);
    assert (v2.getZ() == -6);

    Vector3 vProduct = v1 * 2;
    assert (vProduct.getX() == 4);
    assert (vProduct.getY() == 8);
    assert (vProduct.getZ() == 12);

    Vector3 vQuotient = v1 / 2;
    assert (vQuotient.getX() == 1);
    assert (vQuotient.getY() == 2);
    assert (vQuotient.getZ() == 3);

    Vector3 v1Clone = { 2, 4, 6 };
    assert (v1 == v1Clone);
    assert (v1 != v2);
    assert ((2 * v1) == (v1 * 2));

    assert (dot(v1, v2) == -56);

    Vector3 vCross = { 0, 0, 0 };
    assert (cross(v1, v2) == vCross);

    Box box = Box({ -1, 1, -1 }, { 1, -1, 1 });
    assert (box.min.getX() == -1);
    assert (box.min.getY() == -1);
    assert (box.min.getZ() == -1);
    assert (box.max.getX() == 1);
    assert (box.max.getY() == 1);
    assert (box.max.getZ() == 1);
    assert (box.isInside({ 0, 0, 0 }));
    assert (!box.isInside({ 10, 10, 10 }));

    std::cout << "All tests passed.";
    return 0;
}

int main()
{
    // testMath();

    GrayscaleScene grayscaleScene = GrayscaleScene();
    // std::unique_ptr<Sphere> sphere(new Sphere(20, { 30, 0, 0 }));
    std::unique_ptr<Triangle> triangle(new Triangle({ 10, 10, 10 }, { 10, -10, 10 }, { 10, 10, -10 }));
    std::unique_ptr<ParallelOrthographicCamera> camera(new ParallelOrthographicCamera());
    camera->setOrigin({ 0, 0, 0 });
    camera->setOrientation({ 1, 0, 0 });
    camera->setResolution(640, 360);
    camera->setBounds(-320, 320, 180, -180);
    grayscaleScene.setCamera(std::move(camera));
    // grayscaleScene.setSurface(std::move(sphere));
    grayscaleScene.setSurface(std::move(triangle));
    grayscaleScene.render();
    grayscaleScene.exportToFile("test_render.bmp");

    return 0;
}
