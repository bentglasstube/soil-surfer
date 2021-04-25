#pragma once

#include <random>

#include "screen.h"
#include "text.h"

#include "camera.h"
#include "centipede.h"
#include "map.h"
#include "player.h"

class GameScreen : public Screen {
  public:

    GameScreen();

    bool update(const Input&, Audio&, unsigned int) override;
    void draw(Graphics&) const override;
    Screen* next_screen() const override;

    std::string get_music_track() const override { return "deepdiggin.ogg"; }

  private:

    enum class State { Running, Paused, Dead };

    std::mt19937 rng_;

    Map map_;
    Player player_;
    Camera camera_;
    Text text_;
    State state_;

    class Fader {
      public:
        Fader();

        void reset(Graphics::Color color, unsigned int duration);
        void draw(Graphics& graphics) const;
        void update(unsigned int elapsed);

      private:
        Graphics::Color color_;
        unsigned int duration_, timer_;
    };

    int food_counter_, pede_counter_;
    double max_depth_;

    Fader fader_;

    std::vector<Centipede> pedes_;
};
