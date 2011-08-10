#include "math/matrix.h"

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

TEST(MatrixTest, BasicOperations) {
  Matrix e(2, Vector(2));
  e[0][0] = 1;
  e[1][1] = 1;

  EXPECT_EQ(e, e * e);
  EXPECT_EQ(e, (e + e) / 2);
  EXPECT_EQ(e, e - e + e - e + e);

  EXPECT_EQ(e, get_inverse(e));
  EXPECT_EQ(e, transpose(e));
  EXPECT_EQ(1, get_determinant(e));

  Matrix a(2, Vector(2));
  a[0][0] = 2;
  a[0][1] = 1;
  a[1][0] = 1;
  a[1][1] = 1;

  Matrix a_inv(2, Vector(2));
  a_inv[0][0] = 1;
  a_inv[0][1] = -1;
  a_inv[1][0] = -1;
  a_inv[1][1] = 2;

  EXPECT_EQ(1, get_determinant(a));
  EXPECT_EQ(a_inv, get_inverse(a));
}

TEST(MatrixTest, SolveLinearSystem) {
  Matrix A(3, Vector(3));
  A[0][0] = 1, A[0][1] = 2, A[0][2] = 0;
  A[1][0] = 3, A[1][1] = 4, A[1][2] = 4;
  A[2][0] = 5, A[2][1] = 6, A[2][2] = 3;

  Vector b(3);
  b[0] = 3, b[1] = 7, b[2] = 8;
  Vector correct_x(3);
  correct_x[0] = -1.4, correct_x[1] = 2.2, correct_x[2] = 0.6;
  
  Vector x;
  EXPECT_TRUE(solve_linear_system_lup(A, b, x));
  EXPECT_TRUE(epsilon_equal(correct_x, x));

  x.clear();
  EXPECT_TRUE(solve_linear_system_gauss(A, b, x));
  EXPECT_TRUE(epsilon_equal(correct_x, x));
}
