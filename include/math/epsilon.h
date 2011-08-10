#ifndef TOOLBOX_MATH_EPSILON_H_
#define TOOLBOX_MATH_EPSILON_H_

#define EPS 1e-9

bool epsilon_equal(double a, double b, double eps = EPS);
bool epsilon_lower(double a, double b, double eps = EPS);
bool epsilon_lower_or_equal(double a, double b, double eps = EPS);
bool epsilon_greater(double a, double b, double eps = EPS);
bool epsilon_greater_or_equal(double a, double b, double eps = EPS);

#endif  // TOOLBOX_MATH_EPSILON_H_
