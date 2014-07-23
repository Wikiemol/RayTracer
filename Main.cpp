#include <iostream>
#include "ColorBuffer.hpp"
#include "Vector.hpp"
#include "Shape.hpp"
#include "Scene.hpp"

int main() {
    const unsigned numberOfShapes = 10;
    Shape shapes[numberOfShapes];

    unsigned width = 500;
    unsigned height = 500;

    //- Scene camera -//
    Scene::Camera camera;
    camera.direction(0, 0, -1);
    camera.position(0, 0, 1100);
    camera.focalLength = 600;

    //- Scene light -//
    Scene::PointLight pointLight;
    pointLight.position(1000, 1000, 1000);
    //pointLight.position(0, 10, 0);
    pointLight.intensity = 1;

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

    //- Shapes -//
    Shape sphere(Shape::SPHERE);
    sphere.position(100, -100, 0);
    sphere.radius = 100;
    sphere.material = sphereMaterial;

    Shape sphere2(Shape::SPHERE);
    sphere2.position(300, -100, 150);
    sphere2.radius = 100;
    sphere2.material = sphere2Material;

    Shape sphere3(Shape::SPHERE);
    sphere3.position(-50, -100, 150);
    sphere3.radius = 100;
    sphere3.material = sphere3Material;

    Shape sphere4(Shape::SPHERE);
    sphere4.position(100, -150, 300);
    sphere4.radius = 50;
    sphere4.material = sphere4Material;

    Shape plane(Shape::PLANE);
    plane.position(0, -200, -100);
    plane.normal(0, 1, 0);
    plane.material = planeMaterial;

    //- Constructing the scene -//
    Scene scene;
    scene.camera = camera;
    scene.pointLight = pointLight;
    scene.addShape(sphere4);
    scene.addShape(sphere3);
    scene.addShape(sphere2);
    scene.addShape(sphere);
    scene.addShape(plane);

    ColorBuffer cBuff(width, height);

    double xBound = width / 2;
    double yBound = height / 2;
    for (int x = -xBound; x < xBound; x++) {
        for (int y = -yBound; y < yBound; y++) {
            //- Anti-Aliasing by averaging -//
            Vector colorVector1 = scene.getColorAt(x, y);
            Vector colorVector2 = scene.getColorAt(x + 0.5, y);
            Vector colorVector3 = scene.getColorAt(x + 0.5, y + 0.5);
            Vector colorVector4 = scene.getColorAt(x, y + 0.5);

            Vector colorVector = (colorVector1 + colorVector2 + colorVector3 + colorVector4) * (1 / 4.0);
            cBuff.setStrokeColor(colorVector[0], colorVector[1], colorVector[2]);
            cBuff.setColorAt(x + width / 2, -y - 1 + height / 2);
        }
    }

    cBuff.writeToFile("pictures/output", ".ppm");
}