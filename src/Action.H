#ifndef ACTION_H_
#define ACTION_H_

namespace pong {

/**
 * Represents the actions an agent can make.
 * In the future, the actions could be more fine grained.
 */
enum class Action {
  NONE = 0,
  UP,
  DOWN,
};

} // namespace pong

#endif // ACTION_H_
