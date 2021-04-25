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

  const int f = flip() ? -1 : 1;
  const int stride = GridPoint::kTileSize * 2 * f;

  const long tly = hd.y - 19 - yo;
  const long bly = tly + GridPoint::kTileSize * 7 / 2 - 4;

  const long flx = hd.x - xo;

  draw_legs(graphics, flx + f * 12, tly + 1, 0);
  draw_legs(graphics, flx + f * 21, tly, 1);
  draw_legs(graphics, flx + f * 30, tly, 2);

  draw_legs(graphics, flx + f * 12, bly - 1, 8);
  draw_legs(graphics, flx + f * 21, bly, 9);
  draw_legs(graphics, flx + f * 30, bly, 10);

  sprites_.draw_ex(graphics, 32, hd.x - xo, hd.y - yo, flip(), 0, 0, 0);
  sprites_.draw_ex(graphics, 25, top.x - xo, top.y - yo, flip(), 0, 0, 0);
  sprites_.draw_ex(graphics, 41, bot.x - xo, bot.y - yo, flip(), 0, 0, 0);

  for (int i = 1; i < length_ - 1; ++i) {
    const long mlx = hd.x + stride * i - xo;

    draw_legs(graphics, mlx + f *  8, tly, (3 * i) % 8);
    draw_legs(graphics, mlx + f * 18, tly, (5 * i) % 8);
    draw_legs(graphics, mlx + f * 28, tly, (7 * i) % 8);

    draw_legs(graphics, mlx + f *  8, bly, (9 * i) % 8 + 8);
    draw_legs(graphics, mlx + f * 18, bly, (11 * i) % 8 + 8);
    draw_legs(graphics, mlx + f * 28, bly, (13 * i) % 8 + 8);

    sprites_.draw_ex(graphics, 33, hd.x - xo + stride * i, hd.y - yo, flip(), 0, 0 ,0);
    sprites_.draw_ex(graphics, 26, top.x - xo + stride * i, top.y - yo, flip(), 0, 0 ,0);
    sprites_.draw_ex(graphics, 42, bot.x - xo + stride * i, bot.y - yo, flip(), 0, 0 ,0);
  }

  const long rlx = hd.x + stride * length_ - stride - xo;

  draw_legs(graphics, rlx + f *  9, tly, 6);
  draw_legs(graphics, rlx + f * 18, tly, 7);
  draw_legs(graphics, rlx + f * 27, tly, 0);
  draw_legs(graphics, rlx + f * 35, tly + 2, 1);

  draw_legs(graphics, rlx + f *  9, bly, 14);
  draw_legs(graphics, rlx + f * 18, bly, 15);
  draw_legs(graphics, rlx + f * 27, bly, 8);
  draw_legs(graphics, rlx + f * 35, bly - 2, 9);

  sprites_.draw_ex(graphics, 33, hd.x - xo + stride * length_ - stride , hd.y - yo, flip(), 0, 0 ,0);
  sprites_.draw_ex(graphics, 36, hd.x - xo + stride * length_, hd.y - yo, flip(), 0, 0 ,0);
  sprites_.draw_ex(graphics, 27, top.x - xo + stride * length_ - stride , top.y - yo, flip(), 0, 0 ,0);
  sprites_.draw_ex(graphics, 43, bot.x - xo + stride * length_ - stride , bot.y - yo, flip(), 0, 0 ,0);
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

void Centipede::draw_legs(Graphics& graphics, long x, long y, int mod) const {
  const int n = (mod + leg_frame()) % 8 + (mod > 7 ? 8 : 0);
  sprites_.draw_ex(graphics, n, x, y, flip(), 0, 0, 0);
}


