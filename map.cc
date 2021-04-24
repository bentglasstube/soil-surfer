#include "map.h"

#include <cassert>
#include <iostream>

#include "util.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Map::Map() :
  tiles_("tiles.png", 8, 16, 16),
  rng_(Util::random_seed()),
  surface_seed_(std::uniform_real_distribution<double>(0.0, 256.0)(rng_))
{}

void Map::draw(Graphics &graphics, long xo, long yo) const {
  // TODO faster screen covering if needed
  for (long y = 0; y < graphics.height() + kTileSize; y += kTileSize) {
    for (long x = 0; x < graphics.width() + kTileSize; x += kTileSize) {
      const auto gp = Point(x - xo, y - yo).to_grid();
      const auto dp = gp.draw_point();
      tiles_.draw(graphics, static_cast<int>(get_tile(gp)), dp.x + xo, dp.y + yo);
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
  const int sky_height = 12 + (int)(12 * stb_perlin_turbulence_noise3(gp.q / 60.0, 0, surface_seed_, 2.0f, 0.5f, 6));
  if (gp.r <= sky_height) return TileType::Air;

  const long depth = gp.draw_point().y;
  const double rock = std::min(0.05 + depth / 25600.0, 0.65);
  const double dry = std::min(0.10 + depth / 10240.0, 0.80);

  const double vr = stb_perlin_ridge_noise3(gp.r / 60.0, gp.s / 60.0, surface_seed_, 4.0f, 0.5f, 1.0f, 4);
  if (vr < rock) return TileType::Rock;

  const double air = std::max(0.25 - depth / 51200.0, 0.05);
  const double va = stb_perlin_turbulence_noise3(gp.s / 60.0, gp.r / 60.0, surface_seed_, 4.0f, 0.5f, 4);
  if (va < air) return TileType::Tunnel;

  if (vr < dry) return TileType::Dirt;
  return TileType::WetDirt;
}
