#include <iostream>
#include <assert.h>
#include <cmath>
#include <memory>

#include "math.h"
#include "camera.h"
#include "scene.h"
#include "surface.h"
#include "shader.h"
#include "lightSource.h"

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

int testGrayscaleScene()
{
    GrayscaleScene grayscaleScene = GrayscaleScene();

    std::unique_ptr<Sphere> sphere(new Sphere(2, { 8, 5, 5 }));
    std::unique_ptr<Triangle> triangle(new Triangle({ 10, -2, 4 }, { 20, 2, 4 }, { 10, 2, -4 }));
    std::unique_ptr<GroupSurface> groupSurface(new GroupSurface());
    groupSurface->addSurface(std::move(triangle));
    groupSurface->addSurface(std::move(sphere));

    // std::unique_ptr<LambertShader> shader(new LambertShader({ 255, 255, 255 }));
    // std::unique_ptr<BlinnPhongShader> shader(new BlinnPhongShader(10));
    std::unique_ptr<StandardShader> shader(new StandardShader(0.1, { 255, 255, 255 }, 50, { 255, 255, 255 }, { 255, 255, 255 }));
    groupSurface->setMaterial(std::move(shader));

    // std::unique_ptr<ParallelOrthographicCamera> camera(new ParallelOrthographicCamera());
    // camera->setOrigin({ 0, 0, 0 });
    std::unique_ptr<PerspectiveCamera> camera(new PerspectiveCamera());
    camera->setOrigin({ -10, 0, 0 });
    camera->setFocalLength(10);
    camera->setOrientation({ 1, 0, 0 });
    camera->setResolution(1920, 1080);
    camera->setBounds(-16, 16, 9, -9);

    std::unique_ptr<LightSource> lightSource(new UnidirectionalLightSource({ 1, 0, 0 }));
    // std::unique_ptr<LightSource> lightSource(new PointLightSource({ 0, 0, 0 }));
    lightSource->setIntensity(1);

    grayscaleScene.addLightSource(std::move(lightSource));
    grayscaleScene.setCamera(std::move(camera));
    grayscaleScene.setSurface(std::move(groupSurface));
    grayscaleScene.render();
    grayscaleScene.exportToFile("test_render.bmp");

    return 0;
}

int testRGBScene()
{
    RGBScene rgbScene = RGBScene();

    std::unique_ptr<Sphere> sphere1(new Sphere(2, { 20, -4, 2 }));
    std::unique_ptr<Shader> sphereMaterial1(new StandardShader(0.2, { 0, 255, 0 }, 10, { 0, 255, 0 }, { 255, 255, 255 }));
    sphere1->setMaterial(std::move(sphereMaterial1));

    std::unique_ptr<Sphere> sphere2(new Sphere(3, { 15, 5, 3 }));
    std::unique_ptr<Shader> sphereMaterial2(new StandardShader(0.2, { 0, 0, 255 }, 10, { 0, 0, 255 }, { 255, 255, 255 }));
    sphere2->setMaterial(std::move(sphereMaterial2));

    std::unique_ptr<Triangle> triangle(new Triangle({ 100, 0, 0 }, { -300, -100, 0 }, { -300, 100, 0 }, { 0, 0, 1 }));
    std::unique_ptr<Shader> triangleMaterial(new MirrorShader({ 255, 255, 255 }, { 100, 100, 255 }, 0.1));
    // std::unique_ptr<Shader> triangleMaterial(new StandardShader( 0.2, {125, 125, 125}, 10, {125, 125, 125}, {255, 255, 255}));
    triangle->setMaterial(std::move(triangleMaterial));

    std::unique_ptr<GroupSurface> groupSurface(new GroupSurface());
    groupSurface->addSurface(std::move(sphere1));
    groupSurface->addSurface(std::move(sphere2));
    groupSurface->addSurface(std::move(triangle));

    // std::unique_ptr<LambertShader> shader(new LambertShader({ 255, 255, 255 }));
    // std::unique_ptr<BlinnPhongShader> shader(new BlinnPhongShader(10));
    // std::unique_ptr<StandardShader> shader(new StandardShader(0.1, { 0, 0, 255 }, 50, { 255, 0, 0 }, { 0, 255, 0 }));
    std::unique_ptr<StaticColorShader> shader(new StaticColorShader({ 255, 0, 0 }));
    groupSurface->setMaterial(std::move(shader));

    // std::unique_ptr<ParallelOrthographicCamera> camera(new ParallelOrthographicCamera());
    // camera->setOrigin({ 0, 0, 5 });
    std::unique_ptr<PerspectiveCamera> camera(new PerspectiveCamera());
    camera->setOrigin({ -10, 0, 5 });
    camera->setFocalLength(10);
    camera->setOrientation({ 1, 0, -0.2 });
    camera->setResolution(1920, 1080);
    camera->setBounds(-16, 16, 9, -9);

    // std::unique_ptr<LightSource> lightSource(new UnidirectionalLightSource({ 1, 0, 0 }));
    // std::unique_ptr<LightSource> lightSource1(new PointLightSource({ -10, 0, 0 }));
    // lightSource1->setIntensity(1);
    std::unique_ptr<LightSource> lightSource2(new UnidirectionalLightSource({ 1, -1, -1 }));
    lightSource2->setIntensity(1);


    rgbScene.setBackgroundColor({ 255, 255, 255 });
    // rgbScene.addLightSource(std::move(lightSource1));
    rgbScene.addLightSource(std::move(lightSource2));
    rgbScene.setCamera(std::move(camera));
    rgbScene.setSurface(std::move(groupSurface));
    rgbScene.render();
    rgbScene.exportToFile("test_render.bmp");

    return 0;
}

int main()
{
    // testMath();
    // testGrayscaleScene();
    testRGBScene();

    return 0;
}
