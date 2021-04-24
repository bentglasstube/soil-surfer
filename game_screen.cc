#include "game_screen.h"

GameScreen::GameScreen() : map_() {}

bool GameScreen::update(const Input&, Audio&, unsigned int) {
  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  map_.draw(graphics);
}

Screen* GameScreen::next_screen() const {
  return nullptr;
}
