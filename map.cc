#include "map.h"

#include <cassert>
#include <iostream>

Map::Map() : tiles_("tiles.png", 8, 16, 16) {}

void Map::draw(Graphics &graphics) const {
  // TODO faster screen covering if needed
  for (long y = -kTileSize; y < graphics.height() + kTileSize; y += kTileSize) {
    for (long x = -kTileSize; x < graphics.width() + kTileSize; x += kTileSize) {
      const auto gp = Point(x, y).to_grid();
      const auto dp = gp.draw_point();

      // TODO get tile at gp
      tiles_.draw(graphics, static_cast<int>(get_tile(gp)), dp.x, dp.y);
    }
  }
}

Map::GridPoint Map::Point::to_grid() const {
  const double px = x / (double)kTileSize;
  const double py = y / (double)kTileSize;
  return GridPoint(std::round(px / 2.0 - py / 3.0), std::round(py * 2 / 3.0));
}

Map::GridPoint::GridPoint(long q, long r, long s) : q(q), r(r), s(s) {
  assert(q + r + s == 0);
}

Map::Point Map::GridPoint::draw_point() const {
  return Point(q * 2 * kTileSize + r * kTileSize - kTileSize, r * 3 * kTileSize / 2 - kTileSize);
}

Map::TileType Map::get_tile(const Map::GridPoint& gp) const {
  return gp.r > 5 ? TileType::Dirt : TileType::Air;
}
