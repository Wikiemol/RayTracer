#include <iostream>
#include "ColorBuffer.hpp"
#include "Vector3.hpp"
#include "Shape.hpp"
#include "Scene.hpp"
#include "Matrix.hpp"

int main() {
    const unsigned numberOfShapes = 10;
    Vector3 a(1, 2, 3);
    Vector3 b(4, 5, 6);
    unsigned width = 500;
    unsigned height = 500;

    //- Scene camera -//
    Scene::Camera camera;
    camera.direction(0, 0, -1);
    camera.position(0, 0, 1100);
    camera.focalLength = 600;

    //- Scene light -//
    Scene::AreaLight areaLight;
    areaLight.position(1000, 1000, 1000);
    areaLight.radius = 100;
    areaLight.intensity = 1;

    //- Materials -//
    Shape::Material sphereMaterial;
    sphereMaterial.specularity = 1;
    sphereMaterial.diffusion = 1;
    sphereMaterial.shininess = 100;
    sphereMaterial.reflectivity = 0;
    sphereMaterial.red = 50;
    sphereMaterial.green = 50;
    sphereMaterial.blue = 200;

    Shape::Material sphere2Material;
    sphere2Material.specularity = 1;
    sphere2Material.diffusion = 1;
    sphere2Material.shininess = 100;
    sphere2Material.reflectivity = 1;
    sphere2Material.red = 50;
    sphere2Material.green = 200;
    sphere2Material.blue = 50;

    Shape::Material sphere3Material;
    sphere3Material.specularity = 0;
    sphere3Material.diffusion = 1;
    sphere3Material.shininess = 0;
    sphere3Material.reflectivity = 0;
    sphere3Material.red = 200;
    sphere3Material.green = 50;
    sphere3Material.blue = 50;

    Shape::Material sphere4Material;
    sphere4Material.specularity = 0.5;
    sphere4Material.diffusion = 1;
    sphere4Material.shininess = 100;
    sphere4Material.reflectivity = 0.9;
    sphere4Material.red = 50;
    sphere4Material.green = 200;
    sphere4Material.blue = 50;

    Shape::Material planeMaterial;
    planeMaterial.specularity = 0;
    planeMaterial.diffusion = 1;
    planeMaterial.shininess = 0;
    planeMaterial.reflectivity = 0.1;
    planeMaterial.red = 255;
    planeMaterial.green = 255;
    planeMaterial.blue = 255;

    Shape::Material triangleMaterial;
    triangleMaterial.specularity = 0;
    triangleMaterial.diffusion = 1;
    triangleMaterial.shininess = 0;
    triangleMaterial.reflectivity = 0;
    triangleMaterial.red = 50;
    triangleMaterial.green = 200;
    triangleMaterial.blue = 50;

    //- Shapes -//
    Sphere *sphere = new Sphere(100, -100, 0, 100);
    sphere->material = sphereMaterial;

    Sphere *sphere2 = new Sphere(300, -100, 150, 100);
    sphere2->material = sphere2Material;

    Sphere *sphere3 = new Sphere(-50, -100, 150, 100);
    sphere3->material = sphere3Material;

    Sphere *sphere4 = new Sphere(100, -150, 300, 50);
    sphere4->material = sphere4Material;
    
    sphere4->transform(0, 100, 0, 0, 0, 0);

    Vector3 planeNormal(0, 1, 0);
    Plane *plane = new Plane(0, -200, -100, planeNormal);
    plane->material = planeMaterial;
    //plane->transform(0, 0, 0, 0, 0, M_PI/6);


    double pyramidX = -100;
    double pyramidY = -200;
    double pyramidZ = 400;
    Triangle *triangle = new Triangle(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ,
                                      50 + pyramidX, 0 + pyramidY, 100 + pyramidZ,
                                      -100 + pyramidX, 0 + pyramidY, 50 + pyramidZ);
    triangle->material = triangleMaterial;

    Triangle *triangle1 = new Triangle(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ,
                                       100 + pyramidX, 0 + pyramidY, -100 + pyramidZ,
                                       50 + pyramidX, 0 + pyramidY, 100 + pyramidZ);
    triangle1->material = triangleMaterial;

    Triangle *triangle2 = new Triangle(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ,
                                       -100 + pyramidX, 0 + pyramidY, 50 + pyramidZ,
                                       100 + pyramidX, 0 + pyramidY, -100 + pyramidZ
                                       );
    triangle2->material = triangleMaterial;

    double theta = -M_PI / 6;
    triangle->center(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ);
    triangle1->center(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ);
    triangle2->center(0 + pyramidX, 100 + pyramidY, 0 + pyramidZ);

    triangle->transform(100, 100, 100, theta, 0, 0);
    triangle1->transform(100, 100, 100, theta, 0, 0);
    triangle2->transform(100, 100, 100, theta, 0, 0);

    //- Constructing the scene -//
    Scene scene;
    scene.camera = camera;
    scene.areaLight = areaLight;
    scene.addShape(sphere4);
    scene.addShape(sphere3);
    scene.addShape(sphere2);
    scene.addShape(sphere);
    
    scene.addShape(triangle);
    scene.addShape(triangle1);
    scene.addShape(triangle2);
    
    scene.addShape(plane);

    ColorBuffer cBuff(width, height);

    double cameraViewXBound = width / 2;
    double cameraViewYBound = height / 2;
    for (int x = -cameraViewXBound; x < cameraViewXBound; x++) {
        for (int y = -cameraViewYBound; y < cameraViewYBound; y++) {
            int iteration = (x + cameraViewXBound) * height + (y + cameraViewYBound);
            if (iteration % 100 == 0) {
                std::cout << "\r" << (int) (100 * iteration / ((double) width * height))  << "\% complete";
            }

            //- Anti-Aliasing by averaging -//
            Vector3 colorVector1 = scene.getColorAt(x, y);
            Vector3 colorVector2 = scene.getColorAt(x + 0.5, y);
            Vector3 colorVector3 = scene.getColorAt(x + 0.5, y + 0.5);
            Vector3 colorVector4 = scene.getColorAt(x, y + 0.5);

            Vector3 colorVector = (colorVector1 + colorVector2 + colorVector3 + colorVector4) * (1 / 4.0);
            cBuff.setStrokeColor(colorVector[0], colorVector[1], colorVector[2]);
            cBuff.setColorAt(x + width / 2, -y - 1 + height / 2);
        }
    }

    cBuff.writeToFile("pictures/output", ".ppm");
}
