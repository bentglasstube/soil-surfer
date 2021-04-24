#pragma once

#include "spritemap.h"

class Map {
  public:
    Map();

    void draw(Graphics& graphics) const;

    struct GridPoint;

    struct Point {
      const long x, y;
      Point(long x, long y) : x(x), y(y) {}
      GridPoint to_grid() const;
    };

    struct GridPoint {
      const long q, r, s;

      GridPoint(long q, long r, long s);
      GridPoint(long q, long r) : GridPoint(q, r, -q-r) {}
      GridPoint(const GridPoint& gp) : GridPoint(gp.q, gp.r) {}
      GridPoint(const Point& p) : GridPoint(p.to_grid()) {}

      Point draw_point() const;
    };

  private:

    enum class TileType { Air, Dirt, Tunnel, Rock };

    static constexpr int kTileSize = 8;

    SpriteMap tiles_;

    TileType get_tile(const GridPoint& gp) const;
};
