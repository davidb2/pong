#include <cmath>
#include <random>

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
    , numGames_{0}
    , randomNumberGenerator_{randomDevice_()}
    , directionDistribution_{-1, +1}
    , moveFactorDistribution_{-1., +1.}
    , uniformDistribution_{0, 1}
    {}

void TD::explore(Environment& environment) {
  while (environment.isActive()) {
    const State state = environment.getState();

    learn(state);

    const bool exploit =
        uniformDistribution_(randomNumberGenerator_) > (1. / numGames_);

    const Action action =
        exploit ? getBestAction(state) : getRandomAction(state);

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

  const Action bestAction = getBestAction(currentState);
  const int currentBallX = discretize(currentState.ballX);
  const int currentBallY = discretize(currentState.ballY);
  const int currentPaddleY = discretize(currentState.paddleY);
  const int bestDirection = 1 + static_cast<int>(bestAction.direction);
  const int bestMoveFactor = discretize(bestAction.moveFactor);

  E_[ballX][ballY][paddleY][direction][moveFactor]++;
  const int n = ++N_[ballX][ballY][paddleY][direction][moveFactor];
  const double qLast = Q_[ballX][ballY][paddleY][direction][moveFactor];
  const double qBest =
      Q_[currentBallX][currentBallY][currentPaddleY][bestDirection][bestMoveFactor];

  /**
   * avg({x_1, ..., x_{n+1}}) = (sum({x_1, ..., x_n}) + x_{n+1}) / (n+1)
   *                          = (n * avg({x_1, ..., x_n}) + x_{n+1}) / (n+1) 
   *                          = ((n * avg({x_1, ..., x_n})) / (n+1)) + (x_{n+1} / (n+1))
   *                          = avg({x_1, ..., x_n}) + (x_{n+1} - avg({x_1, ..., x_n})) / (n+1)
   *                                                   \------------------------------/
   *                                                                  |
   *                                                              "TD target"
   */
  const double tdTarget =
      static_cast<int>(lastReward_) + TD::DISCOUNT_FACTOR * qBest - qLast;

  /* Q-update and eligibility trace decays. */
  for (size_t x = 0; x < TD::PARTITIONS; x++) {
    for (size_t y = 0; y < TD::PARTITIONS; y++) {
      for (size_t py = 0; py < TD::PARTITIONS; py++) {
        for (size_t d = 0; d < TD::DIRECTIONS; d++) {
          for (size_t mf = 0; mf < TD::PARTITIONS; mf++) {
            double* q = &Q_[x][y][py][d][mf];
            double* e = &E_[x][y][py][d][mf];
            *q += ((*e) / n) * tdTarget;
            *e = TD::LAMBDA * TD::DISCOUNT_FACTOR * (*e);
          }
        }
      }
    }
  }
}

Action TD::getBestAction(const State& state) const {
  const int ballX = discretize(state.ballX);
  const int ballY = discretize(state.ballY);
  const int paddleY = discretize(state.paddleY);

  Direction bestDirection = Direction::NONE;
  double bestMoveFactor = 0.;

  const int iBestDirection = 1 + static_cast<int>(bestDirection);
  const int iBestMoveFactor = discretize(bestMoveFactor);
  double maxValue = Q_[ballX][ballY][paddleY][iBestDirection][iBestMoveFactor];

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

Action TD::getRandomAction(const State& state) {
  const Direction direction = static_cast<Direction>(
      directionDistribution_(randomNumberGenerator_)
  );
  const double moveFactor = moveFactorDistribution_(randomNumberGenerator_);
  return {direction, moveFactor};
}

void TD::terminate() {
  numGames_++;
}

int TD::discretize(const double num) const {
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
