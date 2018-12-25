#include <cmath>
#include <iostream>

#include "agents/TD.H"

namespace agents {

using pong::Action;
using pong::Direction;
using pong::Environment;
using pong::Reward;
using pong::State;

TD::TD()
    : lastState_{}
    , lastAction_{}
    , lastReward_{Reward::NONE}
    {}

void TD::explore(Environment& environment) {
  while (environment.isActive()) {
    const State state = environment.getState();

    learn(state);

    const Action action = getBestAction(state);
    const Reward reward =
        environment.performAction(action);

    lastState_ = state;
    lastAction_ = action;
    lastReward_ = reward;
  }
}

void TD::learn(const State& currentState) {
  const int ballX = discretize(lastState_.ballX);
  const int ballY = discretize(lastState_.ballY);
  const int paddleY = discretize(lastState_.paddleY);
  const int direction = 1 + static_cast<int>(lastAction_.direction);
  const int moveFactor = discretize(lastAction_.moveFactor);

  const int n = ++N_[ballX][ballY][paddleY][direction][moveFactor];
  double* qLast = &Q_[ballX][ballY][paddleY][direction][moveFactor];

  const Action bestAction = getBestAction(currentState);
  const int currentBallX = discretize(currentState.ballX);
  const int currentBallY = discretize(currentState.ballY);
  const int currentPaddleY = discretize(currentState.paddleY);
  const int bestDirection = 1 + static_cast<int>(bestAction.direction);
  const int bestMoveFactor = discretize(bestAction.moveFactor);

  const double qBest =
      Q_[currentBallX][currentBallY][currentPaddleY][bestDirection][bestMoveFactor];

  /**
   * avg({x_1, ..., x_{n+1}}) = (sum({x_1, ..., x_n}) + x_{n+1}) / (n+1)
   *                          = (n * avg({x_1, ..., x_n}) + x_{n+1}) / (n+1) 
   */
  *qLast = (1. / n) * ((n-1) * (*qLast) + (static_cast<int>(lastReward_) + TD::DISCOUNT_FACTOR * qBest));
  std::cout << "new value of " << (*qLast) << " "
            << qBest << " " << static_cast<int>(lastReward_) << std::endl;
}

Action TD::getBestAction(const State& state) {
  const int ballX = discretize(state.ballX);
  const int ballY = discretize(state.ballY);
  const int paddleY = discretize(state.paddleY);

  Direction bestDirection = Direction::NONE;
  double bestMoveFactor = 0.;

  const int iBestDirection = 1 + static_cast<int>(bestDirection);
  const int iBestMoveFactor = discretize(bestMoveFactor);
  double maxValue = Q_[ballX][ballY][paddleY][iBestDirection][iBestMoveFactor];
  std::cout << "max value " << maxValue << std::endl;

  for (int direction = -1; direction <= +1; direction++) {
    for (int moveFactor = 0;
        moveFactor < TD::PARTITIONS; moveFactor++) {

      const double value = Q_[ballX][ballY][paddleY][1 + direction][moveFactor];
      if (value > maxValue) {
        maxValue = value;
        bestDirection = static_cast<Direction>(direction);
        bestMoveFactor =
            2. * (static_cast<double>(moveFactor) / TD::PARTITIONS) - 1.;
      }
    }
  }

  return {bestDirection, bestMoveFactor};
}

void TD::terminate() {
  numGames_++;
}

int TD::discretize(const double num) {
  int discrete = static_cast<int>(std::floor(
      TD::PARTITIONS * (num + 1.) / 2.
  ));
  if (discrete < 0) {
    discrete = 0;
  } else if (discrete >= TD::PARTITIONS) {
    discrete = TD::PARTITIONS - 1;
  }
  return discrete;
}

} // namespace agents
