#include "player.h"

Player::Player(const Map& map) : sprites_("worm.png", 8, 16, 16), power_(100), vim_(5.0) {
  segments_.emplace_back(GridPoint(6, 6), Direction::SE, Direction::W);
  while (drop(map)) power_ = 100;
}

void Player::update(Map& map, unsigned int elapsed) {
  vim_ -= elapsed / 30000.0;

  power_ += elapsed;
  if (power_ > 1000) {
    vim_ -= 1;
    power_ -= 1000;
  }

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
}

void Player::draw(Graphics& graphics, long xo, long yo) const {
  auto head = segments_.begin();

  const auto& hdp = head->p.draw_point();
  sprites_.draw(graphics, head->sprite(), hdp.x - xo, hdp.y - yo);
  head = std::next(head);

  while (head != segments_.end()) {
    const auto& dp = (*head).p.draw_point();
    sprites_.draw(graphics, head->sprite() + 24, dp.x - xo, dp.y - yo);
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

void Player::injure(const Centipede& pede) {
  int n = 0;
  for (auto& s : segments_) {
    if (n > 0 || pede.touching(s.p)) ++n;
  }

  vim_ -= n;
}

int Player::Segment::sprite() const {
  switch (static_cast<int>(forward) * 10 + static_cast<int>(backward)) {
    case  2: return 0;
    case 15: return 1;
    case 20: return 2;
    case 35: return 3;
    case 42: return 4;
    case 51: return 5;
    case  3: return 8;
    case 14: return 9;
    case 25: return 10;
    case 30: return 11;
    case 41: return 12;
    case 52: return 13;
    case  4: return 16;
    case 13: return 17;
    case 24: return 18;
    case 31: return 19;
    case 40: return 20;
    case 53: return 21;
    default: return 99;
  }
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
