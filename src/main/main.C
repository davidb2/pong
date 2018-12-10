#include <iostream>

#include "agents/Human.H"
#include "pong/Manager.H"

int main() {
  std::cout << "Playing pong ..." << std::endl;
  pong::Manager manager;
  agents::Human human;
  size_t bounces = manager.playGame(human);
  std::cout << bounces << " bounces. " << std::endl;
}
