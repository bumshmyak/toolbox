#include "math/simplex.h"

#include <iostream>
#include <vector>

#include "gtest/gtest.h"

using std::vector;
using std::cout;
using std::cerr;
using std::endl;

TEST(SimplexTest, ConvertToStandartForm) {
  Vector c(2);
  c[0] = -2, c[1] = 3;

  Matrix A(3, Vector(2));
  vector<ConstraintType> t(3);
  Vector b(3);
  A[0][0] = 1, A[0][1] = 1, t[0] = EQUAL_CONSTRAINT, b[0] = 7;
  A[1][0] = 1, A[1][1] = -2, t[1] = LOWER_EQUAL_CONSTRAINT, b[1] = 4;
  A[2][0] = 1, A[2][1] = 0, t[2] = GREATER_EQUAL_CONSTRAINT, b[2] = 0;

  bool is_max = false;

  Matrix As;
  Vector bs, cs;

  convert_lp_to_standart_form(is_max, A, t, b, c, As, bs, cs);

  Vector cc(4);
  cc[0] = 2, cc[1] = -3, cc[2] = -2, cc[3] = 3;

  Matrix Ac(4, Vector(4));
  Ac[0][0] = 1, Ac[0][1] = 1, Ac[0][2] = -1, Ac[0][3] = -1;
  Ac[1][0] = 1, Ac[1][1] = -2, Ac[1][2] = -1, Ac[1][3] = 2;
  Ac[2][0] = -1, Ac[2][1] = 0, Ac[2][2] = 1, Ac[2][3] = 0;
  Ac[3][0] = -1, Ac[3][1] = -1, Ac[3][2] = 1, Ac[3][3] = 1;

  Vector bc(4);
  bc[0] = 7, bc[1] = 4, bc[2] = 0, bc[3] = -7;

  ASSERT_EQ(cc, cs);
  ASSERT_EQ(Ac, As);
  ASSERT_EQ(bc, bs);
}

TEST(SimplexTest, FirstSimplex) {
  Vector c(2);
  c[0] = 2, c[1] = -1;

  Matrix A(2, Vector(2));
  Vector b(2);
  A[0][0] = 2, A[0][1] = -1, b[0] = 2;
  A[1][0] = 1, A[1][1] = -5, b[1] = -4;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(2, x.size());
  EXPECT_TRUE(epsilon_equal(14.0 / 9, x[0]));
  EXPECT_TRUE(epsilon_equal(10.0 / 9, x[1]));
  EXPECT_TRUE(epsilon_equal(2.0, v));
}

TEST(SimplexTest, SecondSimplex) {
  Vector c(3);
  c[0] = 3, c[1] = 1, c[2] = 2;

  Matrix A(3, Vector(3));
  Vector b(3);
  A[0][0] = 1, A[0][1] = 1, A[0][2] = 3, b[0] = 30;
  A[1][0] = 2, A[1][1] = 2, A[1][2] = 5, b[1] = 24;
  A[2][0] = 4, A[2][1] = 1, A[2][2] = 2, b[2] = 36;
  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_TRUE(epsilon_equal(8, x[0]));
  EXPECT_TRUE(epsilon_equal(4, x[1]));
  EXPECT_TRUE(epsilon_equal(0, x[2]));
  EXPECT_TRUE(epsilon_equal(28, v));
}

// same example as in ConvertToStandartForm
// but manualy converted to standart form
// (less variables)
TEST(SimplexTest, ThirdSimplex) {
  Vector c(3);
  c[0] = 2, c[1] = -3, c[2] = 3;

  Matrix A(3, Vector(3));
  Vector b(3);
  A[0][0] = 1, A[0][1] = 1, A[0][2] = -1, b[0] = 7;
  A[1][0] = -1, A[1][1] = -1, A[1][2] = 1, b[1] = -7;
  A[2][0] = 1, A[2][1] = -2, A[2][2] = 2, b[2] = 4;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(3, x.size());
  EXPECT_TRUE(epsilon_equal(6, x[0]));
  EXPECT_TRUE(epsilon_equal(1, x[1]));
  EXPECT_TRUE(epsilon_equal(0, x[2]));
  EXPECT_TRUE(epsilon_equal(9, v));
}

