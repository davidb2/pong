#include <cmath>
#include <iostream>
#include <random>

#include "agents/MonteCarlo.H"

namespace agents {

using pong::Action;
using pong::Direction;
using pong::Environment;
using pong::Reward;
using pong::State;

MonteCarlo::MonteCarlo()
  : randomNumberGenerator_{randomDevice_()}
  , directionDistribution_{-1, +1}
  , moveFactorDistribution_{-1., +1.} {
}

void MonteCarlo::explore(Environment& environment) {
  if (numGames_ % 50 >= 40) {
    play(environment);
  } else {
    practice(environment);
  }
}

void MonteCarlo::practice(Environment& environment) {
  while (environment.isActive()) {
    const State state = environment.getState();
    const Direction direction = static_cast<Direction>(
        directionDistribution_(randomNumberGenerator_)
    );
    const double moveFactor = moveFactorDistribution_(randomNumberGenerator_);
    const Action action{direction, moveFactor};
    const Reward reward = environment.performAction(action);
    
    states_.push_back(state);
    actions_.push_back(action);
    rewards_.push_back(reward);
  }
}

void MonteCarlo::play(Environment& environment) {
  while (environment.isActive()) {
    const State state = environment.getState();
    const int ballX = discretize(state.ballX);
    const int ballY = discretize(state.ballY);
    const int paddleY = discretize(state.paddleY);

    double maxValue = NAN;
    Direction bestDirection = Direction::NONE;
    double bestMoveFactor = 0.;
    std::cout << "values: ";
    for (int direction = -1; direction <= +1; direction++) {
      for (int moveFactor = 0;
          moveFactor < MonteCarlo::PARTITIONS; moveFactor++) {

        const double value = V_[ballX][ballY][paddleY][1 + direction][moveFactor];
        std::cout << value << " ";
        if (std::isnan(maxValue) || value > maxValue) {
          maxValue = value;
          bestDirection = static_cast<Direction>(direction);
          bestMoveFactor =
              2. * (static_cast<double>(moveFactor) / MonteCarlo::PARTITIONS) - 1.;
          std::cout << "(got " << direction << ", " << bestMoveFactor << ") ";
        }
      }
    }

    std::cout << static_cast<int>(bestDirection) << " "
              << bestMoveFactor << std::endl;

    const Reward reward =
        environment.performAction({bestDirection, bestMoveFactor});
  }
}

void MonteCarlo::terminate() {
  double accumulatedReward = 0.;
  for (int i = states_.size() - 1; i >= 0; i--) {
    const int ballX = discretize(states_[i].ballX);
    const int ballY = discretize(states_[i].ballY);
    const int paddleY = discretize(states_[i].paddleY);
    const int direction = 1 + static_cast<int>(actions_[i].direction);
    const int moveFactor = discretize(actions_[i].moveFactor);

    const Reward reward = rewards_[i];
    accumulatedReward = static_cast<int>(reward)
        + MonteCarlo::DISCOUNT_FACTOR * accumulatedReward;

    std::cout << "reward " << static_cast<int>(reward) << std::endl;

    const int n = ++N_[ballX][ballY][paddleY][direction][moveFactor];
    double* v = &V_[ballX][ballY][paddleY][direction][moveFactor];
    /**
     * avg({x_1, ..., x_{n+1}}) = (sum({x_1, ..., x_n}) + x_{n+1}) / (n+1)
     *                          = (n * avg({x_1, ..., x_n}) + x_{n+1}) / (n+1) 
     */
    *v = (1. / n) * ((n-1) * (*v) + accumulatedReward);
    std::cout << "new value of " << (*v) << " "
              << accumulatedReward << std::endl;
  }

  states_.clear();
  actions_.clear();
  rewards_.clear();

  numGames_++;
}

int MonteCarlo::discretize(const double num) {
  int discrete = static_cast<int>(std::floor(
      MonteCarlo::PARTITIONS * (num + 1.) / 2.
  ));
  if (discrete < 0) {
    discrete = 0;
  } else if (discrete >= MonteCarlo::PARTITIONS) {
    discrete = MonteCarlo::PARTITIONS - 1;
  }
  return discrete;
}

} // namespace agents
