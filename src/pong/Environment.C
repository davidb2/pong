#include <pong/Action.H>
#include <pong/Agent.H>
#include <pong/Game.H>
#include <pong/Reward.H>

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
