#pragma once

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "map.h"
#include "player.h"

class GameScreen : public Screen {
  public:

    GameScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;
    Screen* next_screen() const override;

  private:

    Map map_;
    Player player_;
    Camera camera_;
    Text text_;

    int food_counter_;
    double max_depth_;
};
