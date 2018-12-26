#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "agents/Intuitive.H"
#include "pong/Action.H"
#include "pong/Agent.H"
#include "pong/Game.H"
#include "pong/Environment.H"
#include "pong/Reward.H"
#include "pong/State.H"

namespace agents {

using pong::Action;
using pong::Direction;
using pong::Environment;
using pong::Reward;
using pong::State;

void Intuitive::explore(Environment& environment) {
  while (environment.isActive()) {
    State state = environment.getState();

    Direction direction = Direction::NONE;
    if (state.paddleY > state.ballY) {
      direction = Direction::UP;
    } else if (state.paddleY < state.ballY) {
      direction = Direction::DOWN;
    }

    const double moveFactor = std::abs((state.paddleY - state.ballY) / 2.);

    Reward reward =
        environment.performAction({direction, moveFactor});

    std::cout << static_cast<int>(reward) << std::endl;
  }
}

void Intuitive::terminate() {
}

}
