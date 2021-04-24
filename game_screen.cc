#include "game_screen.h"

GameScreen::GameScreen() : map_(), xo_(0.0), yo_(0.0) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_held(Input::Button::Up)) yo_ += elapsed / 100.0;
  if (input.key_held(Input::Button::Down)) yo_ -= elapsed / 100.0;
  if (input.key_held(Input::Button::Left)) xo_ += elapsed / 100.0;
  if (input.key_held(Input::Button::Right)) xo_ -= elapsed / 100.0;

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, std::round(xo_), std::round(yo_));
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
