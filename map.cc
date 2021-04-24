#include "map.h"

#include <cmath>

#include "util.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

Map::Map() :
  tiles_("tiles.png", 8, 16, 16),
  rng_(Util::random_seed()),
  seed_(std::uniform_real_distribution<double>(0.0, 256.0)(rng_))
{}

void Map::draw(Graphics &graphics, long xo, long yo) const {
  // TODO faster screen covering if needed
  for (long y = 0; y < graphics.height() + kTileSize; y += kTileSize) {
    for (long x = 0; x < graphics.width() + kTileSize; x += kTileSize) {
      const auto gp = Point(x + xo, y + yo).to_grid();
      const auto dp = gp.draw_point();
      tiles_.draw(graphics, static_cast<int>(get_tile(gp)), dp.x - xo, dp.y - yo);

      if (food_.find(gp) != food_.end()) tiles_.draw(graphics, 8, dp.x - xo, dp.y - yo);
    }
  }
}

void Map::dig(const GridPoint& gp) {
  auto t = get_tile(gp);
  if (t != TileType::Air) overrides_.insert(std::make_pair(gp, TileType::Tunnel));
}

bool Map::open(const GridPoint& gp) const {
  return strength(gp) < 200;
}

int Map::strength(const GridPoint& gp) const {
  auto t = get_tile(gp);
  switch (t) {
    case TileType::Air:       return 50;
    case TileType::Tunnel:    return 150;
    case TileType::WetDirt:   return 300;
    case TileType::Dirt:      return 500;
    default:                  return 9000;
  }
}

void Map::spawn_food(const GridPoint& head) {
  const double a = std::uniform_real_distribution<double>(1, 5)(rng_) * M_PI / 6.0;
  const double r = std::uniform_real_distribution<double>(15, 25)(rng_) * kTileSize;

  const auto tg = (head.center() + Point(r * std::cos(a), r * std::sin(a))).to_grid();
  if (get_tile(tg) == TileType::WetDirt) food_.insert(tg);
}

bool Map::eat_food(const GridPoint& head) {
  return food_.erase(head) > 0;
}

Map::TileType Map::get_tile(const GridPoint& gp) const {
  auto it = overrides_.find(gp);
  if (it != overrides_.end()) {
    return it->second;
  }

  const int sky_height = 12 + (int)(12 * stb_perlin_turbulence_noise3(gp.q() / 60.0, 0, seed_, 2.0f, 0.5f, 6));
  if (gp.r() <= sky_height) return TileType::Air;

  const long depth = gp.draw_point().y;
  const double rock = std::min(0.05 + depth / 25600.0, 0.65);
  const double dry = std::min(0.10 + depth / 10240.0, 0.80);

  const double vr = stb_perlin_ridge_noise3(gp.r() / 60.0, gp.s() / 60.0, seed_, 4.0f, 0.5f, 1.0f, 4);
  if (vr < rock) return TileType::Rock;

  const double air = std::max(0.25 - depth / 51200.0, 0.05);
  const double va = stb_perlin_turbulence_noise3(gp.s() / 60.0, gp.r() / 60.0, seed_, 4.0f, 0.5f, 4);
  if (va < air) return TileType::Tunnel;

  if (vr < dry) return TileType::Dirt;
  return TileType::WetDirt;
}
