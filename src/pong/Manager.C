#include <conditional_variable>
#include <memory>
#include <mutex>

#include <pong/Agent.H>
#include <pong/Environment.H>
#include <pong/Game.H>
#include <pong/Manager.H>

namespace pong {

size_t playGame(const Agent& agent) {
  /* The game has started. */
  Game game(agent);

  std::conditional_variable gameOverConditionalVariable;
  /**
   * TODO(ljeabmreosn): Add this to GameOptions since this is a race condition.
   */
  game.subscribeToGameOver(&gameOverConditionalVariable);

  const Environment environment(game, agent);

  /* Let the agent explore. */
  const std::thread agentThread(&agent::explore, std::ref(environment));

  /* Wait until the game is over. */
  std::mutex gameOverMutex;
  std::unique_lock<std::mutex> gameOverLock(gameOverMutex);
  gameOverConditionalVariable.wait(gameOverLock, &game::isOver);
  /* The game is now over. Don't attempt to join the agent thread. */

  return game.numberOfBounces();
}

} // namespace pong
