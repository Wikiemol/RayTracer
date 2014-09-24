#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>
#include <time.h>

class Vector3 {
public:
    //- Constructors -//
    Vector3(double a, double b, double c);
    Vector3();

    //- Operator overloads -//
    friend Vector3 operator*(double b, const Vector3 &a);
    bool operator==(const Vector3 &b) const;
    bool operator!=(const Vector3 &b) const;
    Vector3 operator+(const Vector3 &b) const;
    Vector3 operator-(const Vector3 &b) const;
    double operator*(const Vector3 &b) const;
    Vector3 operator*(double b) const;
    double operator[](int i) const;
    void operator()(double a, double b, double c);
    bool operator!() const;

    //- Methods -//
    void print() const;
    Vector3 normalise() const;
    bool isUndefined() const;
    Vector3 reflectOver(const Vector3 &vec) const;
    Vector3 cross(const Vector3 &b) const;
    void setAt(int i, double value);

    //- Static Methods -//
    static Vector3 rand();
private:
    void throwErrorIfUndefined(const Vector3 &b) const;
    void throwErrorIfUndefined() const;

    double vectorArray[3];
    bool undefined;
};

struct Ray {
    Vector3 direction;
    Vector3 position;
};

//-Constructors-//
Vector3::Vector3(double a, double b, double c) {
    vectorArray[0] = a;
    vectorArray[1] = b;
    vectorArray[2] = c;
    undefined = false;
}

Vector3::Vector3() {
    undefined = true;
}

//-Operator Overloads-//
bool Vector3::operator==(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    return ((*this)[0] == b[0] && (*this)[1] == b[1] && (*this)[2] == b[2]);
}

bool Vector3::operator!=(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    return !((*this) == b);
}

Vector3 Vector3::operator+(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    Vector3 result(vectorArray[0] + b[0], vectorArray[1] + b[1], vectorArray[2] + b[2]);
    return result;
}

Vector3 Vector3::operator-(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    Vector3 result(vectorArray[0] - b[0], vectorArray[1] - b[1], vectorArray[2] - b[2]);
    return result;
}

double Vector3::operator*(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    double result = vectorArray[0]*b[0] + vectorArray[1]*b[1] + vectorArray[2]*b[2];
    return result;
}

Vector3 Vector3::operator*(double b) const {
    throwErrorIfUndefined();
    Vector3 result(vectorArray[0] * b, vectorArray[1] * b, vectorArray[2] * b);
    return result;
}

Vector3 operator*(double b, const Vector3 &a) {
    a.throwErrorIfUndefined();
    return a * b;
}

double Vector3::operator[](int i) const {
    throwErrorIfUndefined();
    return vectorArray[i];
}

void Vector3::operator()(double a, double b, double c) {
    vectorArray[0] = a;
    vectorArray[1] = b;
    vectorArray[2] = c;
    undefined = false;
}

bool Vector3::operator!() const{
    return isUndefined();
}

//- Public Methods -//
Vector3 Vector3::normalise() const {
    throwErrorIfUndefined();
    double mag = sqrt((*this) * (*this));
    Vector3 copy(vectorArray[0] / mag,
                vectorArray[1] / mag,
                vectorArray[2] / mag);
    return copy;
}

bool Vector3::isUndefined() const {
    return undefined;
}

void Vector3::print() const {
    std::cout << "[" << vectorArray[0] << ", " << vectorArray[1] << ", " << vectorArray[2] << "]\n";
}

Vector3 Vector3::reflectOver(const Vector3 &normal) const {
    throwErrorIfUndefined();
    return 2 * ((*this) * normal) * normal - (*this);
}

Vector3 Vector3::cross(const Vector3 &b) const {
    throwErrorIfUndefined(b);
    Vector3 result(vectorArray[1] * b[2] - b[1] * vectorArray[2],
                  (vectorArray[2] * b[0] - vectorArray[0] * b[2]),
                  vectorArray[0] * b[1] - vectorArray[1] * b[0]);
    return result;
}

void Vector3::setAt(int i, double value) {
    vectorArray[i] = value;
}

//- Static Methods -//
Vector3 Vector3::rand() {
    Vector3 randomVector((double) std::rand() / (double) RAND_MAX,
                        (double) std::rand() / (double) RAND_MAX,
                        (double) std::rand() / (double) RAND_MAX);
    return randomVector;
}

//- Private Methods -//
void Vector3::throwErrorIfUndefined(const Vector3 &b) const {
    if (!b) {
        throw std::runtime_error("Vector3 passed as argument is uninitialized.");
    }

    if (undefined) {
        throw std::runtime_error("Method or operator called on uninitialized Vector3.");
    }
}

void Vector3::throwErrorIfUndefined() const {
    if (undefined) {
        throw std::runtime_error("Method or operator called on uninitialized Vector3.");
    }
}

#endif
