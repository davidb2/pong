#ifndef INTUITIVE_H_
#define INTUITIVE_H_

#include "pong/Agent.H"
#include "pong/Environment.H"

namespace agents {

class Intuitive : public pong::Agent {
  void explore(pong::Environment&) override;
  void terminate() override;
};

} // namespace agents

#endif // INTUITIVE_H_
