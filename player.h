#pragma once

#include <list>

#include "spritemap.h"

#include "centipede.h"
#include "geometry.h"
#include "map.h"

class Player {
  public:

    Player(const Map& map);

    void update(Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, long xo, long yo) const;
    void grow();
    bool drop(const Map& map);

    void turn_left();
    void turn_right();
    void eat(int value);
    void injure(const Centipede& pede);

    GridPoint head() const { return segments_.front().p; }
    bool dead() const { return vim_ <= 1; }
    double vim() const { return vim_; }

  private:

    struct Segment {
      GridPoint p;
      Direction forward, backward;

      Segment(GridPoint p, Direction f, Direction b) : p(p), forward(f), backward(b) {}

      int sprite() const;
    };

    SpriteMap sprites_;
    std::list<Segment> segments_;
    int power_;
    double vim_;

    bool occupying(const GridPoint& gp) const;
    bool check_and_move(const Map& map, Direction dir);
};
