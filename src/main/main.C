#include <chrono>
#include <iostream>
#include <thread>

#include "agents/Human.H"
#include "agents/Intuitive.H"
#include "pong/Manager.H"

int main() {
  while (true) {
    std::cout << "Playing pong ..." << std::endl;
    pong::Manager manager;
    agents::Intuitive intuitive;
    size_t bounces = manager.playGame(intuitive);
    std::cout << bounces << " bounces. " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds{1});
  }
}
