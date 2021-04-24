#include "title_screen.h"

#include "game_screen.h"

TitleScreen::TitleScreen() : text_("text.png"), blink_timer_(0) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  blink_timer_ = (blink_timer_ + elapsed) % 1000;

  if (input.any_pressed()) return false;
  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  if (blink_timer_ < 500) {
    text_.draw(graphics, "Press any key", 128, 200, Text::Alignment::Center);
  }
}

Screen* TitleScreen::next_screen() const {
  return new GameScreen();
}
