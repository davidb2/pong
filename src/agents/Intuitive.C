#include <iostream>

#include "agents/Intuitive.H"
#include "pong/Action.H"
#include "pong/Agent.H"
#include "pong/Game.H"
#include "pong/Environment.H"
#include "pong/Reward.H"
#include "pong/State.H"

namespace agents {

void Intuitive::explore(pong::Environment& environment) {
  while (true) {
    pong::State state = environment.getState();

    pong::Action action = pong::Action::NONE;
    if (state.paddleY - pong::PADDLE_LENGTH / 2. > state.ballY) {
      action = pong::Action::UP;
    } else if (state.paddleY + pong::PADDLE_LENGTH / 2. < state.ballY) {
      action = pong::Action::DOWN;
    }
    pong::Reward reward = environment.performAction(action);
    std::cout << static_cast<int>(reward) << std::endl;
  }
}

void Intuitive::terminate() {
}

}
