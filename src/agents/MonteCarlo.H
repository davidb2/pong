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

class MonteCarlo : public pong::Agent {
 public:
  MonteCarlo();

  void explore(pong::Environment&) override;
  void terminate() override;

 private:
  void play(pong::Environment&);
  void practice(pong::Environment&);
  int discretize(const double);

 private:
  static constexpr int PARTITIONS = 20;
  static constexpr int DIRECTIONS = 3;

  /* [ballX][ballY][paddleY][direction][moveFactor] */
  double V_[PARTITIONS][PARTITIONS][PARTITIONS][DIRECTIONS][PARTITIONS] = {};
  double N_[PARTITIONS][PARTITIONS][PARTITIONS][DIRECTIONS][PARTITIONS] = {};

  std::vector<pong::State> states_;
  std::vector<pong::Action> actions_;
  std::vector<pong::Reward> rewards_;

  int numGames_ = 0;

  std::random_device randomDevice_;
  std::mt19937 randomNumberGenerator_;
  std::uniform_int_distribution<int> directionDistribution_;
  std::uniform_real_distribution<double> moveFactorDistribution_;
};

} // namespace agents

#endif // MONTECARLO_H_
