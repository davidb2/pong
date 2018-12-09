#ifndef STATE_H_
#define STATE_H_

namespace pong {

/**
 * Represents the state of the agent.
 */
struct State {
  /* The position of the ball. */
  double ballX, ballY;

  /* The velocity of the ball. */
  double ballDx, ballDy;

  /* The y-coordinate of the agent's paddle. */
  double paddleY;
};

} // namespace pong

#endif STATE_H_
