#include "geometry.h"

Direction Direction::opposite() const {
  switch (value) {
    case Direction::NW: return Direction::SE;
    case Direction::NE: return Direction::SW;
    case Direction::E:  return Direction::W;
    case Direction::SE: return Direction::NW;
    case Direction::SW: return Direction::NE;
    case Direction::W:  return Direction::E;
  }

  return value;
}

int Direction::angle(Value other) const {
  const int diff = std::abs(value - other);
  return 60 * (diff > 3 ? 6 - diff : diff);
}

GridPoint Point::to_grid() const {
  const double px = x / GridPoint::kTileSize;
  const double py = y / GridPoint::kTileSize;
  return GridPoint(std::round(px / 2.0 - py / 3.0), std::round(py * 2 / 3.0));
}

Point GridPoint::center() const {
  return Point(q() * 2 * kTileSize + r() * kTileSize, r() * 3 * kTileSize / 2);
}

Point GridPoint::draw_point() const {
  return center() - Point(kTileSize, kTileSize);
}

GridPoint GridPoint::apply(Direction d) const {
  switch (d) {
    case Direction::NW: return GridPoint(q(), r() - 1);
    case Direction::NE: return GridPoint(q() + 1, r() - 1);
    case Direction::E:  return GridPoint(q() + 1, r());
    case Direction::SE: return GridPoint(q(), r() + 1);
    case Direction::SW: return GridPoint(q() - 1, r() + 1);
    case Direction::W:  return GridPoint(q() - 1, r());
  }

  return GridPoint(q(), r());
}

long GridPoint::dist(const GridPoint& other) const {
  return (std::abs(other.q() - q()) + std::abs(other.r() - r()) + std::abs(other.s() - s())) / 2;
}
