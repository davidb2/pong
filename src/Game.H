#ifndef GAME_H_
#define GAME_H_

#include <conditional_variable>
#include <vector>

#include "Action.H"
#include "Agent.H"
#include "Reward.H"

namespace pong {

/**
 * This class holds the game functionality.
 * Note that the game starts on construction; the game is multi-threaded.
 */
class Game {
 public:
  /* Play a single-player game against a wall. */
  Game(const Agent&);

  /* Play a two-player game against each other. */
  Game(const Agent&, const Agent&);

  /* Other constructors. */
  Game(const Game&) = delete;
  Game(Game&&) = delete;

  /* Destructor. */
  virtual ~Game() = default;

  /* Operators. */
  Game& operator=(const Game&) = default;
  Game& operator=(Game&&) = default;

  /* Determines if the game is over. */
  bool isOver() const;

  /* When the game ends, `cv->notify_all()` is called. */
  void subscribeToGameOver(std::conditional_variable*);

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
  void play(const Agent&, const Agent&);
  void updatePaddle(State*);
  void updateState();
  static bool ballIsInBounds(const double x, const double y);
  static State moveBall(const State&);
  static double distance(
      const double x1, const double y1,
      const double x2, const double y2);

 private:
  bool isOver_;
  size_t numberOfBounces_;

  State state_;

  const std::vector<Agent> agents_;
  std::vector<std::conditional_variable*> gameOverConditionalVariables_;

  mutable std::mutex agentToActionMapLock_;
  mutable std::mutex stateLock_;
  mutable std::mutex isOverLock_;

  std::unordered_map<Agent, Action> agentToActionMap_;
  std::unordered_map<Agent, Reward> agentToRewardMap_;

  std::thread gameThread_;
};

} // namespace pong
#endif // GAME_H_
