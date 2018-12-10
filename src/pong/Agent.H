#ifndef AGENT_H_
#define AGENT_H_

namespace pong {

/* Forward Declaration. */
class Environment;

/**
 * Abstract class for an agent.
 */
class Agent {
 public:
  /**
   * This is the main function for exploring the environment.
   * The agent should not exit this function.
   */
  virtual void explore(const Environment&) = 0;

  /**
   * This function is called after the game has ended. Due to the semantics
   * of the `explore` function, after this function has ended, the reference
   * to the `Environment` in `explore` should not be trusted.
   * This function will be called from a different thread than the `explore`
   * function is running on.
   */
  virtual void terminate() = 0;
};

} // namespace pong

#endif // AGENT_H_
