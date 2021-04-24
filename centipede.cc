#include "centipede.h"

Centipede::Centipede(GridPoint head, int length) :
  sprites_("pedes.png", 8, 16, 16),
  head_(head), length_(length), timer_(0) {}

void Centipede::update(Map& map, unsigned int elapsed) {
  timer_ += elapsed;
  if (timer_ > 200) {
    head_ = head_.apply(Direction::W);

    map.dig(head_);
    map.dig(head_.apply(Direction::NE));
    map.dig(head_.apply(Direction::SE));

    map.eat_food(head_);
    map.eat_food(head_.apply(Direction::NE));
    map.eat_food(head_.apply(Direction::SE));

    timer_ -= 200;
  }
}

void Centipede::draw(Graphics& graphics, long xo, long yo) const {
  const Point hd = head_.draw_point();
  const Point top = head_.apply(Direction::NE).draw_point();
  const Point bot = head_.apply(Direction::SE).draw_point();

  sprites_.draw(graphics, 32, hd.x - xo, hd.y - yo);
  sprites_.draw(graphics, 25, top.x - xo, top.y - yo);
  sprites_.draw(graphics, 41, bot.x - xo, bot.y - yo);

  for (int i = 1; i < length_ - 1; ++i) {
    sprites_.draw(graphics, 33, hd.x - xo + 16 * i, hd.y - yo);
    sprites_.draw(graphics, 26, top.x - xo + 16 * i, top.y - yo);
    sprites_.draw(graphics, 42, bot.x - xo + 16 * i, bot.y - yo);
  }

  sprites_.draw(graphics, 33, hd.x - xo + 16 * length_ - 16, hd.y - yo);
  sprites_.draw(graphics, 36, hd.x - xo + 16 * length_, hd.y - yo);
  sprites_.draw(graphics, 27, top.x - xo + 16 * length_ - 16, top.y - yo);
  sprites_.draw(graphics, 43, bot.x - xo + 16 * length_ - 16, bot.y - yo);
}

bool Centipede::touching(GridPoint gp) const {
  for (int i = 0; i < length_; ++i) {
    if (gp == head_ + GridPoint(i, 0)) return true;
    if (gp == head_.apply(Direction::NE) + GridPoint(i, 0)) return true;
    if (gp == head_.apply(Direction::SE) + GridPoint(i, 0)) return true;
  }

  return gp == tail();
}

