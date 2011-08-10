#include "math/epsilon.h"
#include <cmath>
#include <iostream>

bool epsilon_equal(double a, double b, double eps) {
  return fabs(a - b) < eps;
}

bool epsilon_lower(double a, double b, double eps) {
  return a + eps < b;
}

bool epsilon_lower_or_equal(double a, double b, double eps) {
  return epsilon_lower(a, b, eps) || epsilon_equal(a, b, eps);
}

bool epsilon_greater(double a, double b, double eps) {
  return !epsilon_lower_or_equal(a, b, eps);
}
      
bool epsilon_greater_or_equal(double a, double b, double eps) {
  return !epsilon_lower(a, b, eps);
}
