#include <iostream>

#include "agents/Human.H"
#include "pong/Environment.H"

namespace agents {

void Human::explore(pong::Environment& environment) {
  double lastPaddleY = 0;
  pong::Action lastAction = pong::Action::UP;
  while (true) {
    pong::State state = environment.getState();

    pong::Action action = pong::Action::NONE;
    if (state.paddleY == lastPaddleY) {
      if (state.paddleY <= 0) {
        action = pong::Action::DOWN;
      } else if (state.paddleY > 0) {
        action = pong::Action::UP;
      }
    } else {
      action = lastAction;
    }
    lastAction = action;
    lastPaddleY = state.paddleY;
    pong::Reward reward = environment.performAction(action);
  }
}

void Human::terminate() {
}

} // namespace agents
