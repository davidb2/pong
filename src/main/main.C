#include <chrono>
#include <iostream>
#include <thread>

#include "agents/TD.H"
#include "pong/Manager.H"

int main() {
  pong::Manager manager;
  agents::TD agent;
  int i = 0;
  while (true) {
    std::cout << "Playing pong ..." << std::endl;
    size_t bounces = manager.playGame(agent);
    std::cout << bounces << " bounces. (" << i++ << ")" << std::endl;
  }
}
