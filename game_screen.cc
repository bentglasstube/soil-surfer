#include "game_screen.h"

GameScreen::GameScreen() : camera_(), map_(), player_() {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) player_.turn_left();
  if (input.key_pressed(Input::Button::Right)) player_.turn_right();

  player_.update(map_, elapsed);
  camera_.update(player_, elapsed);

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
