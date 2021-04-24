#pragma once

#include <list>

#include "spritemap.h"

#include "map.h"

class Player {
  public:

    Player(const Map& map);

    void draw(Graphics& graphics, long xo, long yo) const;
    void grow();
    void advance();

    void turn_left();
    void turn_right();

    Map::GridPoint head() const { return segments_.front().p; }

  private:

    struct Segment {
      Map::GridPoint p;
      Map::Direction forward, backward;

      Segment(Map::GridPoint p, Map::Direction f, Map::Direction b) : p(p), forward(f), backward(b) {}
    };

    SpriteMap sprites_;
    std::list<Segment> segments_;

};
