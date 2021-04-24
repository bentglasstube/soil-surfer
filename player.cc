#include "player.h"

Player::Player(const Map& map) : sprites_("worm.png", 8, 16, 16) {
  // TODO randomize starting position more
  segments_.emplace_back(Map::GridPoint(6, 6, -12), Map::Direction::SE, Map::Direction::W);
  for (size_t i = 1; i < 5; ++i) {
    segments_.emplace_back(Map::GridPoint(6 - i, 6, i - 12), Map::Direction::E, Map::Direction::W);
  }

  // TODO make the worm fall to earth
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
