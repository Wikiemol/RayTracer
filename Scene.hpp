#ifndef SCENE_HPP
#define SCENE_HPP

#include "Shape.hpp"
#include "Vector3.hpp"

class Scene {
public:
    struct Camera {
        Vector3 direction;
        Vector3 position;
        double focalLength;
    };

    struct PointLight {
        Vector3 position;
        double intensity;
    };

    Camera camera;
    PointLight pointLight;
    Scene();
    ~Scene();
    void addShape(Shape *shape);
    Vector3 getColorAt(double x, double y);
    int reflectionDepth;
private:
    Shape** shapeBuffer;
    unsigned numberOfShapes;
    unsigned shapeBufferSize;
    void resizeShapeBuffer(unsigned newSize);
    Vector3 castRay(const Ray &ray, unsigned numberOfTimesRecursed) const;
};

Scene::Scene() {
    shapeBufferSize = 2;
    shapeBuffer = new Shape*[shapeBufferSize];
    numberOfShapes = 0;
    reflectionDepth = 3;
}

Scene::~Scene() {
    for (unsigned i = 0; i < shapeBufferSize; i++)
        delete shapeBuffer[i];

    delete[] shapeBuffer;
}

void Scene::addShape(Shape *shape) {
    //- if shapeBuffer is full, resize shapeBuffer to twice its size -//
    if (shapeBufferSize == numberOfShapes)
        resizeShapeBuffer(shapeBufferSize * 2);

    shapeBuffer[numberOfShapes] = shape;
    numberOfShapes++;
}

Vector3 Scene::getColorAt(double x, double y) {
    //- current point on lens plane -//
    Vector3 pointOnLensPlane(x, y, -camera.focalLength);

    //- cast ray -//
    Ray rayFromCameraToLens;
    rayFromCameraToLens.position = camera.position;
    rayFromCameraToLens.direction = pointOnLensPlane.normalise();
    return castRay(rayFromCameraToLens, 0);
}

//- returns a vector representing color -//
Vector3 Scene::castRay(const Ray &mainRay, unsigned numberOfTimesRecursed) const {
    //-find closest intersection/closest shape-//
    Shape::Intersection shapeIntersection;
    Shape *closestShape;
    double distanceFromIntersectionToRay;

    unsigned closestIndex;
    for (unsigned i = 0; i < numberOfShapes && shapeBuffer[i] != NULL; i++) {

        Shape::Intersection currentShapeIntersection = shapeBuffer[i]->intersect(mainRay);

        if (!currentShapeIntersection.intersection) {
            //- Do nothing if currentShapeIntersection is undefined. The ray intersected nothing -//
        } else {
            double currentDistanceFromIntersectionToRay = (currentShapeIntersection.intersection - mainRay.position) 
                                                        * (currentShapeIntersection.intersection - mainRay.position);

            if (!shapeIntersection.intersection || currentDistanceFromIntersectionToRay < distanceFromIntersectionToRay) {
                shapeIntersection = currentShapeIntersection;
                closestShape = shapeBuffer[i];
                distanceFromIntersectionToRay = currentDistanceFromIntersectionToRay;
                closestIndex = i;
            }
        }
    }

    //- if intersected, set cBuffColor -//
    if (!shapeIntersection.intersection.isUndefined()) {
        //- We mustn't normalize the directionToLight vector yet, as we need its full length
        //- to test for shadows.
        Vector3 directionToLight = (pointLight.position - shapeIntersection.intersection);
        Ray rayFromShapeToLight;
        rayFromShapeToLight.position = shapeIntersection.intersection;
        rayFromShapeToLight.direction = directionToLight;

        bool inShadow = false;

        //- See if light ray intersects with another shape. If so, a shadow must be cast -//
        for (unsigned i = 0; i < numberOfShapes && shapeBuffer[i] != NULL; i++) {
            Shape::Intersection lightRayIntersection = shapeBuffer[i]->intersect(rayFromShapeToLight);
            if (!lightRayIntersection.intersection.isUndefined() && lightRayIntersection.time < 1) {
                inShadow = true;
                break;
            }
        }

        //- Now we can normalise the vector from the light to the shapeIntersection -//
        directionToLight = directionToLight.normalise();

        //- if there is no shadow, set cBuffColor -//
        if (!inShadow) {
            Vector3 normal = closestShape->getNormalAt(shapeIntersection.intersection);
            Vector3 lightRayReflected = directionToLight.reflectOver(normal);
            Vector3 directionToViewer = (camera.position - shapeIntersection.intersection).normalise();
            Shape::Material material = closestShape->material;

            double diffuseComponent = directionToLight * normal;

            //- using phong illumination -//
            double illumination = 0;

            if (diffuseComponent > 0) {
                illumination += material.diffusion * (diffuseComponent) 
                             +  material.specularity * pow(directionToViewer * lightRayReflected, material.shininess);
            }

            illumination *= pointLight.intensity;

            double r = material.red * illumination;
            double g = material.green * illumination;
            double b = material.blue * illumination;

            if (r > 255)
                r = 255;
            if (g > 255)
                g = 255;
            if (b > 255)
                b = 255;

            Vector3 colorVector(r, g, b);

            if (numberOfTimesRecursed < reflectionDepth && material.reflectivity != 0) {
                Vector3 directionToViewerReflected;
                directionToViewerReflected = mainRay.direction * (-1);
                directionToViewerReflected = directionToViewerReflected.reflectOver(normal);

                Ray rayReflected = mainRay;
                rayReflected.position = shapeIntersection.intersection;
                rayReflected.direction = directionToViewerReflected;

                Vector3 reflectionColor = castRay(rayReflected, ++numberOfTimesRecursed) * material.reflectivity;
                colorVector = colorVector * (1 - material.reflectivity);
                colorVector = colorVector + reflectionColor;
            }

            return colorVector;
        } else {
            Vector3 colorVector(0, 0, 0);
            return colorVector;
        }
    }
    Vector3 backgroundVector(0, 0, 0);
    return backgroundVector;
}

void Scene::resizeShapeBuffer(unsigned newSize) {
    shapeBufferSize = newSize;
    Shape **newShapeBuffer = new Shape*[shapeBufferSize];

    for (unsigned i = 0; i < numberOfShapes; i++)
        newShapeBuffer[i] = shapeBuffer[i];

    delete[] shapeBuffer;
    shapeBuffer = newShapeBuffer;
}

#endif