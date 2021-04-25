#include "centipede.h"

Centipede::Centipede(GridPoint head, int length, Direction travel) :
  sprites_("pedes.png", 8, GridPoint::kTileSize * 2, GridPoint::kTileSize * 2),
  head_(head), travel_(travel),
  length_(length), timer_(0) {}

void Centipede::update(Map& map, unsigned int elapsed) {
  timer_ += elapsed;
  if (timer_ > 100) {
    head_ = head_.apply(travel_);

    map.destroy(head_);
    map.destroy(head_.apply(up()));
    map.destroy(head_.apply(down()));

    timer_ -= 100;
  }
}

void Centipede::draw(Graphics& graphics, long xo, long yo) const {
  const Point hd = head_.draw_point();
  const Point top = head_.apply(up()).draw_point();
  const Point bot = head_.apply(down()).draw_point();

  const int stride = GridPoint::kTileSize * (travel_ == Direction::W ? 2 : -2);
  const bool flip = travel_ != Direction::W;

  sprites_.draw_ex(graphics, 32, hd.x - xo, hd.y - yo, flip, 0, 0, 0);
  sprites_.draw_ex(graphics, 25, top.x - xo, top.y - yo, flip, 0, 0, 0);
  sprites_.draw_ex(graphics, 41, bot.x - xo, bot.y - yo, flip, 0, 0, 0);

  for (int i = 1; i < length_ - 1; ++i) {
    sprites_.draw_ex(graphics, 33, hd.x - xo + stride * i, hd.y - yo, flip, 0, 0 ,0);
    sprites_.draw_ex(graphics, 26, top.x - xo + stride * i, top.y - yo, flip, 0, 0 ,0);
    sprites_.draw_ex(graphics, 42, bot.x - xo + stride * i, bot.y - yo, flip, 0, 0 ,0);
  }

  sprites_.draw_ex(graphics, 33, hd.x - xo + stride * length_ - stride , hd.y - yo, flip, 0, 0 ,0);
  sprites_.draw_ex(graphics, 36, hd.x - xo + stride * length_, hd.y - yo, flip, 0, 0 ,0);
  sprites_.draw_ex(graphics, 27, top.x - xo + stride * length_ - stride , top.y - yo, flip, 0, 0 ,0);
  sprites_.draw_ex(graphics, 43, bot.x - xo + stride * length_ - stride , bot.y - yo, flip, 0, 0 ,0);
}

bool Centipede::touching(GridPoint gp) const {
  const int stride = travel_ == Direction::W ? 1 : -1;
  for (int i = 0; i < length_; ++i) {
    if (gp == head_ + GridPoint(i * stride, 0)) return true;
    if (gp == head_.apply(up()) + GridPoint(i * stride, 0)) return true;
    if (gp == head_.apply(down()) + GridPoint(i * stride, 0)) return true;
  }

  return gp == tail();
}

