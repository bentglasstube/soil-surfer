#include "player.h"

Player::Player(const Map& map) : sprites_("worm.png", 8, 16, 16), power_(0) {
  // TODO randomize starting position more
  segments_.emplace_back(Map::GridPoint(6, 6, -12), Map::Direction::SE, Map::Direction::W);
  for (size_t i = 1; i < 5; ++i) {
    segments_.emplace_back(Map::GridPoint(6 - i, 6, i - 12), Map::Direction::E, Map::Direction::W);
  }
}

void Player::update(Map& map, unsigned int elapsed) {
  power_ += elapsed;
  if (power_ > 1000) power_ = 1000;

  if (power_ > 50 && drop(map)) {
    power_ -= 50;
    return;
  }

  auto& head = segments_.front();
  int target = map.strength(head.p.apply(head.forward));
  if (power_ > target) {
    power_ -= target;
    advance();
  }
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

void Player::advance() {
  grow();
  segments_.pop_back();
}

bool Player::drop(const Map& map) {
  auto head = segments_.front();

  if (head.p.r % 2 == 1) {
    if (check_and_move(map, Map::Direction::SW)) return true;
    if (check_and_move(map, Map::Direction::SE)) return true;
  } else {
    if (check_and_move(map, Map::Direction::SE)) return true;
    if (check_and_move(map, Map::Direction::SW)) return true;
  }

  return false;
}

void Player::turn_left() {
  auto& head = segments_.front();
  if (head.forward.left().angle(head.backward) > 90) {
    head.forward = head.forward.left();
    power_ = 0;
  }
}

void Player::turn_right() {
  auto& head = segments_.front();
  if (head.forward.right().angle(head.backward) > 90) {
    head.forward = head.forward.right();
    power_ = 0;
  }
}

bool Player::occupying(const Map::GridPoint& gp) const {
  for (const auto& s : segments_) {
    if (s.p == gp) return true;
  }
  return false;
}

bool Player::check_and_move(const Map& map, Map::Direction d) {
  auto& head = segments_.front();

  const auto target = head.p.apply(d);
  if (map.strength(target) < 100 && ! occupying(target)) {
    if (head.backward.angle(d) > 90) {
      head.forward = d;
      advance();
      return true;
    } else {
      d == Map::Direction::SW ? turn_left() : turn_right();
    }
  }
  return false;
}
