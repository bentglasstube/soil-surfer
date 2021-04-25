#pragma once

#include <random>
#include <unordered_map>

#include "audio.h"
#include "spritemap.h"

#include "geometry.h"

class Map {
  public:
    Map(unsigned int seed);

    void draw(Graphics& graphics, long xo, long yo) const;

    void dig(Audio& audio, const GridPoint& gp);
    bool open(const GridPoint& gp) const;
    int strength(const GridPoint& gp) const;
    void spawn_food(const GridPoint& head);

    int eat_food(const GridPoint& head);

    void destroy(Audio& audio, const GridPoint& p) { dig(audio, p); eat_food(p); }

  private:

    enum class TileType { Air, Dirt, WetDirt, Tunnel, Rock };

    SpriteMap tiles_;
    std::mt19937 rng_;
    double seed_;

    std::unordered_map<GridPoint, TileType> overrides_;
    std::unordered_map<GridPoint, int> food_;

    TileType get_tile(const GridPoint& gp) const;
    int get_sprite(const GridPoint& gp) const;
};
