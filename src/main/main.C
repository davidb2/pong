#include <chrono>
#include <iostream>
#include <thread>

#include "agents/Human.H"
#include "agents/Intuitive.H"
#include "agents/MonteCarlo.H"
#include "pong/Manager.H"

int main() {
  pong::Manager manager;
  agents::MonteCarlo monteCarlo;
  // agents::Human monteCarlo;
  int i = 0;
  while (true) {
    std::cout << "Playing pong ..." << std::endl;
    size_t bounces = manager.playGame(monteCarlo);
    std::cout << bounces << " bounces. (" << i++ << ")" << std::endl;
  }
}
