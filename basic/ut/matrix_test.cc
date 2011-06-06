#include "basic/matrix.h"

#include <cstdlib>
#include <iostream>

#include "gtest/gtest.h"

TEST(MatrixTest, BasicOperations) {
  Matrix e(2, 2);
  e.set(0, 0, 1);
  e.set(1, 1, 1);

  EXPECT_EQ(2, e.get_row_count());
  EXPECT_EQ(2, e.get_col_count());

  EXPECT_EQ(e, e * e);
  EXPECT_EQ(e, (e + e) / 2);
  EXPECT_EQ(e, e - e + e - e + e);
  EXPECT_EQ(e, e.get_inverse());
  EXPECT_EQ(e, e.transpose());
  EXPECT_EQ(1, e.get_determinant());

  Matrix a(2, 2);
  a.set(0, 0, 2);
  a.set(0, 1, 1);
  a.set(1, 0, 1);
  a.set(1, 1, 1);

  Matrix a_inv(2, 2);
  a_inv.set(0, 0, 1);
  a_inv.set(0, 1, -1);
  a_inv.set(1, 0, -1);
  a_inv.set(1, 1, 2);

  EXPECT_EQ(1, a.get_determinant());
  EXPECT_EQ(a_inv, a.get_inverse());
}
