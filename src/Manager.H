#ifndef MANAGER_H_
#define MANAGER_H_

#include "Agent.H"
#include "Environment.H"
#include "Game.H"

namespace pong {

/**
 * This class manages/dispatches games.
 */
class Manager {
 public:
  /* Constructors. */
  Manager();
  Manager(const Manager&) = delete;
  Manager(Manager&&) = delete;

  /* Destructor. */
  virtual ~Manager();

  /* Operators. */
  Manager& operator=(const Manager&);
  Manager& operator=(Manager&&);

  /**
   * Returns the number of bounces in the game.
   * See `Game::Game(const Agent&)`.
   */
  size_t playGame(const Agent&);

  /**
   * Returns the winning agent.
   * See `Game::Game(const Agent&, const Agent&)`.
   */
  const Agent& playGame(const Agent&, const Agent&);
};

} // namespace pong

#endif // MANAGER_H_
