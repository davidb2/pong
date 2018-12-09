#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Action.H"
#include "Agent.H"
#include "Game.H"
#include "Reward.H"

namespace pong {

/**
 * This class represents an interface between game and the agent.
 * Due to the nature/semantics of this class, all members are stored by
 * reference because an `Environment` can only exist with the original game
 * and the original agent. This interface is needed because an agent should
 * not be able act as another agent when playing. So an agent is given an
 * environment corresponding itself to inspect and upon which to perform
 * actions.
 *
 * Game <---> Environment <---> Agent
 */
class Environment {
 public:
  /* Constructors. */
  Environment(Game&, const Agent&);
  Environment(const Environment&) = delete;
  Environment(Environment&&) = delete;

  /* Destructor. */
  virtual ~Environment();

  /* Operators. */
  Environment& operator=(const Environment&);
  Environment& operator=(Environment&&);

  /* See `Game::getState(const Agent&)`. */
  State getState() const;

  /* See `Game::performAction(const Agent&, const Action)`. */
  Reward performAction(const Action);

 private:
  Game& game_;
  const Agent& agent_;
};

} // namespace pong

#endif // ENVIRONMENT_H_
