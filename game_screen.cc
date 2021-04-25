#include "game_screen.h"

#include <algorithm>
#include <sstream>

#include "util.h"

#include "title_screen.h"

std::string format(std::string label, double value, std::string unit) {
  std::ostringstream out;
  out.precision(2);
  out << label << ": " << std::fixed << value << unit;
  return out.str();
}

GameScreen::GameScreen() :
  rng_(Util::random_seed()),
  map_(rng_()), player_(map_), camera_(rng_(), player_.head().center()),
  text_("text.png"),
  state_(State::Running),
  food_counter_(500), pede_counter_(0),
  max_depth_(0.0), max_length_(0.0), fader_() {}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (state_ == State::Running) {
    if (input.key_pressed(Input::Button::Left)) player_.turn_left();
    if (input.key_pressed(Input::Button::Right)) player_.turn_right();
    if (input.key_pressed(Input::Button::Start)) {
      state_ = State::Paused;
      fader_.reset(0x000000cc, 500);
      audio.music_volume(3);
    }

    player_.update(audio, map_, elapsed);
    camera_.update(player_, elapsed);

    for (auto& p : pedes_) {
      p.update(audio, map_, elapsed);
      player_.injure(audio, p);
      const long dist = player_.head().dist(p.head());
      const double intensity = 1 - ((dist - 5) * (dist - 5) / 625.0);
      camera_.shake(intensity);
    }

    pedes_.erase(std::remove_if( pedes_.begin(), pedes_.end(),
          [this](const Centipede& p) {
            return p.head().dist(player_.head()) > 69; // NICE
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

      audio.play_random_sample("boom.wav", 6);
    }

    const int food_value = map_.eat_food(player_.head());
    if (food_value > 0) {
      player_.eat(audio, food_value);
    }

    if (player_.dead()) {
      state_ = State::Dead;
      fader_.reset(0x000000ff, 3000);
      audio.play_music("wormfood.ogg", true);
    }

    const double depth = player_.head().center().y / GridPoint::kTileSize * 0.005;
    if (depth > max_depth_) max_depth_ = depth;

    const double length = player_.vim() * 0.5;
    if (length > max_length_) max_length_ = length;

  } else if (state_ == State::Paused) {
    if (input.key_pressed(Input::Button::Start)) {
      state_ = State::Running;
      audio.music_volume(10);
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

  if (player_.vim() < 2) {
    const double opacity = 2 - player_.vim();
    const int pixels = GridPoint::kTileSize * 2;
    for (int i = 0; i < pixels; ++i) {
      const int alpha = (255 - 255 * i / pixels) * opacity;
      graphics.draw_rect({i, i}, {graphics.width() - i, graphics.height() - i}, 0xff000000 + alpha, false);
    }
  }

  if (state_ == State::Paused) {
    fader_.draw(graphics);
    // TODO make fancy pause
    text_.draw(graphics, "P A U S E", graphics.width() / 2, graphics.height() / 2 - 8, Text::Alignment::Center);
  }

  if (state_ == State::Dead) {
    fader_.draw(graphics);
    text_.draw(graphics, "G A M E", graphics.width() / 2, graphics.height() / 2 - 12, Text::Alignment::Center);
    text_.draw(graphics, "O V E R", graphics.width() / 2, graphics.height() / 2 + 12, Text::Alignment::Center);
  }

  text_.draw(graphics, format("Max Depth", max_depth_, "m"), 4, graphics.height() - 40);
  text_.draw(graphics, format("Max Length", max_length_, "cm"), 4, graphics.height() - 20);
}

Screen* GameScreen::next_screen() const {
  return new TitleScreen();
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
