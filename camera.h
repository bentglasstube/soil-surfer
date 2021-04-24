#pragma once

#include "rect.h"

#include "player.h"

class Camera {
  public:

    Camera();

    void update(const Player& focus, unsigned int elapsed);

    long xoffset() const { return std::round(xo_); }
    long yoffset() const { return std::round(yo_); }

    Rect inner_focus() const;
    Rect outer_focus() const;

  private:

    static constexpr int kWidth = 256;
    static constexpr int kHeight = 224;

    static constexpr double kHFocus = 0.167;
    static constexpr double kVFocus = 0.25;
    static constexpr double kMaxSpeed = 0.03;

    double xo_, yo_;
};
