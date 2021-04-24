#pragma once

#include <random>
#include <unordered_map>
#include <unordered_set>

#include "spritemap.h"

#include "geometry.h"

class Map {
  public:
    Map();

    void draw(Graphics& graphics, long xo, long yo) const;

    void dig(const GridPoint& gp);
    bool open(const GridPoint& gp) const;
    int strength(const GridPoint& gp) const;
    void spawn_food(const GridPoint& head);

    bool eat_food(const GridPoint& head);

  private:

    enum class TileType { Air, Dirt, WetDirt, Tunnel, Rock };

    static constexpr int kTileSize = 8;

    SpriteMap tiles_;
    std::mt19937 rng_;
    double seed_;

    std::unordered_map<GridPoint, TileType> overrides_;
    std::unordered_set<GridPoint> food_;

    TileType get_tile(const GridPoint& gp) const;
};
