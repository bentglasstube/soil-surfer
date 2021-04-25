#pragma once

#include <cassert>
#include <cmath>
#include <functional>

struct Direction {
  enum Value : int { NW, NE, E, SE, SW, W };

  Direction() = default;
  constexpr Direction(Value v) : value(v) {}
  operator Value() const { return value; }
  explicit operator bool() = delete;
  Direction opposite() const;
  Direction left() const { return static_cast<Value>((value + 5) % 6); }
  Direction right() const { return static_cast<Value>((value + 1) % 6); }
  int angle(Value other) const;

  Value value;
};

class GridPoint;
struct Point {
  long x, y;

  Point(long x, long y) : x(x), y(y) {}

  GridPoint to_grid() const;

  Point operator+(const Point& other) const { return Point(x + other.x, y + other.y); }
  Point operator-(const Point& other) const { return Point(x - other.x, y - other.y); }
};

class GridPoint {
  public:
    GridPoint(long q, long r, long) : GridPoint(q, r) {}
    GridPoint(long q, long r) : q_(q), r_(r) {}
    GridPoint(const Point& p) : GridPoint(p.to_grid()) {}

    Point center() const;
    Point draw_point() const;

    long q() const { return q_; }
    long r() const { return r_; }
    long s() const { return -q_ - r_; }

    GridPoint apply(Direction d) const;
    bool operator==(const GridPoint& other) const { return q() == other.q() && r() == other.r(); }

    long dist(const GridPoint& other) const;

    GridPoint operator+(const GridPoint& other) const { return GridPoint(q() + other.q(), r() + other.r()); }
    GridPoint operator-(const GridPoint& other) const { return GridPoint(q() - other.q(), r() - other.r()); }

    static constexpr double kTileSize = 16.0;

  private:
    long q_, r_;
};

namespace std {
  template<> struct hash<GridPoint> {
    size_t operator()(const GridPoint& k) const {
      hash<long> h;
      size_t hq = h(k.q());
      size_t hr = h(k.r());
      return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
    }
  };
}
