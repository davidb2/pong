#include <chrono>
#include <iostream>
#include <thread>

#include "agents/Human.H"
#include "agents/Intuitive.H"
#include "pong/Manager.H"

int main() {
  pong::Manager manager;
  agents::Intuitive intuitive;
  while (true) {
    std::cout << "Playing pong ..." << std::endl;
    size_t bounces = manager.playGame(intuitive);
    std::cout << bounces << " bounces. " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
    break;
  }
}
