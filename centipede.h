#pragma once

#include "spritemap.h"

#include "geometry.h"
#include "map.h"

class Centipede {
  public:

    Centipede(GridPoint head, int length);

    void update(Map& map, unsigned int elapsed);
    void draw(Graphics& graphics, long xo, long yo) const;

    GridPoint head() const { return head_; }
    GridPoint tail() const { return head_ + GridPoint(length_ + 1, 0); }
    bool touching(GridPoint gp) const;

  private:

    SpriteMap sprites_;
    GridPoint head_;
    int length_, timer_;

};
