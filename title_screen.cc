#include "title_screen.h"

#include "game_screen.h"

TitleScreen::TitleScreen() : back_("title.png"), text_("text.png"), blink_timer_(0) {}

bool TitleScreen::update(const Input& input, Audio&, unsigned int elapsed) {
  blink_timer_ = (blink_timer_ + elapsed) % 844;

  if (input.any_pressed()) return false;
  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  back_.draw(graphics);
  if (blink_timer_ < 422) {
    text_.draw(graphics, "Press any key", graphics.width() / 2, graphics.height() - 48, Text::Alignment::Center);
  }
}

Screen* TitleScreen::next_screen() const {
  return new GameScreen();
}
