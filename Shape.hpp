#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Vector.hpp"
#include <math.h>
#include <time.h>

class Shape {
public:
    struct Material {
        Material() {
            specularity = 0;
            diffusion = 1;
            shininess = 0;
            reflectivity = 0;
            red = 128;
            green = 128;
            blue = 128;
        }

        double specularity;
        double diffusion;
        double shininess;
        double reflectivity;
        unsigned red;
        unsigned green;
        unsigned blue;
    };

    struct Intersection {
        Vector intersection;
        double time;
    };

    Shape();
    virtual ~Shape(){};
    Vector position;
    Vector normal;
    double radius;
    Material material;
    virtual Shape::Intersection intersect(const Ray &ray) const = 0;
    virtual Vector getNormalAt(const Vector &point) const = 0;
};

class Sphere: public Shape {
public:
    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(const Vector &point) const;
};

class Plane: public Shape {
public:
    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(const Vector &point) const;
};

Shape::Shape() {
    position(0, 0, 0);
    normal(0, 1, 0);
    radius = 100;
}

Shape::Intersection Sphere::intersect(const Ray &ray) const {
    Shape::Intersection intersect;
    double a = ray.direction * ray.direction;
    double b = (ray.position - position) * ray.direction * 2;
    double c = ray.position * ray.position
               - 2 * position * ray.position 
               + position * position 
               - radius * radius;

    double descriminant = b * b - 4 * a * c;
    if (descriminant >= 0) {
        double solution = (-b - sqrt(descriminant)) / (2 * a);
        if (solution > 1e-10) {
            Vector intersection = solution * ray.direction + ray.position;
            intersect.intersection = intersection;
            intersect.time = solution;
        }
    }
    return intersect;
}

Shape::Intersection Plane::intersect(const Ray &ray) const {
    Shape::Intersection intersect;
    double denominator = (normal * ray.direction);
    if (denominator != 0) {
        double solution = -(normal * (ray.position - position)) / denominator;
        if (solution > 1e-10) {
            Vector intersection = solution * ray.direction + ray.position;
            intersect.intersection = intersection;
            intersect.time = solution;
        }
    }
    return intersect;
}

Vector Sphere::getNormalAt(const Vector &point) const {
        Vector norm = (point - position).normalise();
        return norm;
}

Vector Plane::getNormalAt(const Vector &point) const {
        return normal.normalise();
}
#endif