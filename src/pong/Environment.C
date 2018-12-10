#include "Action.H"
#include "Agent.H"
#include "Environment.H"
#include "Game.H"
#include "Reward.H"

namespace pong {

Environment::Environment(Game& game, const Agent& agent)
  : game_{game}, agent_{agent} {}

State Environment::getState() const {
  return game_.getState(agent_);
}

Reward Environment::performAction(const Action action) {
  return game_.performAction(agent_, action);
}

} // namespace pong
