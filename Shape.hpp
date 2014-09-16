#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Vector3.hpp"
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
        Vector3 intersection;
        double time;
    };

    virtual ~Shape(){};
    Material material;
    virtual Shape::Intersection intersect(const Ray &ray) const = 0;
    virtual Vector3 getNormalAt(const Vector3 &point) const = 0;
};

class Sphere: public Shape {
public:
    Vector3 position;
    Shape::Intersection intersect(const Ray &ray) const;
    Vector3 getNormalAt(const Vector3 &point) const;
    double radius;
};

class Plane: public Shape {
public:
    Vector3 position;
    Vector3 normal;
    Shape::Intersection intersect(const Ray &ray) const;
    Vector3 getNormalAt(const Vector3 &point) const;
};

class Triangle: public Shape {

public:
    Triangle(const Vector3 &vert1, const Vector3 &vert2, const Vector3 &vert3) {
        init(vert1, vert2, vert3);
    }
    
    Triangle(double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3) {
        Vector3 vert1(x1, y1, z1);
        Vector3 vert2(x2, y2, z2);
        Vector3 vert3(x3, y3, z3);

        init(vert1, vert2, vert3);
    }

    Shape::Intersection intersect(const Ray &ray) const;
    Vector3 getNormalAt(const Vector3 &point) const;

private:
    void init(const Vector3 &vert1, const Vector3 &vert2, const Vector3 &vert3) {
        vertex1 = vert1;
        vertex2 = vert2;
        vertex3 = vert3;

        position = (1 / 3.0) * (vertex1 + vertex2 + vertex3);

        Vector3 side1 = vertex1 - vertex2;
        Vector3 side2 = vertex3 - vertex2;
        normal = side1.cross(side2);
    };
    Vector3 vertex1;
    Vector3 vertex2;
    Vector3 vertex3;
    Vector3 normal;
    Vector3 position;
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
            Vector3 intersection = solution * ray.direction + ray.position;
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
            Vector3 intersection = solution * ray.direction + ray.position;
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
            Vector3 intersection = solution * ray.direction + ray.position;

            double triangleArea = sqrt(normal * normal) / 2;

            //-- Checking to see if the intersection is inside the triangle using area method --//
            Vector3 triSegment1Normal = (intersection - vertex1).cross(vertex2 - vertex1);
            double areaOfTriSegment1 = sqrt(triSegment1Normal * triSegment1Normal) / 2;

            Vector3 triSegment2Normal = (intersection - vertex2).cross(vertex3 - vertex2);
            double areaOfTriSegment2 = sqrt(triSegment2Normal * triSegment2Normal) / 2;

            Vector3 triSegment3Normal = (intersection - vertex3).cross(vertex1 - vertex3);
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

Vector3 Sphere::getNormalAt(const Vector3 &point) const {
        Vector3 norm = (point - position).normalise();
        return norm;
}

Vector3 Plane::getNormalAt(const Vector3 &point) const {
        return normal.normalise();
}

Vector3 Triangle::getNormalAt(const Vector3 &point) const {
    return normal.normalise();
}
#endif