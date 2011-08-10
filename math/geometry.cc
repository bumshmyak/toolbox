#include "math/geometry.h"

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using std::min;
using std::max;
using std::vector;

void Point::init_from_polar(double r, double phi) {
  x = r * cos(phi);
  y = r * sin(phi);
}

double Point::polar_angle() const {
  double res = atan2(y, x);
  if (res < 0) {
    res += 2.0 * M_PI;
  }
  return res;
}

double Point::length() const {
  return sqrt(x * x + y * y);
}

bool operator<(const Point& first, const Point& second) {
  if (first.y == second.y) {
    return first.x < second.x;
  } else {
    return first.y < second.y;
  }
}

Point operator+(const Point& first, const Point& second) {
  return Point(first.x + second.x, first.y + second.y);
}

Point operator-(const Point& first, const Point& second) {
  return Point(first.x - second.x, first.y - second.y);
}

double vector_product(const Point& first, const Point& second) {
  return first.x * second.y - first.y * second.x;
}

double scalar_product(const Point& first, const Point& second) {
  return first.x * second.x + first.y * second.y;
}

std::istream& operator>>(std::istream& in, Point& point) {
  in >> point.x >> point.y;
  return in;
}


std::ostream& operator<<(std::ostream& out, const Point& point) {
  out << '(' << point.x << ';' << point.y << ')';
  return out;
}

double Point::angle(const Point& another) const {
  return acos(scalar_product(*this, another) / (length() * another.length()));
}


// Segments
std::istream& operator>>(std::istream& in, Segment& segment) {
  in >> segment.A >> segment.B;
  return in;
}

std::ostream& operator<<(std::ostream& out, const Segment& segment) {
  out << segment.A << " -- " << segment.B;
  return out;
}

bool Segment::in_box(const Point& C) const {
  return min(A.x, B.x) <= C.x && C.x <= max(A.x, B.x) &&
      min(A.y, B.y) <= C.y && C.y <= max(A.y, B.y);
}

bool Segment::ray_contains(const Point& C) const {
  return vector_product(B - A, C - A) == 0 &&
      scalar_product(B - A, C - A) >= 0;
}

bool Segment::contains(const Point& C) const {
  return vector_product(B - A, C - A) == 0 &&
      in_box(C);
}

double Segment::length() const {
  double dx = B.x - A.x;
  double dy = B.y - A.y;
  return sqrt(dx * dx + dy * dy);
}

double Segment::ray_distance(const Point& C) const {
  if (scalar_product(C - A, B - A) < 0) {
    return Segment(A, C).length();
  } else {
    return abs(vector_product(C - A, B - A)) / length();
  }
}

double Segment::distance(const Point& C) const {
  if (scalar_product(C - A, B - A) < 0 ||
      scalar_product(C - B, A - B) < 0) {
    return min(Segment(A, C).length(),
               Segment(B, C).length());
  } else {
    return fabs(vector_product(C - A, B - A)) / length();
  }
}

int Segment::direction(const Point& C) const {
  double res = vector_product(C - A, B - A);
  if (res == 0.0) {
    return 0;
  } else if (res > 0) {
    return 1;
  } else {
    return -1;
  }
}

bool Segment::intersects(const Segment& another) const {
  int d1 = direction(another.A);
  int d2 = direction(another.B);
  int d3 = another.direction(A);
  int d4 = another.direction(B);

  if ((d1 * d2 < 0 && d3 * d4 < 0) ||
      (d1 == 0 && in_box(another.A)) ||
      (d2 == 0 && in_box(another.B)) ||
      (d3 == 0 && another.in_box(A)) ||
      (d4 == 0 && another.in_box(B))) {
    return true;
  } else {
    return false;
  }
}

bool Segment::ray_intersects(const Segment& another) const {
  Line first;
  Line second;

  first.init_from_two_points(A, B);
  second.init_from_two_points(another.A, another.B);

  Line::IntersectionStatus status;
  Point C;
  first.intersect(second, status, C);

  if (status == Line::PARALLEL) {
    return false;
  } else if (status == Line::SAME) {
    if (scalar_product(B - A, another.B - another.A) > 0 ||
        scalar_product(B - A, another.A - A) >= 0) {
      return true;
    } else {
      return false;
    }
  } else {
    if (scalar_product(C - A, B - A) >= 0 &&
        scalar_product(C - another.A, another.B - another.A) >= 0) {
      return true;
    } else {
      return false;
    }
  }
}

// Lines
void Line::init_from_two_points(const Point& A, const Point& B) {
  a = B.y - A.y;
  b = A.x - B.x;
  c = A.x * (A.y - B.y) + A.y * (B.x - A.x);
}

