#include "game_screen.h"

GameScreen::GameScreen() : map_(), player_(map_), xo_(0.0), yo_(0.0) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::A)) player_.advance();
  if (input.key_pressed(Input::Button::Left)) player_.turn_left();
  if (input.key_pressed(Input::Button::Right)) player_.turn_right();

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, std::round(xo_), std::round(yo_));
  player_.draw(graphics, std::round(xo_), std::round(yo_));
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
