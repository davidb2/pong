#ifndef HUMAN_H_
#define HUMAN_H_

#include "pong/Agent.H"
#include "pong/Environment.H"

namespace agents {

class Human : public pong::Agent {
  void explore(const pong::Environment&) override;
  void terminate() override;
};

} // namespace agents

#endif // HUMAN_H_
