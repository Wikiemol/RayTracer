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

    virtual ~Shape(){};
    Material material;
    virtual Shape::Intersection intersect(const Ray &ray) const = 0;
    virtual Vector getNormalAt(const Vector &point) const = 0;
};

class Sphere: public Shape {
public:
    Vector position;
    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(const Vector &point) const;
    double radius;
};

class Plane: public Shape {
public:
    Vector position;
    Vector normal;
    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(const Vector &point) const;
};

class Triangle: public Shape {

public:
    Triangle(const Vector &vert1, const Vector &vert2, const Vector &vert3) {
        init(vert1, vert2, vert3);
    }
    
    Triangle(double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3) {
        Vector vert1(x1, y1, z1);
        Vector vert2(x2, y2, z2);
        Vector vert3(x3, y3, z3);

        init(vert1, vert2, vert3);
    }

    Shape::Intersection intersect(const Ray &ray) const;
    Vector getNormalAt(const Vector &point) const;

private:
    void init(const Vector &vert1, const Vector &vert2, const Vector &vert3) {
        vertex1 = vert1;
        vertex2 = vert2;
        vertex3 = vert3;

        position = (1 / 3.0) * (vertex1 + vertex2 + vertex3);

        Vector side1 = vertex1 - vertex2;
        Vector side2 = vertex3 - vertex2;
        normal = side1.cross(side2);
    };
    Vector vertex1;
    Vector vertex2;
    Vector vertex3;
    Vector normal;
    Vector position;
};

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
        //-- Checking if the ray intersects the plane that the triangle is on
        //   using the equation for a plane--//
        double solution = -(normal * (ray.position - position)) / denominator;
        if (solution > 1e-10) {
            Vector intersection = solution * ray.direction + ray.position;
            intersect.intersection = intersection;
            intersect.time = solution;
        }
    }
    return intersect;
}

Shape::Intersection Triangle::intersect(const Ray &ray) const {
    Shape::Intersection intersect;
    double denominator = (normal * ray.direction);

    if (denominator != 0) {
        //-- Checking if the ray intersects the plane that the triangle is on
        //   using the equation for a plane--//
        double solution = -(normal * (ray.position - vertex1)) / denominator;
        if (solution > 1e-10) {
            Vector intersection = solution * ray.direction + ray.position;

            double triangleArea = sqrt(normal * normal) / 2;

            //-- Checking to see if the intersection is inside the triangle using area method --//
            Vector triSegment1Normal = (intersection - vertex1).cross(vertex2 - vertex1);
            double areaOfTriSegment1 = sqrt(triSegment1Normal * triSegment1Normal) / 2;

            Vector triSegment2Normal = (intersection - vertex2).cross(vertex3 - vertex2);
            double areaOfTriSegment2 = sqrt(triSegment2Normal * triSegment2Normal) / 2;

            Vector triSegment3Normal = (intersection - vertex3).cross(vertex1 - vertex3);
            double areaOfTriSegment3 = sqrt(triSegment3Normal * triSegment3Normal) / 2;

            double totalArea = areaOfTriSegment1 + areaOfTriSegment2 + areaOfTriSegment3;
            if (totalArea <= triangleArea + 1e-10 && totalArea >= triangleArea - 1e-10) {
                intersect.intersection = intersection;
                intersect.time = solution;
            }
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

Vector Triangle::getNormalAt(const Vector &point) const {
    return normal.normalise();
}
#endif