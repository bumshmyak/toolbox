#ifndef _TOOLBOX_BASIC_MATRIX_H_
#define _TOOLBOX_BASIC_MATRIX_H_

#include <vector>
#include <iostream>

// simple Matrix class inspred by:
// http://chi3x10.wordpress.com/2008/05/28/calculate-matrix-inversion-in-c/
class Matrix {
 public:
  Matrix();
  Matrix(int row_count, int col_count);
  explicit Matrix(int order);  // square matrix

  int get_row_count() const { return a_.size(); }
  int get_col_count() const { return a_.empty() ? 0 : a_[0].size(); }

  void set(int row, int col, double value) { a_[row][col] = value; }
  double get(int row, int col) const { return a_[row][col]; }

  Matrix transpose() const;

  bool operator==(const Matrix& b) const { return (a_ == b.a_); }

  Matrix operator+(const Matrix& b) const;
  Matrix operator-(const Matrix& b) const;
  Matrix operator*(const Matrix& b) const;
  Matrix operator/(double k) const;

  Matrix operator+=(const Matrix& b) { return (*this = *this + b); }
  Matrix operator-=(const Matrix& b) { return (*this = *this - b); }
  Matrix operator*=(const Matrix& b) { return (*this = *this * b); }
  Matrix operator/=(double k) { return (*this = *this / k); }

  Matrix get_minor(int row, int col) const;
  double get_determinant() const;
  Matrix get_inverse() const;

  friend std::istream& operator>>(std::istream& in, Matrix& m);
  friend std::ostream& operator<<(std::ostream& out, const Matrix& m);

 private:
  std::vector< std::vector< double > > a_;
};

#endif  // _TOOLBOX_BASIC_MATRIX_H_
