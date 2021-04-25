#pragma once

#include "backdrop.h"
#include "screen.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    TitleScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override { return "title.ogg"; }

  private:

    Backdrop back_;
    Text text_;
    unsigned int blink_timer_;
};
