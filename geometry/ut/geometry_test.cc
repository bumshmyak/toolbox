#include "geometry/geometry.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

#include "gtest/gtest.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;

TEST(PointsTest, PolarAngle) {
  Point A(2, 3);
  ASSERT_TRUE(epsilon_equal(0.98279, A.polar_angle(), 1e-5));
}

TEST(PointsTest, AngleBetweenVectors) {
  Point A(2, 1);
  Point B(3, 5);
  ASSERT_TRUE(epsilon_equal(0.56673, A.angle(B), 1e-5));
}

TEST(PointsTest, FromPolarToCartesian) {
  Point A, B;
  A.init_from_polar(3, M_PI / 2);
  B.init_from_polar(4, M_PI);
  ASSERT_EQ(5, (A - B).length());
}

TEST(SegmentTest, RayContains) {
  Segment first(Point(3, 7), Point(5, 8));
  ASSERT_FALSE(first.ray_contains(Point(1, 6)));

  Segment second(Point(1, 6), Point(5, 8));
  ASSERT_TRUE(second.ray_contains(Point(3, 7)));
}

TEST(SegmentTest, RayDistance) {
  Segment first(Point(1, 1), Point(0, 2));
  ASSERT_EQ(1, first.ray_distance(Point(2, 1)));
}

TEST(SegmentTest, SegmentDistance) {
  Segment first(Point(2, 3), Point(2, 5));
  ASSERT_EQ(2, first.distance(Point(0, 4)));
}

TEST(SegmentTest, Intersection) {
  Segment first(Point(5, 1), Point(2, 6));
  Segment second(Point(1, 1), Point(7, 8));
  ASSERT_TRUE(first.intersects(second));
}

TEST(SegmentTest, ContainsPoint) {
  Segment AB(Point(1, 2), Point(5, 4));
  Point C(3, 3);
  ASSERT_TRUE(AB.contains(C));
}

TEST(SegmentTest, RayIntersection) {
  Segment AB(Point(0, 1), Point(1, 2));
  Segment BC(Point(1, -1), Point(1, 0));
  ASSERT_TRUE(AB.ray_intersects(BC));
}

TEST(SegmentTest, RayNonIntersection) {
  Segment AB(Point(0, 0), Point(1, 0));
  Segment BC(Point(0, 1), Point(1, 2));
  ASSERT_FALSE(AB.ray_intersects(BC));
}

TEST(LinesTest, ContainsPoint) {
  Line line(-2, 1, -1);
  Point point(3, 7);
  ASSERT_TRUE(line.contains(point));
}

TEST(LinesTest, InitFromTwoPoints) {
  Point A(1, 2);
  Point B(3, 1);
  Line line;
  line.init_from_two_points(A, B);
  ASSERT_EQ(-1, line.a);
  ASSERT_EQ(-2, line.b);
  ASSERT_EQ(5, line.c);
}

TEST(LinesTest, InitFromPointAndNormal) {
  Point point(1, 2);
  Point normal(3, 1);
  Line line;
  line.init_from_point_and_normal(point, normal);
  ASSERT_EQ(3, line.a);
  ASSERT_EQ(1, line.b);
  ASSERT_EQ(-5, line.c);
}

TEST(LinesTest, InitFromParallelLineAndDistance) {
  Line line;
  line.init_from_parallel_line_and_distance(
      Line(0, -1, 1),
      1);
  ASSERT_EQ(0, line.a);
  ASSERT_EQ(-1, line.b);
  ASSERT_EQ(2, line.c);
}

TEST(LinesTest, Direction) {
  Line line(2, -1, -1);
  EXPECT_EQ(line.direction(Point(0, 0)), line.direction(Point(2, 4)));
}

TEST(LinesTest, IntersectionParallelLines) {
  Line first, second;
  
  first.init_from_two_points(Point(1, 1), Point(2, 2));
  second.init_from_two_points(Point(1, 10), Point(2, 11));
                           
  Line::IntersectionStatus intersection_status;
  Point intersection_point;
  first.intersect(second, intersection_status, intersection_point);
  ASSERT_EQ(Line::PARALLEL, intersection_status);
}

TEST(LinesTest, Intersection) {
  Line first, second;
  
  first.init_from_two_points(Point(0, 0), Point(1, 1));
  second.init_from_two_points(Point(1, 0), Point(-1, 2));
                           
  Line::IntersectionStatus intersection_status;
  Point intersection_point;
  first.intersect(second, intersection_status, intersection_point);
  ASSERT_EQ(Line::INTERSECTS, intersection_status);
  ASSERT_EQ(Point(0.5, 0.5), intersection_point);
}

TEST(LinesTest, LinePointDistance) {
  Line line(1, 1, -1);
  Point point(1, 1);
  ASSERT_TRUE(epsilon_equal(0.70711, line.distance(point), 1e-5));
}

  
TEST(TringleTest, MedianIntersectionPoint) {
  Tringle tringle(Point(1, 1),
                  Point(2, 4),
                  Point(3, 2));

  Point p = tringle.get_medians_intersection_point();

  ASSERT_TRUE(epsilon_equal(2, p.x, 1e-5));
  ASSERT_TRUE(epsilon_equal(2.33333, p.y, 1e-5));
}