void Line::init_from_point_and_normal(const Point& A, const Point& normal) {
  a = normal.x;
  b = normal.y;
  c = -normal.x * A.x - normal.y * A.y;
}

void Line::init_from_parallel_line_and_distance(const Line& l, double r) {
  Point P = l.get_point();
  init_from_point_and_normal(
      Point(P.x - l.a * r / sqrt(l.a * l.a + l.b * l.b),
            P.y - l.b * r / sqrt(l.a * l.a + l.b * l.b)),
      Point(l.a, l.b));
}

bool Line::contains(const Point& A) {
  return epsilon_equal(a * A.x + b * A.y + c, 0);
}

Point Line::get_point() const {
  if (a == 0) {
    return Point(0, -c / b);
  } else {
    return Point(-c / a, 0);
  }
}

void Line::get_two_points(Point& first, Point& second) const {
  if (a == 0) {
    first = Point(0, -c / b);
    second = Point(1, -c / b);
  } else {
    first = Point(-c / a, 0);
    second = Point(-(c + b) / a, 1);
  }
}

int Line::direction(const Point& A) {
  double res = a * A.x + b * A.y + c;
  if (epsilon_equal(res, 0)) {
    return 0;
  } else if (epsilon_lower(res, 0)) {
    return -1;
  } else {
    return 1;
  }
}

void Line::intersect(const Line& another,
                     IntersectionStatus& status,
                     Point& point) {
  double d1 = vector_product(Point(a, b), Point(another.a, another.b));
  double d2 = vector_product(Point(b, c), Point(another.b, another.c));
  double d3 = vector_product(Point(c, a), Point(another.c, another.a));
  if (d1 == 0) {
    if (d2 == 0) {
      status = Line::SAME;
    } else {
      status = Line::PARALLEL;
    }
  } else {
    status = Line::INTERSECTS;
    point = Point(d2 / d1, d3 / d1);
  }
}

double Line::distance(const Point& C) {
  Point A, B;
  get_two_points(A, B);

  return abs(vector_product(C - A, B - A)) / Segment(A, B).length();
}

std::istream& operator>>(std::istream& in, Triangle& tringle) {
  in >> tringle.A >> tringle.B >> tringle.C;
  return in;
}

Point Triangle::get_centroid() const {
  return Point((A.x + B.x + C.x) / 3,
               (A.y + B.y + C.y) / 3);
}

Point Triangle::get_orthocenter() const {
  Line first, second;
  first.init_from_point_and_normal(A, Point(B.x - C.x, B.y - C.y));
  second.init_from_point_and_normal(B, Point(A.x - C.x, A.y - C.y));

  Line::IntersectionStatus status;
  Point res;
  first.intersect(second, status, res);
  assert(status == Line::INTERSECTS);
  return res;
}

Point Triangle::get_incircle() const {
  Point A1(A.x + (B - A).x / (B - A).length() + (C - A).x / (C - A).length(),
           A.y + (B - A).y / (B - A).length() + (C - A).y / (C - A).length());

  Point B1(B.x + (A - B).x / (A - B).length() + (C - B).x / (C - B).length(),
           B.y + (A - B).y / (A - B).length() + (C - B).y / (C - B).length());

  Line first, second;
  first.init_from_two_points(A, A1);
  second.init_from_two_points(B, B1);

  Line::IntersectionStatus status;
  Point res;
  first.intersect(second, status, res);
  assert(status == Line::INTERSECTS);
  
  return res;
}

Point Triangle::get_circumcenter() const {
  Line first, second;
  first.init_from_point_and_normal(Point((A.x + B.x) / 2, (A.y + B.y) / 2),
                                   B - A);
  second.init_from_point_and_normal(Point((A.x + C.x) / 2, (A.y + C.y) / 2),
                                    C - A);
  
  Line::IntersectionStatus status;
  Point res;
  first.intersect(second, status, res);
  assert(status == Line::INTERSECTS);
  return res; 
}

std::istream& operator>>(std::istream& in, Polygon& poly) {
  int points_count;
  in >> points_count;
  
  poly.points.resize(points_count);
  for (int i = 0; i < points_count; ++i) {
    in >> poly.points[i];
  }

  return in;
}

void Polygon::init(const vector<Point>& _points) {
  points = _points;
}

bool Polygon::is_convex() const {
  int last_sign = 0;
  for (int i = 0; i < points.size(); ++i) {
    double vp = vector_product(points[(i + 1) % points.size()] - points[i],
                               points[(i + 2) % points.size()] - points[i]);
    int sign;
    if (vp > 0) {
      sign = 1;
    } else if (vp < 0) {
      sign = -1;
    } else {
      sign = 0;
    }

    if (sign != 0) {
      if (last_sign != 0) {
        if (sign != last_sign) {
          return false;
        }
      } else {
        last_sign = sign;
      }
    }
  }

  return true;
}

