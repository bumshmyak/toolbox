#ifndef _TOOLBOX_MATH_MATRIX_H_
#define _TOOLBOX_MATH_MATRIX_H_

#include <vector>
#include <iostream>
#include "math/epsilon.h"

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

bool epsilon_equal(const Vector& first,
                   const Vector& second,
                   double eps = EPS);

bool epsilon_equal(const Matrix& first,
                   const Matrix& second,
                   double eps = EPS);

Matrix transpose(const Matrix& A);
Matrix operator+(const Matrix& first, const Matrix& second);
Matrix operator-(const Matrix& first, const Matrix& second);
Matrix operator*(const Matrix& first, const Matrix& second);
Matrix operator*(const Matrix& A, double value);
Matrix operator/(const Matrix& A, double value);

Matrix& operator+=(Matrix& me, const Matrix& another);
Matrix& operator-=(Matrix& me, const Matrix& another);
Matrix& operator*=(Matrix& me, const Matrix& another);
Matrix& operator*=(Matrix& me, double value);
Matrix& operator/=(Matrix& me, double value);

Matrix get_minor(const Matrix& A, int row, int col);
double get_determinant(const Matrix& A);
Matrix get_inverse(const Matrix& A);

std::istream& operator>>(std::istream& in, Vector& a);
std::ostream& operator<<(std::ostream& out, const Vector& a);

std::istream& operator>>(std::istream& in, Matrix& A);
std::ostream& operator<<(std::ostream& out, const Matrix& A);

bool solve_linear_system_lup(const Matrix& A,
                             const Vector& b,
                             Vector& x);

bool solve_linear_system_gauss(const Matrix& A,
                               const Vector& b,
                               Vector& x);



#endif  // _TOOLBOX_MATH_MATRIX_H_
