#include "game_screen.h"

#include <algorithm>
#include <sstream>

#include "util.h"

GameScreen::GameScreen() :
  rng_(Util::random_seed()),
  map_(rng_()), player_(map_), camera_(rng_(), player_.head().center()),
  text_("text.png"),
  food_counter_(500), pede_counter_(0),
  max_depth_(0.0) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) player_.turn_left();
  if (input.key_pressed(Input::Button::Right)) player_.turn_right();

  player_.update(map_, elapsed);
  camera_.update(player_, elapsed);

  for (auto& p : pedes_) {
    p.update(map_, elapsed);
    player_.injure(p);
    const long dist = player_.head().dist(p.head());
    const double intensity = 1 - ((dist - 5) * (dist - 5) / 625.0);
    camera_.shake(intensity);
  }

  pedes_.erase(std::remove_if( pedes_.begin(), pedes_.end(),
        [this](const Centipede& p) {
          return p.tail().q() + 30 < player_.head().q();
        }), pedes_.end());

  food_counter_ -= elapsed;
  if (food_counter_ < 0) {
    map_.spawn_food(player_.head());
    food_counter_ += std::uniform_int_distribution<int>(1000, 3000)(rng_);
  }

  if (max_depth_ >= 1.0) pede_counter_ -= elapsed;
  if (pede_counter_ < 0) {
    pede_counter_ += std::uniform_int_distribution<int>(20000, 40000)(rng_);

    const long q = std::uniform_int_distribution<int>(15, 35)(rng_);
    const long r = std::uniform_int_distribution<int>(0, 2)(rng_);
    const long l = 2 * std::uniform_int_distribution<int>(3, 7)(rng_);

    if (std::uniform_real_distribution<double>(0, 1)(rng_) < 0.5) {
      pedes_.emplace_back(player_.head() + GridPoint(q, r), l, Direction::W);
    } else {
      pedes_.emplace_back(player_.head() + GridPoint(-q - r, r), l, Direction::E);
    }
  }

  if (map_.eat_food(player_.head())) {
    // idk play a sound or something?
    player_.eat();
  }

  const double depth = player_.head().center().y / 8 * 0.005;
  if (depth > max_depth_) max_depth_ = depth;

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  player_.draw(graphics, camera_.xoffset(), camera_.yoffset());

  for (const auto& p : pedes_) {
    p.draw(graphics, camera_.xoffset(), camera_.yoffset());
  }

#ifndef NDEBUG
  camera_.inner_focus().draw(graphics, camera_.xoffset(), camera_.yoffset(), 0xffff00ff, false);
  camera_.outer_focus().draw(graphics, camera_.xoffset(), camera_.yoffset(), 0xff0000ff, false);
#endif

  std::ostringstream out;
  out.precision(2);
  out << "Depth: " << std::fixed << max_depth_ << "m";
  text_.draw(graphics, out.str(), 4, 204);
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
