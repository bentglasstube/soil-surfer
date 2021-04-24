#include "game_screen.h"

#include <algorithm>
#include <sstream>

#include "util.h"

GameScreen::GameScreen() :
  rng_(Util::random_seed()),
  map_(rng_()), player_(map_), camera_(rng_(), player_.head().center()),
  text_("text.png"),
  state_(State::Running),
  food_counter_(500), pede_counter_(0),
  max_depth_(0.0), fader_() {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (state_ == State::Running) {
    if (input.key_pressed(Input::Button::Left)) player_.turn_left();
    if (input.key_pressed(Input::Button::Right)) player_.turn_right();
    if (input.key_pressed(Input::Button::Start)) {
      state_ = State::Paused;
      fader_.reset(0x000000cc, 500);
    }

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

    if (player_.dead()) {
      state_ = State::Dead;
      fader_.reset(0x000000ff, 3000);
    }

    const double depth = player_.head().center().y / 8 * 0.005;
    if (depth > max_depth_) max_depth_ = depth;

  } else if (state_ == State::Paused) {
    if (input.key_pressed(Input::Button::Start)) {
      state_ = State::Running;
    }
    fader_.update(elapsed);

  } else if (state_ == State::Dead) {
    if (input.key_pressed(Input::Button::Start)) return false;
    fader_.update(elapsed);
  }

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

  if (state_ == State::Paused) {
    fader_.draw(graphics);
    // TODO make fancy pause
    text_.draw(graphics, "P A U S E", 128, 104, Text::Alignment::Center);
  }

  if (state_ == State::Dead) {
    fader_.draw(graphics);
    text_.draw(graphics, "G A M E", 128, 92, Text::Alignment::Center);
    text_.draw(graphics, "O V E R", 128, 116, Text::Alignment::Center);
  }

  std::ostringstream out;
  out.precision(2);
  out << "Depth: " << std::fixed << max_depth_ << "m";
  text_.draw(graphics, out.str(), 4, 204);
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}

GameScreen::Fader::Fader() : color_(0), duration_(0), timer_(0) {}

void GameScreen::Fader::reset(Graphics::Color color, unsigned int duration) {
  color_ = color;
  duration_ = duration;
  timer_ = 0;
}

void GameScreen::Fader::draw(Graphics& graphics) const {
  const double value = timer_ / (double) duration_;
  const int opacity = value * (color_ & 0xff);
  graphics.draw_rect({0, 0}, {graphics.width(), graphics.height()}, (color_ & 0xffffff00) | opacity, true);
}

void GameScreen::Fader::update(unsigned int elapsed) {
  if (timer_ < duration_) timer_ = std::min(duration_, timer_ + elapsed);
}
