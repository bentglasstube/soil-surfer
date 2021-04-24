#pragma once

#include <random>

#include "spritemap.h"

class Map {
  public:
    Map();

    void draw(Graphics& graphics, long xo, long yo) const;

    struct Direction {
      enum Value : int8_t{ NW, NE, E, SE, SW, W };

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


    struct GridPoint;
    struct Point {
      const long x, y;

      Point(long x, long y) : x(x), y(y) {}

      GridPoint to_grid() const;

      Point operator+(Point other) const { return Point(x + other.x, y + other.y); }
      Point operator-(Point other) const { return Point(x - other.x, y - other.y); }
    };

    struct GridPoint {
      const long q, r, s;

      GridPoint(long q, long r, long s);
      GridPoint(long q, long r) : GridPoint(q, r, -q-r) {}
      GridPoint(const GridPoint& gp) : GridPoint(gp.q, gp.r) {}
      GridPoint(const Point& p) : GridPoint(p.to_grid()) {}

      Point center() const;
      Point draw_point() const;

      GridPoint apply(Direction d) const;
    };

  private:

    enum class TileType { Air, Dirt, WetDirt, Tunnel, Rock };

    static constexpr int kTileSize = 8;

    SpriteMap tiles_;
    std::mt19937 rng_;
    double surface_seed_;

    TileType get_tile(const GridPoint& gp) const;
};
