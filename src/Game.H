#ifndef GAME_H_
#define GAME_H_

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
  virtual ~Game();

  /* Operators. */
  Game& operator=(const Game&);
  Game& operator=(Game&&);

  /* Determines if the game is over. */
  bool isOver() const;

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
  /* Stores the players of the game. */
  const std::vector<Agent> agents_;
};

} // namespace pong
#endif // GAME_H_