bool Polygon::contains(const Point& point) const {
  assert(!points.empty());
  
  double sum = 0;
  for (int i = 0; i < points.size(); ++i) {
    Point first(points[i] - point);
    Point second(points[(i + 1) % points.size()] - point);
    sum += atan2(vector_product(first, second), scalar_product(first, second));
    if (Segment(points[i], points[(i + 1) % points.size()]).contains(point)) {
      return true;
    }
  }

  return !epsilon_equal(sum, 0);
}

bool Polygon::convex_contains(const Point& point) const {
  int sign = 0;
  for (int i = 0; i < points.size(); ++i) {
    double vp = vector_product(points[i] - point,
                               points[(i + 1) % points.size()] - point);
    if (vp > 0) {
      vp = 1;
    } else {
      vp = -1;
    }

    if (sign == 0) {
      sign = vp;
    } else {
      if (vp != sign) {
        return false;
      }
    }
  }

  return true;
}

double Polygon::square() const {
  double res = 0;
  for (int i = 1; i + 1 < points.size(); ++i) {
    res += vector_product(points[i + 1] - points[0], points[i] - points[0]);
  }
  return fabs(res) / 2;                       
}

vector<Point> get_convex_hull(const vector<Point>& input_points) {
  vector<Point> points = input_points;
  std::sort(points.begin(), points.end());
  points.resize(std::unique(points.begin(), points.end()) - points.begin());

  if (points.size() < 3) {
    return points;
  }
  
  std::sort(points.begin() + 1, points.end(), AngleComparator(points[0]));
  vector<bool> considerable(points.size(), true);

  for (int i = 1; i + 1 < points.size(); ++i) {
    if (epsilon_equal(vector_product(points[i] - points[0],
                                     points[i + 1] - points[0]), 0)) {
      considerable[i] = false;
    }
  }

  vector<Point> res;
  res.push_back(points[0]);

  for (int i = 1; i < points.size(); ++i) {
    if (considerable[i]) {
      while (res.size() >= 2 &&
             !epsilon_lower(
                 vector_product(points[i] - res[res.size() - 2],
                                res[res.size() - 1] -
                                res[res.size() - 2]), 0)) {
        res.pop_back();
      }
      res.push_back(points[i]);
    }
  }

  return res;
}
int gcd(int a, int b) {
  if (b == 0) {
    return a;
  } else {
    return gcd(b, a % b);
  }
}

int get_interior_integer_points_count(const vector<Point>& points) {
  Polygon poly;
  poly.init(points);
  double s = poly.square();
  double b = points.size();
  for (int i = 0; i < points.size(); ++i) {
    Point first = points[(i + 1) % points.size()];
    Point second = points[i];
    int x = fabs(first.x - second.x);
    int y = fabs(first.y - second.y);
    int add = gcd(x, y) - 1;
    b += add;
  }
  return s - b / 2 + 1;
}

#if 0
  int n;
  cin >> n;
  vector<Point> points(n);
  for (int i = 0; i < n; ++i) {
    cin >> points[i];
  }
  cout << get_interior_integer_points_count(points) << endl;
  
  return 0;
  
  int k;
  Polygon poly;
  cin >> k >> poly;

  double part_square = poly.square() / k;
  double have_so_far = 0;

  Point origin = poly.points[0];

  stack<Point> last;
  for (int i = poly.points.size() - 1; i > 0; --i) {
    last.push(poly.points[i]);
  }

  vector<Point> res;
  
  cout.precision(6);
  int outputed = 0;
  while (outputed < (k - 1)) {
    Point first = last.top();
    last.pop();
    Point second = last.top();
    double s = fabs(vector_product(first - origin, second - origin) / 2);
    if (epsilon_lower(have_so_far + s, part_square)) {
      have_so_far += s;
    } else {
      double l = 0;
      double r = 1;
      double add;
      Point middle_point;
      for (int i = 0; i < 200; ++i) {
        double m = (l + r) / 2;
        middle_point = first + Point(m * (second.x - first.x),
                                     m * (second.y - first.y));

        add = fabs(vector_product(first - origin,
                                  middle_point - origin) / 2);
        if (epsilon_greater(have_so_far + add, part_square)) {
          r = m;
        } else {
          l = m;
        }
      }
      res.push_back(middle_point);
      have_so_far = 0;
      last.push(middle_point);
      ++outputed;
    }
  }


  for (int i = 0; i < res.size(); ++i) {
    cout << std::fixed << origin << ' ' << res[i] << endl;
  }
#endif
