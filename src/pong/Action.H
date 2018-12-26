#ifndef ACTION_H_
#define ACTION_H_

namespace pong {

enum class Direction : int {
  UP = -1,
  NONE,
  DOWN,
};

/**
 * Represents the actions an agent can make.
 * In the future, the actions could be more fine grained.
 */
struct Action {
  Action() : direction{Direction::NONE}, moveFactor{0} {}
  Action(const Direction direction, const double moveFactor)
      : direction{direction}, moveFactor{moveFactor} {}
  Direction direction;

  Action(const Action&) = default;
  Action& operator=(const Action&) = default;
  /**
   * A number between 0 and 1. It is undefined behavior to specify a number
   * less than 0 or greater than 1.
   */
  double moveFactor;
};


} // namespace pong

#endif // ACTION_H_
