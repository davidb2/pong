#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

#include "Agent.H"
#include "Environment.H"
#include "Game.H"
#include "Manager.H"

namespace pong {

size_t Manager::playGame(Agent& agent) {
  /* The game has started. */
  Game game{agent};

  std::condition_variable gameOverConditionVariable;
  /**
   * TODO(ljeabmreosn): Add this to GameOptions since this is a race condition.
   */
  game.subscribeToGameOver(&gameOverConditionVariable);

  Environment environment{game, agent};

  /* Let the agent explore. */
  std::thread agentThread([&agent, &environment] {
    agent.explore(environment);

    /* Let the agent store any relevant information. This may hang forever. */
    agent.terminate();
  });

  /* Wait until the game is over. */
  {
    std::mutex gameOverMutex;
    std::unique_lock<std::mutex> gameOverLock(gameOverMutex);
    gameOverConditionVariable.wait(gameOverLock, [&game] {
        return game.isOver();
    });
  }

  /* The game is now over; join the agent thread. */
  agentThread.join();

  return game.numberOfBounces();
}

} // namespace pong