TEST(SimplexTest, FourthSimplex) {
  Vector c(2);
  c[0] = 18, c[1] = 12.5;

  Matrix A(3, Vector(2));
  Vector b(3);
  A[0][0] = 1, A[0][1] = 1, b[0] = 20;
  A[1][0] = 1, A[1][1] = 0, b[1] = 12;
  A[2][0] = 0, A[2][1] = 1, b[2] = 16;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);

  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(2, x.size());
  EXPECT_TRUE(epsilon_equal(12, x[0]));
  EXPECT_TRUE(epsilon_equal(8, x[1]));
  EXPECT_TRUE(epsilon_equal(316, v));
}

TEST(SimplexTest, FifthSimplex) {
  Vector c(2);
  c[0] = -5, c[1] = -3;

  Matrix A(2, Vector(2));
  Vector b(2);
  A[0][0] = 1, A[0][1] = -1, b[0] = 1;
  A[1][0] = 2, A[1][1] = 1, b[1] = 2;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(2, x.size());
  EXPECT_TRUE(epsilon_equal(0, x[0]));
  EXPECT_TRUE(epsilon_equal(0, x[1]));
  EXPECT_TRUE(epsilon_equal(0, v));
}

// unlimted task example
TEST(SimplexTest, SixsSimplex) {
  Vector c(3);
  c[0] = 1, c[1] = 1, c[2] = 1;

  Matrix A(2, Vector(3));
  Vector b(2);
  A[0][0] = -2, A[0][1] = -7.5, A[0][2] = -3, b[0] = -10000;
  A[1][0] = -20, A[1][1] = -5, A[1][2] = -10, b[1] = -30000;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);

  EXPECT_TRUE(is_solvable);
  EXPECT_FALSE(is_limited);
}

TEST(SimplexTest, FirstSolveLP) {
  Vector c(2);
  c[0] = 1, c[1] = 1;

  Matrix A(3, Vector(2));
  vector<ConstraintType> t(3);
  Vector b(3);

  A[0][0] = 4, A[0][1] = -1, t[0] = LOWER_EQUAL_CONSTRAINT, b[0] = 8;
  A[1][0] = 2, A[1][1] = 1, t[1] = LOWER_EQUAL_CONSTRAINT, b[1] = 10;
  A[2][0] = 5, A[2][1] = -2, t[2] = GREATER_EQUAL_CONSTRAINT, b[2] = -2;
  bool is_max = true;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(2, x.size());
  EXPECT_TRUE(epsilon_equal(2, x[0]));
  EXPECT_TRUE(epsilon_equal(6, x[1]));
  EXPECT_TRUE(epsilon_equal(8, v));
}

// same example as in ConvertToStandartForm
TEST(SimplexTest, SecondSolveLP) {
  Vector c(2);
  c[0] = -2, c[1] = 3;

  Matrix A(3, Vector(2));
  vector<ConstraintType> t(3);
  Vector b(3);
  A[0][0] = 1, A[0][1] = 1, t[0] = EQUAL_CONSTRAINT, b[0] = 7;
  A[1][0] = 1, A[1][1] = -2, t[1] = LOWER_EQUAL_CONSTRAINT, b[1] = 4;
  A[2][0] = 1, A[2][1] = 0, t[2] = GREATER_EQUAL_CONSTRAINT, b[2] = 0;

  bool is_max = false;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(2, x.size());
  EXPECT_TRUE(epsilon_equal(6, x[0]));
  EXPECT_TRUE(epsilon_equal(1, x[1]));
  EXPECT_TRUE(epsilon_equal(-9, v));
}

// example of unsolvable task
TEST(SimplexTest, ThirdSolveLP) {
  Vector c(2);
  c[0] = 2, c[1] = 7;

  Matrix A(4, Vector(2));
  vector<ConstraintType> t(4);
  Vector b(4);
  A[0][0] = 1, A[0][1] = 0, t[0] = EQUAL_CONSTRAINT, b[0] = 7;
  A[1][0] = 3, A[1][1] = 2, t[1] = GREATER_EQUAL_CONSTRAINT, b[1] = 24;
  A[2][0] = 1, A[2][1] = 0, t[2] = GREATER_EQUAL_CONSTRAINT, b[2] = 0;
  A[2][0] = 0, A[3][1] = 1, t[3] = LOWER_EQUAL_CONSTRAINT, b[3] = 0;  

  bool is_max = false;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_FALSE(is_solvable);
}

