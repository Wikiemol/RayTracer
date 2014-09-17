#include "Vector3.hpp"
#ifndef VECTOR4_HPP
#define VECTOR4_HPP
class Vector4 {
public:
    Vector4(double a, double b, double c, double d);
    Vector4();
    double operator *(const Vector4 &b) const;
    Vector4 operator +(const Vector4 &b) const;
    Vector4 operator -(const Vector4 &b) const;
    double operator[](int i) const;
    void print() const;
    void setAt(int i, double value);
    bool isUndefined() const;
    static Vector4 vec3ToVec4(Vector3 vector, double last) {
        Vector4 result(vector[0], vector[1], vector[2], last);
        return result;
    }
private:
    double vectorArray[4];
    bool undefined;
    void throwErrorIfUndefined(const Vector4 &b) const;
    void throwErrorIfUndefined() const;
};

Vector4::Vector4(double a, double b, double c, double d) {
    vectorArray[0] = a;
    vectorArray[1] = b;
    vectorArray[2] = c;
    vectorArray[3] = d;
    undefined = false;
}

Vector4::Vector4() {
    undefined = true;
}

Vector4 Vector4::operator+(const Vector4 &b) const {
    throwErrorIfUndefined(b);
    Vector4 result(vectorArray[0] + b[0], vectorArray[1] + b[1], vectorArray[2] + b[2], vectorArray[3] + b[3]);
    return result;
}

Vector4 Vector4::operator-(const Vector4 &b) const {
    throwErrorIfUndefined(b);
    Vector4 result(vectorArray[0] - b[0], vectorArray[1] - b[1], vectorArray[2] - b[2], vectorArray[3] - b[3]);
    return result;
}

double Vector4::operator*(const Vector4 &b) const {
    throwErrorIfUndefined(b);
    double result = vectorArray[0]*b[0] + vectorArray[1]*b[1] + vectorArray[2]*b[2] + vectorArray[3] * b[3];
    return result;
}

double Vector4::operator[](int i) const {
    throwErrorIfUndefined();
    return vectorArray[i];
}

bool Vector4::isUndefined() const {
    return undefined;
}

void Vector4::print() const {
    std::cout << "[" << vectorArray[0] << ", " << vectorArray[1] << ", " << vectorArray[2] << ", " << vectorArray[3] << "]\n";
}

void Vector4::setAt(int i, double value) {
    vectorArray[i] = value;
}

void Vector4::throwErrorIfUndefined(const Vector4 &b) const {
    if (b.isUndefined()) {
        throw std::runtime_error("Vector4 passed as argument is uninitialized.");
    }

    if (undefined) {
        throw std::runtime_error("Method or operator called on uninitialized Vector4.");
    }
}

void Vector4::throwErrorIfUndefined() const {
    if (undefined) {
        throw std::runtime_error("Method or operator called on uninitialized Vector4.");
    }
}
#endif