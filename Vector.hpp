#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>

class Vector {
public:
    Vector(double a, double b, double c);
    Vector();
    friend Vector operator*(double b, const Vector &a);
    bool operator==(const Vector &b) const;
    bool operator!=(const Vector &b) const;
    Vector operator+(const Vector &b) const;
    Vector operator-(const Vector &b) const;
    double operator*(const Vector &b) const;
    Vector operator*(double b) const;
    double operator[](int i) const;
    void operator()(double a, double b, double c);
    void print() const;
    Vector normalise() const;
    bool isUndefined() const;
    Vector reflectOver(const Vector &vec) const;
    bool operator!() const;
private:
    void throwErrorIfUndefined(const Vector &b) const;
    void throwErrorIfUndefined() const;
    double vectorArray[3];
    bool undefined;
};

Vector::Vector(double a, double b, double c) {
    vectorArray[0] = a;
    vectorArray[1] = b;
    vectorArray[2] = c;
    undefined = false;
}

Vector::Vector() {
    undefined = true;
}

bool Vector::operator==(const Vector &b) const {
    throwErrorIfUndefined(b);
    return ((*this)[0] == b[0] && (*this)[1] == b[1] && (*this)[2] == b[2]);
}

bool Vector::operator!=(const Vector &b) const {
    throwErrorIfUndefined(b);
    return !((*this) == b);
}

Vector Vector::operator+(const Vector &b) const {
    throwErrorIfUndefined(b);
    Vector result(vectorArray[0] + b[0], vectorArray[1] + b[1], vectorArray[2] + b[2]);
    return result;
}

Vector Vector::operator-(const Vector &b) const {
    throwErrorIfUndefined(b);
    Vector result(vectorArray[0] - b[0], vectorArray[1] - b[1], vectorArray[2] - b[2]);
    return result;
}

double Vector::operator*(const Vector &b) const {
    throwErrorIfUndefined(b);
    double result = vectorArray[0]*b[0] + vectorArray[1]*b[1] + vectorArray[2]*b[2];
    return result;
}

Vector Vector::operator*(double b) const {
    throwErrorIfUndefined();
    Vector result(vectorArray[0] * b, vectorArray[1] * b, vectorArray[2] * b);
    return result;
}

Vector operator*(double b, const Vector &a) {
    a.throwErrorIfUndefined();
    return a * b;
}

double Vector::operator[](int i) const {
    throwErrorIfUndefined();
    return vectorArray[i];
}

void Vector::operator()(double a, double b, double c) {
    vectorArray[0] = a;
    vectorArray[1] = b;
    vectorArray[2] = c;
    undefined = false;
}

Vector Vector::normalise() const {
    throwErrorIfUndefined();
    double mag = sqrt((*this) * (*this));
    Vector copy(vectorArray[0] / mag,
                vectorArray[1] / mag,
                vectorArray[2] / mag);
    return copy;
}

bool Vector::isUndefined() const {
    return undefined;
}

bool Vector::operator!() const{
    return isUndefined();
}

void Vector::print() const {
    throwErrorIfUndefined();
    std::cout << "[" << vectorArray[0] << ", " << vectorArray[1] << ", " << vectorArray[2] << "]\n";
}

Vector Vector::reflectOver(const Vector &normal) const {
    throwErrorIfUndefined();
    return 2 * ((*this) * normal) * normal - (*this);
}

void Vector::throwErrorIfUndefined(const Vector &b) const {
    if (!b || undefined) {
        throw std::runtime_error("Vector passed as argument is uninitialized.");
    }
}

void Vector::throwErrorIfUndefined() const {
    if (undefined) {
        throw std::runtime_error("Vector passed as argument is uninitialized.");
    }
}

struct Ray {
    Vector direction;
    Vector position;
};

#endif