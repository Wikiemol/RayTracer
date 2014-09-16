#include "Vector4.hpp"
#include "math.h"
class Matrix {
public:
    Matrix(double, double, double, double,
           double, double, double, double,
           double, double, double, double,
           double, double, double, double);

    static Matrix createTransformationMatrix(double tx, double ty, double tz, 
                                             double rx, double ry, double rz) {
        Matrix rotationZ( cos(rz), sin(rz), 0, tx,
                         -sin(rz), cos(rz), 0, ty,
                          0,       0,       1, tz,
                          0,       0,       0, 1);

        Matrix rotationX( 1,  0,       0,       0,
                          0,  cos(rx), sin(rx), 0,
                          0, -sin(rx), cos(rx), 0,
                          0,  0,       0,       1);

        Matrix rotationY( cos(ry), 0, sin(ry), 0,
                          0,       1, 0,       0,
                         -sin(ry), 0, cos(ry), 0,
                          0,       0, 0,       1);
        return rotationZ * rotationY * rotationX;
    }
    Matrix operator *(const Matrix &m)const;
    Vector4 operator *(const Vector4 &v)const;
    void print() const;
    void setAt(unsigned int i, unsigned int j, double value);
    Vector4 getRowVector(unsigned int row) const;
    Vector4 getColumnVector(unsigned int row) const;
private:
    double matrixArray[4][4];
};
Matrix::Matrix(double x1, double y1, double z1, double w1,
               double x2, double y2, double z2, double w2,
               double x3, double y3, double z3, double w3,
               double x4, double y4, double z4, double w4) {

    matrixArray[0][0] = x1;
    matrixArray[0][1] = y1;
    matrixArray[0][2] = z1;
    matrixArray[0][3] = w1;

    matrixArray[1][0] = x2;
    matrixArray[1][1] = y2;
    matrixArray[1][2] = z2;
    matrixArray[1][3] = w2;

    matrixArray[2][0] = x3;
    matrixArray[2][1] = y3;
    matrixArray[2][2] = z3;
    matrixArray[2][3] = w3;

    matrixArray[3][0] = x4;
    matrixArray[3][1] = y4;
    matrixArray[3][2] = z4;
    matrixArray[3][3] = w4;
}


void Matrix::print() const {
    for (int i = 0; i < 4; i++) {
        cout << "|";
        for (int j = 0; j < 4; j++) {
            cout << matrixArray[i][j] << " ";
        }
        cout << "|\n";
    }
}

void Matrix::setAt(unsigned int i, unsigned int j, double value) {
    matrixArray[j][i] = value;
}

Vector4 Matrix::getRowVector(unsigned int row) const {
    Vector4 result(matrixArray[row][0], matrixArray[row][1], matrixArray[row][2], matrixArray[row][3]); 
    return result;
}
Vector4 Matrix::getColumnVector(unsigned int column) const {
    Vector4 result(matrixArray[0][column], matrixArray[1][column], matrixArray[2][column], matrixArray[3][column]);
    return result;
}

Matrix Matrix::operator *(const Matrix &m) const {

    Matrix result(0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0,
                  0, 0, 0, 0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.setAt(i, j, this->getRowVector(j) * m.getColumnVector(i));
        }
    }

    return result;
}

Vector4 Matrix::operator *(const Vector4 &v)const {
    return v;
}