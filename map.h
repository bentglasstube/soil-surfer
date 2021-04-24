#pragma once

#include <random>
#include <unordered_map>

#include "spritemap.h"

#include "geometry.h"

class Map {
  public:
    Map();

    void draw(Graphics& graphics, long xo, long yo) const;

    void dig(const GridPoint& gp);
    bool open(const GridPoint& gp) const;
    int strength(const GridPoint& gp) const;

  private:

    enum class TileType { Air, Dirt, WetDirt, Tunnel, Rock };

    static constexpr int kTileSize = 8;

    SpriteMap tiles_;
    std::mt19937 rng_;
    double surface_seed_;
    std::unordered_map<GridPoint, TileType> overrides_;

    TileType get_tile(const GridPoint& gp) const;
};

