#include "player.h"

Player::Player(const Map& map) : sprites_("worm.png", 8, 16, 16), power_(100), vim_(5.0) {
  segments_.emplace_back(GridPoint(6, 6), Direction::SE, Direction::W);
  while (drop(map)) power_ = 100;
}

void Player::update(Map& map, unsigned int elapsed) {
  vim_ -= elapsed / 30000.0;

  power_ += elapsed;
  if (power_ > 1000) power_ = 1000;
  if (drop(map)) return;

  auto& head = segments_.front();
  auto target = head.p.apply(head.forward);
  if (!occupying(target)) {
    if (power_ > map.strength(target)) {
      power_ = 0;
      grow();
      map.dig(target);
    }
  }

  while (segments_.size() > std::ceil(vim_)) segments_.pop_back();

  // TODO if vim_ <= 1 you dead
}

void Player::draw(Graphics& graphics, long xo, long yo) const {
  auto head = segments_.begin();

  const auto& hdp = head->p.draw_point();
  sprites_.draw(graphics, static_cast<int>(head->forward), hdp.x - xo, hdp.y - yo);
  head = std::next(head);

  while (head != segments_.end()) {
    const auto& dp = (*head).p.draw_point();
    const int n = static_cast<int>(head->forward) + 8;
    sprites_.draw(graphics, n, dp.x - xo, dp.y - yo);
    head = std::next(head);
  }
}

void Player::grow() {
  const auto& head = segments_.front();
  const auto facing = head.forward;
  segments_.emplace_front(head.p.apply(facing), facing, facing.opposite());
}

bool Player::drop(const Map& map) {
  auto head = segments_.front();

  if (head.p.r() % 2 == 1) {
    if (check_and_move(map, Direction::SW)) return true;
    if (check_and_move(map, Direction::SE)) return true;
  } else {
    if (check_and_move(map, Direction::SE)) return true;
    if (check_and_move(map, Direction::SW)) return true;
  }

  return false;
}

void Player::turn_left() {
  auto& head = segments_.front();
  if (head.forward.left().angle(head.backward) > 90) {
    head.forward = head.forward.left();
  }
}

void Player::turn_right() {
  auto& head = segments_.front();
  if (head.forward.right().angle(head.backward) > 90) {
    head.forward = head.forward.right();
  }
}

void Player::eat() {
  vim_ += 0.25;
}

bool Player::occupying(const GridPoint& gp) const {
  for (const auto& s : segments_) {
    if (s.p == gp) return true;
  }
  return false;
}

bool Player::check_and_move(const Map& map, Direction d) {
  auto& head = segments_.front();

  const auto target = head.p.apply(d);
  if (map.open(target) && ! occupying(target)) {
    if (head.backward.angle(d) > 90) {
      int s = map.strength(target);
      if (power_ > s) {
        power_ = 0;
        head.forward = d;
        grow();
        return true;
      }
    } else {
      d == Direction::SW ? turn_left() : turn_right();
    }
  }
  return false;
}