TEST(SimplexTest, FourthSolveLP) {
  Vector c(3);
  c[0] = 2, c[1] = 0, c[2] = -6;

  Matrix A(6, Vector(3, 0));
  vector<ConstraintType> t(6, GREATER_EQUAL_CONSTRAINT);
  Vector b(6, 0);
  
  A[0][0] = 1, A[0][1] = 1, A[0][2] = -1,
      t[0] = LOWER_EQUAL_CONSTRAINT, b[0] = 7;
  A[1][0] = 3, A[1][1] = -1, A[1][2] = 0, b[1] = 8;
  A[2][0] = -1, A[2][1] = 2, A[2][2] = 2, b[2] = 0;
  A[3][0] = 1;
  A[4][1] = 1;
  A[5][2] = 1;

  bool is_max = true;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_TRUE(is_limited);
  EXPECT_EQ(3, x.size());
  EXPECT_TRUE(epsilon_equal(14.0 / 3, x[0]));
  EXPECT_TRUE(epsilon_equal(7.0 / 3, x[1]));
  EXPECT_TRUE(epsilon_equal(0, x[2]));
  EXPECT_TRUE(epsilon_equal(28.0 / 3, v));
#if 0
  cout << is_solvable << endl
       << is_limited << endl
       << x << endl
       << v << endl;
#endif
}

// another unsolvable example
TEST(SimplexTest, FifthSolveLP) {
  Vector c(2);
  c[0] = 3, c[1] = -2;

  Matrix A(4, Vector(2, 0));
  vector<ConstraintType> t(4, GREATER_EQUAL_CONSTRAINT);
  Vector b(4, 0);
  A[0][0] = 1, A[0][1] = 1, t[0] = LOWER_EQUAL_CONSTRAINT, b[0] = 2;
  A[1][0] = -2, A[1][1] = -2, t[1] = LOWER_EQUAL_CONSTRAINT, b[1] = -10;
  A[2][0] = 1;
  A[3][1] = 1;

  bool is_max = true;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_FALSE(is_solvable);
}

// example of unlimited
TEST(SimplexTest, SixsSolveLP) {
  Vector c(2);
  c[0] = 1, c[1] = -1;

  Matrix A(4, Vector(2, 0));
  vector<ConstraintType> t(4, GREATER_EQUAL_CONSTRAINT);
  Vector b(4, 0);
  A[0][0] = -2, A[0][1] = 1, t[0] = LOWER_EQUAL_CONSTRAINT, b[0] = -1;
  A[1][0] = -1, A[1][1] = -2, t[1] = LOWER_EQUAL_CONSTRAINT, b[1] = -2;
  A[2][0] = 1;
  A[3][1] = 1;

  bool is_max = true;

  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);
  EXPECT_TRUE(is_solvable);
  EXPECT_FALSE(is_limited);
}

TEST(SimplexTest, PoliticalProblemSimple) {
  Vector c(4);
  c[0] = -1, c[1] = -1, c[2] = -1, c[3] = -1;

  Matrix A(3, Vector(4, 0));
  Vector b(3, 0);

  A[0][0] = 2, A[0][1] = -8, A[0][2] = 0, A[0][3] = -10, b[0] = -50;
  A[1][0] = -5, A[1][1] = -2, A[1][2] = 0, A[1][3] = 0, b[1] = -100;
  A[2][0] = -3, A[2][1] = 5, A[2][2] = -10, A[2][3] = 2, b[2] = -25;
  bool is_solvable, is_limited;
  Vector x;
  double v;
  simplex(A, b, c, is_solvable, is_limited, x, v);

#if 0
  cout << is_solvable << endl
       << is_limited << endl
       << x << endl
       << v << endl;
#endif
}


TEST(SimplexTest, PoliticalProblem) {
  Vector c(4);
  c[0] = 1, c[1] = 1, c[2] = 1, c[3] = 1;

  Matrix A(7, Vector(4, 0));
  vector<ConstraintType> t(7, GREATER_EQUAL_CONSTRAINT);
  Vector b(7, 0);

  A[0][0] = -2, A[0][1] = 8, A[0][2] = 0, A[0][3] = 10, b[0] = 50;
  A[1][0] = 5, A[1][1] = 2, A[1][2] = 0, A[1][3] = 0, b[1] = 100;
  A[2][0] = 3, A[2][1] = -5, A[2][2] = 10, A[2][3] = -2, b[2] = 25;
  A[3][0] = 1;
  A[4][1] = 1;
  A[5][2] = 1;
  A[6][3] = 1;

  bool is_max = false;
  
  bool is_solvable, is_limited;
  Vector x;
  double v;
  solve_lp(is_max, A, t, b, c, is_solvable, is_limited, x, v);

#if 0
  cout << is_solvable << endl
       << is_limited << endl
       << x << endl
       << v << endl;
#endif
}

