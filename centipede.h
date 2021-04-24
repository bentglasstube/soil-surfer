#pragma once

#include "spritemap.h"

#include "geometry.h"
#include "map.h"

class Centipede {
  public:

    Centipede(GridPoint head, int length, Direction travel);

    void update(Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, long xo, long yo) const;

    GridPoint head() const { return head_; }
    GridPoint tail() const { return head_ + GridPoint((length_ + 1) * (travel_ == Direction::W ? 1 : -1), 0); }
    bool touching(GridPoint gp) const;

  private:

    SpriteMap sprites_;
    GridPoint head_;
    Direction travel_;
    int length_, timer_;

    Direction up() const { return travel_ == Direction::W ? Direction::NE : Direction::NW; }
    Direction down() const { return travel_ == Direction::W ? Direction::SE : Direction::SW; }

};
