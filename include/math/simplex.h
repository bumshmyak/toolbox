#ifndef _TOOLBOX_MATH_SIMPLEX_H_
#define _TOOLBOX_MATH_SIMPLEX_H_

#include <vector>
#include "math/matrix.h"


enum ConstraintType {
  EQUAL_CONSTRAINT = 0,
  LOWER_EQUAL_CONSTRAINT = 1,
  GREATER_EQUAL_CONSTRAINT = 2
};

void convert_lp_to_standart_form(
    bool is_max,
    const Matrix& constraints_left_part,
    const std::vector<ConstraintType>& constraint_types,
    const Vector& constraints_right_part,
    const Vector& target_function,
    Matrix& standart_constraints_left_part,
    Vector& standart_constraints_right_part,
    Vector& standart_target_function);

// solves lp in standart form
void simplex(
    const Matrix& standart_constraints_left_part,
    const Vector& standart_constraints_right_part,
    const Vector& standart_target_function,
    bool& is_solvable,
    bool& is_limited,
    Vector& optimal_solution,
    double& optimal_solution_value);

// solves arbitrary lp
void solve_lp(
    bool is_max,
    const Matrix& constraints_left_part,
    const std::vector<ConstraintType>& constraint_types,
    const Vector& constraints_right_part,
    const Vector& target_function,
    bool& is_solvable,
    bool& is_limited,
    Vector& optimal_solution,
    double& optimal_solution_value);



#endif  // _TOOLBOX_MATH_SIMPLEX_H_
