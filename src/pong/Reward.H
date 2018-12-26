#ifndef REWARD_H_
#define REWARD_H_

namespace pong {

/**
 * Represents the reward for a particular action.
 */
enum class Reward {
  BAD = -1,
  NONE = 0,
  GOOD = +1,
};

} // namespace pong

#endif // REWARD_H_
