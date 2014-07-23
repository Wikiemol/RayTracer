#ifndef SCENE_HPP
#define SCENE_HPP

#include "Shape.hpp"
#include "Vector.hpp"

class Scene {
public:
    struct Camera {
        Vector direction;
        Vector position;
        double focalLength;
    };

    struct PointLight {
        Vector position;
        double intensity;
    };

    Camera camera;
    PointLight pointLight;
    Scene();
    ~Scene();
    void addShape(const Shape &shape);
    Vector getColorAt(double x, double y);
    int reflectionDepth;
private:
    Shape* shapeBuffer;
    unsigned numberOfShapes;
    unsigned shapeBufferSize;
    void resizeShapeBuffer(unsigned newSize);
    Vector castRay(const Ray &ray, unsigned numberOfTimesRecursed) const;
};

Scene::Scene() {
    shapeBufferSize = 2;
    shapeBuffer = new Shape[shapeBufferSize];
    numberOfShapes = 0;
    reflectionDepth = 3;
}

Scene::~Scene() {
    delete[] shapeBuffer;
    shapeBuffer = NULL;
}

void Scene::addShape(const Shape &shape) {
    //- if shapeBuffer is full, resize shapeBuffer to twice its size -//
    if (shapeBufferSize == numberOfShapes) {
        resizeShapeBuffer(shapeBufferSize * 2);
    }

    shapeBuffer[numberOfShapes] = shape;
    numberOfShapes++;
}

Vector Scene::getColorAt(double x, double y) {
    //- current point on lens plane -//
    Vector pointOnLensPlane(x, y, -camera.focalLength);

    //- cast ray -//
    Ray ray;
    ray.position = camera.position;
    ray.direction = pointOnLensPlane.normalise();
    return castRay(ray, 0);
}

//- returns a vector representing color -//
Vector Scene::castRay(const Ray &ray, unsigned numberOfTimesRecursed) const{
    //-find closest intersection/closest shape-//
    Shape::Intersection shapeIntersection;
    Shape closestShape;
    double distanceFromIntersectionToRay;

    unsigned closestIndex;
    for (unsigned i = 0; i < numberOfShapes && shapeBuffer[i].type != Shape::UNDEFINED; i++) {

        Shape::Intersection currentShapeIntersection = shapeBuffer[i].intersect(ray);

        if (!currentShapeIntersection.intersection) {
            //- Do nothing if currentShapeIntersection is undefined. The ray intersected nothing -//
        } else {
            double currentDistanceFromIntersectionToRay = (currentShapeIntersection.intersection - ray.position) 
                                                        * (currentShapeIntersection.intersection - ray.position);

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
        Vector directionToLight = (pointLight.position - shapeIntersection.intersection);
        Ray rayFromShapeToLight;
        rayFromShapeToLight.position = shapeIntersection.intersection;
        rayFromShapeToLight.direction = directionToLight;

        bool inShadow = false;

        //- See if light ray intersects with another shape. If so, a shadow must be cast -//
        for (unsigned i = 0; i < numberOfShapes && shapeBuffer[i].type != Shape::UNDEFINED; i++) {
            Shape::Intersection lightRayIntersection = shapeBuffer[i].intersect(rayFromShapeToLight);
            if (!lightRayIntersection.intersection.isUndefined() && lightRayIntersection.time < 1) {
                inShadow = true;
                break;
            }
        }

        //- Now we can normalise the vector from the light to the shapeIntersection -//
        directionToLight = directionToLight.normalise();

        //- if there is no shadow, set cBuffColor -//
        if (!inShadow) {
            Vector normal = closestShape.getNormalAt(shapeIntersection.intersection);
            Vector lightRayReflected = directionToLight.reflectOver(normal);
            Vector directionToViewer = (camera.position - shapeIntersection.intersection).normalise();
            Shape::Material material = closestShape.material;

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

            Vector colorVector(r, g, b);

            if (numberOfTimesRecursed < reflectionDepth && material.reflectivity != 0) {
                Vector directionToViewerReflected;
                directionToViewerReflected = ray.direction * (-1);
                directionToViewerReflected = directionToViewerReflected.reflectOver(normal);

                Ray rayReflected = ray;
                rayReflected.position = shapeIntersection.intersection;
                rayReflected.direction = directionToViewerReflected;

                Vector reflectionColor = castRay(rayReflected, ++numberOfTimesRecursed) * material.reflectivity;
                colorVector = colorVector * (1 - material.reflectivity);
                colorVector = colorVector + reflectionColor;
            }

            return colorVector;
        } else {
            Vector colorVector(0, 0, 0);
            return colorVector;
        }
    }
    Vector backgroundVector(0, 0, 0);
    return backgroundVector;
}

void Scene::resizeShapeBuffer(unsigned newSize) {
    shapeBufferSize = newSize;
    Shape *temp = new Shape[shapeBufferSize];

    for (unsigned i = 0; i < numberOfShapes; i++)
        temp[i] = shapeBuffer[i];

    delete[] shapeBuffer;
    shapeBuffer = temp;
}

#endif