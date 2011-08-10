#include "math/matrix.h"

#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>

using std::vector;
using std::swap;

bool epsilon_equal(const Vector& first, const Vector& second, double eps) {
  if (first.size() != second.size()) {
    return false;
  }
  for (int i = 0; i < first.size(); ++i) {
    if (!epsilon_equal(first[i], second[i])) {
      return false;
    }
  }
  return true;
}

bool epsilon_equal(const Matrix& first, const Matrix& second, double eps) {
  if (first.size() != second.size()) {
    return false;
  }
  for (int i = 0; i < first.size(); ++i) {
    if (!epsilon_equal(first[i], second[i])) {
      return false;
    }
  }
  return true;
}

std::istream& operator>>(std::istream& in, Vector& a) {
  int elements_count;
  in >> elements_count;
  a.resize(elements_count);
  for (int i = 0; i < elements_count; ++i) {
    in >> a[i];
  }
  return in;
}
 
std::istream& operator>>(std::istream& in, Matrix& A) {
  int row_count, col_count;
  in >> row_count >> col_count;
  A = vector< vector<double> >(row_count, vector< double >(col_count));
  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      in >> A[i][j];
    }
  }
  return in;
}

std::ostream& operator<<(std::ostream& out, const Vector& a) {
  for (int i = 0; i < a.size(); ++i) {
    out << a[i] << ' ';
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const Matrix& A) {
  for (int i = 0; i < A.size(); ++i) {
    out << A[i] << std::endl;
  }
  return out;
}

Matrix transpose(const Matrix& A) {
  int row_count = A.size();
  int col_count = A[0].size();

  Matrix res(col_count, Vector(row_count));

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res[j][i] = A[i][j];
    }
  }

  return res;
}


Matrix operator+(const Matrix& first, const Matrix& second) {
  int row_count = first.size();
  int col_count = first[0].size();

  assert(row_count == second.size());
  assert(col_count == second[0].size());

  Matrix res(row_count, Vector(col_count));

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res[i][j] = first[i][j] + second[i][j];
    }
  }

  return res;
}

Matrix operator-(const Matrix& first, const Matrix& second) {
  int row_count = first.size();
  int col_count = first[0].size();

  assert(row_count == second.size());
  assert(col_count == second[0].size());

  Matrix res(row_count, Vector(col_count));

  for (int i = 0; i < row_count; ++i) {
    for (int j = 0; j < col_count; ++j) {
      res[i][j] = first[i][j] - second[i][j];
    }
  }

  return res;
}


Matrix operator*(const Matrix& first, const Matrix& second) {
  assert(first[0].size() == second.size());

  Matrix res(first.size(), Vector(second[0].size()));

  for (int i = 0; i < first.size(); ++i) {
    for (int j = 0; j < second[0].size(); ++j) {
      for (int k = 0; k < second.size(); ++k) {
        res[i][j] += first[i][k] * second[k][j];
      }
    }
  }

  return res;
}


Matrix operator*(const Matrix& A, double value) {
  Matrix res = A;
  for (int i = 0; i < A.size(); ++i) {
    for (int j = 0; j < A[i].size(); ++j) {
      res[i][j] *= value;
    }
  }

  return res;
}

Matrix operator/(const Matrix& A, double value) {
  return A * (1.0 / value);
}

Matrix& operator+=(Matrix& me, const Matrix& another) {
  me = me + another;
  return me;
}

Matrix& operator-=(Matrix& me, const Matrix& another) {
  me = me - another;
  return me;
}

Matrix& operator*=(Matrix& me, const Matrix& another) {
  me = me * another;
  return me;
}

Matrix& operator*=(Matrix& me, double value) {
  me = me * value;
  return me;
}

Matrix& operator/=(Matrix& me, double value) {
  me = me / value;
  return me;
}


Matrix get_minor(const Matrix& A, int row, int col) {
  int order = A.size();
  assert(order == A[0].size());
  assert(order > 1);

  Matrix res(order - 1, Vector(order - 1));

  int p = 0;
  int q = 0;
  for (int i = 0; i < order; ++i) {
    if (i != row) {
      for (int j = 0; j < order; ++j) {
        if (j != col) {
          res[p][q] = A[i][j];
          ++q;
        }
      }
      ++p;
      q = 0;
    }
  }

  return res;
}

double get_determinant(const Matrix& A) {
  int order = A.size();
  assert(order == A[0].size());

  if (order == 1) {
    return A[0][0];
  }
  
  double res = 0;
  for (int i = 0; i < order; ++i) {
    res += (i % 2 ? -1 : 1) * A[0][i] *
        get_determinant(get_minor(A, 0, i));
  }

  return res;
}

Matrix get_inverse(const Matrix& A) {
  int order = A.size();
  assert(order == A[0].size());

  Matrix res(order, Vector(order)); 
  for (int i = 0; i < order; ++i) {
    for (int j = 0; j < order; ++j) {
      res[i][j] = ((i + j) % 2 ? -1 : 1) *
          get_determinant(get_minor(A, i, j));
    }
  }

  return res;
}

bool solve_linear_system_lup(const Matrix& A_, const Vector& b, Vector& x) {
  Matrix A = A_;
  int n = A.size();
  assert(A[0].size() == n);
  assert(b.size() == n);

  vector<int> p(n);
  for (int i = 0; i < n; ++i) {
    p[i] = i;
  }

  for (int k = 0; k < n; ++k) {
    int max_row = -1;
    int max_row_value = 0;
    for (int i = k; i < n; ++i) {
      if (fabs(A[i][k]) > max_row_value) {
        max_row = i;
        max_row_value = fabs(A[i][k]);
      }
    }

    if (max_row == -1) {
      return false;
    }
    
    swap(p[k], p[max_row]);
    for (int j = 0; j < n; ++j) {
      swap(A[k][j], A[max_row][j]);
    }

    for (int i = k + 1; i < n; ++i) {
      A[i][k] /= A[k][k];
      for (int j = k + 1; j < n; ++j) {
        A[i][j] -= A[i][k] * A[k][j];
      }
    }
  }

  Vector y(n);
  for (int i = 0; i < n; ++i) {
    y[i] = b[p[i]];
    for (int j = 0; j < i; ++j) {
      y[i] -= A[i][j] * y[j];
    }
  }

  x.resize(n);
  for (int i = n - 1; i >= 0; --i) {
    x[i] = y[i];
    for (int j = i + 1; j < n; ++j) {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }

  return true;
}

bool solve_linear_system_gauss(const Matrix& A_, const Vector& b_, Vector& x) {
  Matrix A = A_;
  Vector b = b_;
  int n = A.size();
  assert(A[0].size() == n);
  assert(b.size() == n);

  for (int k = 0; k < n; ++k) {
    int max_row = -1;
    int max_row_value = 0;
    for (int i = k; i < n; ++i) {
      if (fabs(A[i][k]) > max_row_value) {
        max_row = i;
        max_row_value = fabs(A[i][k]);
      }
    }

    if (max_row == -1) {
      return false;
    }

    swap(b[k], b[max_row]);
    for (int j = 0; j < n; ++j) {
      swap(A[k][j], A[max_row][j]);
    }

    for (int i = k + 1; i < n; ++i) {
      A[i][k] /= A[k][k];
      for (int j = k + 1; j < n; ++j) {
        A[i][j] -= A[i][k] * A[k][j];
      }
      b[i] -= A[i][k] * b[k];
    }
  }

  x.resize(n);
  for (int i = n - 1; i >= 0; --i) {
    x[i] = b[i];
    for (int j = i + 1; j < n; ++j) {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }

  return true;
}
