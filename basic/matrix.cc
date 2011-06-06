#include "basic/matrix.h"

#include <vector>
#include <iostream>
#include <cassert>

using std::vector;

Matrix::Matrix()
  { }

Matrix::Matrix(int row_count, int col_count)
    : a_(vector< vector< double > >(row_count, vector< double >(col_count)))
  { }

Matrix::Matrix(int order)
    : a_(vector< vector< double > >(order, vector< double >(order)))
  { }

std::istream& operator>>(std::istream& in, Matrix& m) {
  int row_count, col_count;
  in >> row_count >> col_count;
  m.a_ = vector< vector< double > >(row_count, vector< double >(col_count));
  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      in >> m.a_[i][j];
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
  for (int i = 0; i < m.a_.size(); ++i) {
    for (int j = 0; j < m.a_[i].size(); ++j) {
      out << m.a_[i][j] << ' ';
    }
    out << std::endl;
  }
  return out;
}

Matrix Matrix::transpose() const {
  int row_count = get_row_count();
  int col_count = get_col_count();

  Matrix res(col_count, row_count);

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res.set(j, i, a_[i][j]);
    }
  }

  return res;
}

Matrix Matrix::operator+(const Matrix& b) const {
  int row_count = get_row_count();
  int col_count = get_col_count();

  assert(row_count == b.get_row_count());
  assert(col_count == b.get_col_count());

  Matrix res(row_count, col_count);

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res.set(i, j, get(i, j) + b.get(i, j));
    }
  }

  return res;
}

Matrix Matrix::operator-(const Matrix& b) const {
  int row_count = get_row_count();
  int col_count = get_col_count();

  assert(row_count == b.get_row_count());
  assert(col_count == b.get_col_count());

  Matrix res(row_count, col_count);

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res.set(i, j, get(i, j) - b.get(i, j));
    }
  }

  return res;
}

Matrix Matrix::operator*(const Matrix& b) const {
  assert(get_col_count() == b.get_row_count());

  Matrix res(get_row_count(), b.get_col_count());

  for (int i = 0; i < get_row_count(); ++i) {
    for (int j = 0; j < b.get_col_count(); ++j) {
      for (int k = 0; k < get_col_count(); ++k) {
        res.set(i, j, res.get(i, j) + get(i, k) * b.get(k, j));
      }
    }
  }

  return res;
}

Matrix Matrix::operator/(double k) const {
  Matrix res(get_row_count(), get_col_count());
  for (int i = 0; i < get_row_count(); ++i) {
    for (int j = 0; j < get_col_count(); ++j) {
      res.set(i, j, get(i, j) / k);
    }
  }

  return res;
}

Matrix Matrix::get_minor(int row, int col) const {
  int order = get_row_count();
  assert(order == get_col_count());
  assert(order > 1);

  Matrix res(order - 1);

  int p = 0;
  int q = 0;
  for (int i = 0; i < order; ++i) {
    if (i != row) {
      for (int j = 0; j < order; ++j) {
        if (j != col) {
          res.set(p, q, a_[i][j]);
          ++q;
        }
      }
      ++p;
      q = 0;
    }
  }

  return res;
}

double Matrix::get_determinant() const {
  int order = get_row_count();
  assert(order == get_col_count());

  if (order == 1) {
    return a_[0][0];
  }

  double res = 0;
  for (int i = 0; i < order; ++i) {
    res += (i % 2 ? -1 : 1) * a_[0][i] * get_minor(0, i).get_determinant();
  }

  return res;
}

Matrix Matrix::get_inverse() const {
  int order = get_row_count();
  assert(order == get_col_count());

  Matrix res(order);
  for (int i = 0; i < order; ++i) {
    for (int j = 0; j < order; ++j) {
      res.set(i, j, ((i + j) % 2 ? -1 : 1) * get_minor(i, j).get_determinant());
    }
  }

  return res;
}
