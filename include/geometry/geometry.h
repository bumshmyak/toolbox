#ifndef TOOLBOX_GEOMETRY_H_
#define TOOLBOX_GEOMETRY_H_

#include <iostream>
#include <algorithm>
#include <vector>

#define EPS 1e-9

bool epsilon_equal(double a, double b, double eps = EPS);
bool epsilon_lower(double a, double b, double eps = EPS);
bool epsilon_lower_or_equal(double a, double b, double eps = EPS);
bool epsilon_greater(double a, double b, double eps = EPS);
bool epsilon_greater_or_equal(double a, double b, double eps = EPS);

struct Point {
  double x;
  double y;
  
  Point()
      : x(0),
        y(0)
  { }

  Point(double _x, double _y) :
      x(_x),
      y(_y)
  { }

  void init_from_polar(double r, double phi);
  double polar_angle() const;
  double length() const;
  double angle(const Point& another) const;
};

Point operator+(const Point& first, const Point& second);
Point operator-(const Point& first, const Point& second);
double vector_product(const Point& first, const Point& second);
double scalar_product(const Point& first, const Point& second);

std::istream& operator>>(std::istream& in, Point& point);
std::ostream& operator<<(std::ostream& out, const Point& point);

bool operator==(const Point& first, const Point& second) {
  return first.x == second.x && first.y == second.y;
}

struct Segment {
  Point A;
  Point B;
  Segment() { }
  Segment(const Point& _A, const Point& _B)
      : A(_A),
        B(_B)
  { }

  double length() const;
  bool in_box(const Point& C) const;
  bool contains(const Point& C) const;
  double distance(const Point& C) const;
  int direction(const Point& c) const;
  bool intersects(const Segment& another) const;
  
  bool ray_contains(const Point& C) const;
  double ray_distance(const Point& C) const;
  bool ray_intersects(const Segment& another) const;
};

std::istream& operator>>(std::istream& in, Segment& segment);
std::ostream& operator<<(std::ostream& out, const Segment& segment);

struct Line {
  double a, b, c;
  Line() :
      a(0),
      b(0),
      c(0)
  { }
 
  Line(double _a, double _b, double _c)
      : a(_a),
        b(_b),
        c(_c)
  { }
     
  void init_from_two_points(const Point& A, const Point& B);
  void init_from_point_and_normal(const Point& A, const Point& normal);
  void init_from_parallel_line_and_distance(const Line& l, double r);

  bool contains(const Point& A);
  Point get_point() const;
  void get_two_points(Point& first, Point& second) const;
  int direction(const Point& A);

  enum IntersectionStatus {
    INTERSECTS = 0,
    PARALLEL = 1,
    SAME = 2
  };

  void intersect(const Line& another,
                 IntersectionStatus& status,
                 Point& point);

  double distance(const Point& point);
};

struct Triangle {
  Point A, B, C;
  Triangle() { }
  
  Triangle(const Point& _A, const Point& _B, const Point& _C)
      : A(_A),
        B(_B),
        C(_C)
  { }

  // medians intersection point
  Point get_centroid() const;

  // altitudes(heights) intersection point
  Point get_orthocenter() const;

  // angle bisectors intersection point
  Point get_incircle() const;

  // perpendicular bisectors intersection point
  Point get_circumcenter() const;
};

std::istream& operator>>(std::istream& in, Triangle& tringle);

struct Polygon {
  std::vector<Point> points;

  void init(const std::vector<Point>& _points);
  bool is_convex() const;
  bool contains(const Point& point);
  bool convex_contains(const Point& point) const;
  double square() const;
};

std::istream& operator>>(std::istream& in, Polygon& poly);

// Convex hull
struct AngleComparator {
  explicit AngleComparator(const Point& point)
      : origin(point)
  { }

  bool operator()(const Point& first, const Point& second) {
    double vp = vector_product(first - origin, second - origin);
    if (epsilon_greater(vp, 0) ||
        (epsilon_equal(vp, 0) &&
         (first - origin).length() < (second - origin).length())) {
      return true;
    } else {
      return false;
    }
  }

  Point origin;
};

std::vector<Point> get_convex_hull(const std::vector<Point>& input_points);

#endif  // TOOLBOX_GEOMETRY_H_
