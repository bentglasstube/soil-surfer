#include "game_screen.h"

GameScreen::GameScreen() : camera_(), map_(), player_(map_), timer_(0) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::A)) player_.advance();
  if (input.key_pressed(Input::Button::Left)) player_.turn_left();
  if (input.key_pressed(Input::Button::Right)) player_.turn_right();

  camera_.update(player_, elapsed);

  timer_ += elapsed;
  if (timer_ > 300) {
    player_.advance();
    timer_ -= 300;
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  player_.draw(graphics, camera_.xoffset(), camera_.yoffset());

#ifndef NDEBUG
  camera_.inner_focus().draw(graphics, camera_.xoffset(), camera_.yoffset(), 0xffff00ff, false);
  camera_.outer_focus().draw(graphics, camera_.xoffset(), camera_.yoffset(), 0xff0000ff, false);
#endif

}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
