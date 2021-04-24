#include "game_screen.h"

#include <sstream>

GameScreen::GameScreen() : camera_(), map_(), player_(), text_("text.png"), food_counter_(500) {}

bool GameScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Left)) player_.turn_left();
  if (input.key_pressed(Input::Button::Right)) player_.turn_right();

  player_.update(map_, elapsed);
  camera_.update(player_, elapsed);

  food_counter_ -= elapsed;
  if (food_counter_ < 0) {
    map_.spawn_food(player_.head());
    food_counter_ += 1000;
  }

  if (map_.eat_food(player_.head())) {
    // idk play a sound or something?
    player_.eat();
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

  const double depth = player_.head().center().y / 8 * 0.005;

  std::ostringstream out;
  out.precision(2);
  out << "Depth: " << std::fixed << depth << "m";
  text_.draw(graphics, out.str(), 4, 204);
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
