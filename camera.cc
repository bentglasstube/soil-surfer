#include "camera.h"

double clamp(double value, double min, double max) {
  if (min > value) return min;
  if (max < value) return max;
  return value;
}

Camera::Camera(Point start) : xo_(start.x - kWidth / 2), yo_(start.y - kHeight / 2) {}

void Camera::update(const Player& p, unsigned int elapsed) {
  double max = kMaxSpeed * elapsed;

  const auto f = p.head().center();
  const Rect target = inner_focus();

  if (!outer_focus().contains(f.x, f.y)) max *= 2;

  if (f.x < target.left) {
    xo_ -= std::min(target.left - f.x, max);
  } else if (f.x > target.right) {
    xo_ += std::min(f.x - target.right, max);
  }

  if (f.y < target.top) {
    yo_ -= std::min(target.top - f.y, max);
  } else if (f.y > target.bottom) {
    yo_ += std::min(f.y - target.bottom, max);
  }

  xo_ = clamp(xo_, f.x - kWidth, f.x);
  yo_ = clamp(yo_, f.y - kHeight, f.y);
}

Rect Camera::inner_focus() const {
  return Rect(
      xo_ + kWidth * (0.5 - kHFocus),
      yo_ + kHeight * (0.5 - kVFocus),
      xo_ + kWidth * (0.5 + kHFocus),
      yo_ + kHeight * 0.5);
}

Rect Camera::outer_focus() const {
  return Rect(
      xo_ + kWidth * (0.5 - 1.5 * kHFocus),
      yo_ + kHeight * (0.5 - 1.5 * kVFocus),
      xo_ + kWidth * (0.5 + 1.5 * kHFocus),
      yo_ + kHeight * 0.75);
}

