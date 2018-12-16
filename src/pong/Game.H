#ifndef GAME_H_
#define GAME_H_

#include <chrono>
#include <condition_variable>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Action.H"
#include "Agent.H"
#include "Reward.H"
#include "State.H"

namespace pong {

namespace constants {
  constexpr static double PADDLE_LENGTH = 0.2;
  constexpr static double PADDLE_MOVE_FACTOR = 0.05;
  const static std::chrono::milliseconds TICK =
      std::chrono::milliseconds{100};
} // namespace constants
using namespace constants;

/**
 * This class holds the game functionality.
 * Note that the game starts on construction; the game is multi-threaded.
 */
class Game {
 public:
  /* Play a single-player game against a wall. */
  Game(Agent&);

  /* Play a two-player game against each other. */
  Game(Agent&, Agent&);

  /* Other constructors. */
  Game(const Game&) = delete;
  Game(Game&&) = delete;

  /* Destructor. */
  virtual ~Game();

  /* Operators. */
  Game& operator=(const Game&) = default;
  Game& operator=(Game&&) = default;

  /* Determines if the game is over. */
  bool isOver() const;

  /* When the game ends, `cv->notify_all()` is called. */
  void subscribeToGameOver(std::condition_variable*);

  /* Returns the number of bounces in the game so far. */
  size_t numberOfBounces() const;

  /**
   * Retrieves the current state of the game.
   */
  State getState(const Agent&) const;

  /**
   * This function will block until the game has made an action.
   * The returned reward is guaranteed to be associated with the last given
   * action. For example, if `performAction` is called once from one of the
   * agent's threads and then called from another one of the agent's threads
   * before the first blocking call finished, when both functions return from
   * blocking, the rewards will be the same and will correspond to last time
   * `performAction` was called.
   */
  Reward performAction(const Agent&, const Action);

 private:
  void play(const Agent&);
  void play(Agent&, Agent&);
  bool setAction(const Agent&, const Action);
  void updatePaddle(const Agent&, State*);
  void updateState(const Agent&);
  bool determineAdjustedState(State* newState);
  static bool ballIsInBounds(const double x, const double y);
  static State moveBall(const State&, const double percentage);
  static double distance(
      const double x1, const double x2,
      const double y1, const double y2);

 private:
  bool isOver_;
  size_t numberOfBounces_;

  State state_;

  std::vector<Agent*> agents_;
  std::vector<std::condition_variable*> gameOverConditionVariables_;

  mutable std::mutex agentToActionMapLock_;
  mutable std::mutex stateLock_;
  mutable std::mutex isOverLock_;
  mutable std::mutex tickCvLock_;

  std::condition_variable tickConditionVariable_;

  std::unordered_map<const Agent*, Action> agentToActionMap_;
  std::unordered_map<const Agent*, Reward> agentToRewardMap_;

  std::thread gameThread_;
};

} // namespace pong
#endif // GAME_H_
