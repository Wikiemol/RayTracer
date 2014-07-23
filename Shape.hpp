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

    enum shape_type {SPHERE, PLANE, UNDEFINED};
    Shape();
    Shape(shape_type t);
    shape_type type;
    Vector position;
    Vector normal;
    double radius;
    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(Vector point);
    Material material;
};

Shape::Shape() {
    position(0, 0, 0);
    normal(0, 1, 0);
    type = UNDEFINED;
    radius = 100;
}

Shape::Shape(shape_type t) {
    position(0, 0, 0);
    normal(0, 1, 0);
    type = t;
    radius = 100;
}

Shape::Intersection Shape::intersect(const Ray &ray) const{
    Intersection intersect;
    switch(type) {
        case SPHERE:
        {
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
        }
        break;
        case PLANE:
        {
            double denominator = (normal * ray.direction);
            if (denominator != 0) {
                double solution = -(normal * (ray.position - position)) / denominator;
                if (solution > 1e-10) {
                    Vector intersection = solution * ray.direction + ray.position;
                    intersect.intersection = intersection;
                    intersect.time = solution;
                }
            }
        }
        break;
        case UNDEFINED:
        {
            throw std::runtime_error("Shape type is UNDEFINED. Must specify type.");
        }
        break;
    }

    return intersect;
}

Vector Shape::getNormalAt(Vector point) {
    Vector norm;
    switch (type) {
        case SPHERE:
        {
            norm = (point - position);
        }
        break;
        case PLANE:
        {
            norm = normal;
        }
        break;
        case UNDEFINED:
        {
            throw std::runtime_error("Shape type is UNDEFINED. Must specify type.");
        }
        break;
    }
    norm = norm.normalise();
    return norm;
}
#endif