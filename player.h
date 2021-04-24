#pragma once

#include <list>

#include "spritemap.h"

#include "geometry.h"
#include "map.h"

class Player {
  public:

    Player(const Map& map);

    void update(Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, long xo, long yo) const;
    void grow();
    void advance();
    bool drop(const Map& map);

    void turn_left();
    void turn_right();

    GridPoint head() const { return segments_.front().p; }

  private:

    struct Segment {
      GridPoint p;
      Direction forward, backward;

      Segment(GridPoint p, Direction f, Direction b) : p(p), forward(f), backward(b) {}
    };

    SpriteMap sprites_;
    std::list<Segment> segments_;
    int power_;

    bool occupying(const GridPoint& gp) const;
    bool check_and_move(const Map& map, Direction dir);
};
