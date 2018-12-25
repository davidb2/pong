#ifndef MONTECARLO_H_
#define MONTECARLO_H_

#include <random>
#include <vector>

#include "pong/Action.H"
#include "pong/Agent.H"
#include "pong/Environment.H"
#include "pong/Reward.H"
#include "pong/State.H"

namespace agents {

class TD : public pong::Agent {
 public:
  TD();

  void explore(pong::Environment&) override;
  void terminate() override;

 private:
  void learn(const pong::State& currentState);
  pong::Action getBestAction(const pong::State&);
  int discretize(const double);

 private:
  static constexpr int PARTITIONS = 5;
  static constexpr int DIRECTIONS = 3;
  static constexpr double DISCOUNT_FACTOR = 0.9;

  /* [ballX][ballY][paddleY][direction][moveFactor] */
  double Q_[PARTITIONS][PARTITIONS][PARTITIONS][DIRECTIONS][PARTITIONS] = {};
  double N_[PARTITIONS][PARTITIONS][PARTITIONS][DIRECTIONS][PARTITIONS] = {};

  pong::State lastState_;
  pong::Action lastAction_;
  pong::Reward lastReward_;

  int numGames_ = 0;
};

} // namespace agents

#endif // MONTECARLO_H_
