#include <iostream>

#include "agents/Human.H"
#include "pong/Environment.H"

namespace agents {

using pong::Action;
using pong::Direction;
using pong::Environment;
using pong::Reward;
using pong::State;

void Human::explore(Environment& environment) {
  double lastPaddleY = 0;
  Direction lastDirection = Direction::UP;
  while (true) {
    State state = environment.getState();

    Direction direction = Direction::NONE;
    if (state.paddleY == lastPaddleY) {
      if (state.paddleY <= 0) {
        direction = Direction::DOWN;
      } else if (state.paddleY > 0) {
        direction = Direction::UP;
      }
    } else {
      direction = lastDirection;
    }
    lastDirection = direction;
    lastPaddleY = state.paddleY;

    Reward reward =
        environment.performAction({direction, 1});
  }
}

void Human::terminate() {
}

} // namespace agents
