#include <chrono>
#include <cmath>
#include <condition_variable>
#include <iostream>
#include <random>
#include <thread>
#include <unordered_map>

#include "Action.H"
#include "Agent.H"
#include "Game.H"
#include "Reward.H"
#include "State.H"

namespace pong {

Game::Game(Agent& agent)
    : isOver_{false}
    , numberOfBounces_{0u} {
  agents_.push_back(&agent);

  std::random_device randomDevice;
  std::mt19937 randomNumberGenerator(randomDevice());
  std::uniform_real_distribution<double> distributionX(-1.0, +1.0);
  std::uniform_real_distribution<double> distributionY(-0.5, +0.5);

  /* Initial conditions. Note that |dx| >= |dy| for the initial velocity. */
  state_.paddleY = 0.0;
  state_.ballX = 0.0;
  state_.ballY = 0.0;
  do {
    state_.ballDx = distributionX(randomNumberGenerator);
    state_.ballDy = distributionY(randomNumberGenerator);
    state_.ballDx +=
        ((state_.ballDx > 0) - (state_.ballDx < 0)) * std::abs(state_.ballDy);
  } while (std::abs(state_.ballDy) < 1e-6 || std::abs(state_.ballDx) < 1e-6);
  /* End initial conditions. */

  void (Game::*playFn)(const Agent&) = &Game::play;
  gameThread_ = std::thread(playFn, std::ref(agent));
}

void Game::play(const Agent& agent) {
  while (true) {
    std::this_thread::sleep_for(TICK);

    {
      std::lock_guard<std::mutex> stateGuard(stateLock_);
      updateState(agent);
      std::cout << "x: " << state_.ballX << ", "
                << "y: " << state_.ballY << std::endl;

      /* Check if the game is over. */
      {
        std::lock_guard<std::mutex> isOverGuard(isOverLock_);
        if (isOver_) {
          for (auto*& cv : gameOverConditionVariables_) {
            cv->notify_all();
          }
          break;
        }
      }
    }
    tickConditionVariable_.notify_all();
  }

  std::cout << "Game is over. " << std::endl;
}

void Game::updateState(const Agent& agent) {
  State newState;
  updatePaddle(agent, &newState);

  double percentage = 1;
  newState = moveBall(state_, percentage);
  const double originalDistance = distance(
      state_.ballX, newState.ballX,
      state_.ballY, newState.ballY
  );

  while (!ballIsInBounds(newState.ballX, newState.ballY)) {
    /* Old position. */
    const double ox = state_.ballX;
    const double oy = state_.ballY;
    /* New position. */
    // const double nx = newState.ballX;
    // const double ny = newState.ballY;
    /* Velocities. */
    const double dx = newState.ballDx;
    const double dy = newState.ballDy;
    /* Adjusted positions/percentage. */
    double ax, ay, ap;

    /* This is not a loop; it is only used for scoping purposes. */
    do {
      /* Check if ball hit bottom wall. */
      ay = -1;
      ax =  -(dx / dy) * (oy - ay) + ox;
      ap = (ax - ox) / dx;
      if ((-1 <= ax && ax <= +1) && (0 <= ap && ap <= +1)) {
        newState.ballDy *= -1;
        break;
      }

      /* Check if ball hit top wall. */
      ay = +1;
      ax =  -(dx / dy) * (oy - ay) + ox;
      ap = (ax - ox) / dx;
      if ((-1 <= ax && ax <= +1) && (0 <= ap && ap <= +1)) {
        newState.ballDy *= -1;
        break;
      }

      /* Check if ball hit left wall. */
      ax = -1;
      ay =  -(dy / dx) * (ox - ax) + oy;
      ap = (ay - oy) / dy;
      if ((-1 <= ay && ay <= +1) && (0 <= ap && ap <= +1)) {
        newState.ballDx *= -1;
        break;
      }

      /* Check if ball hit right wall. */
      ax = +1;
      ay =  -(dy / dx) * (ox - ax) + oy;
      ap = (ay - oy) / dy;
      if ((-1 <= ay && ay <= +1) && (0 <= ap && ap <= +1)) {
        if (ay < newState.paddleY - PADDLE_LENGTH / 2.0
         || ay > newState.paddleY + PADDLE_LENGTH / 2.0) {
          /* Game is over. */
          std::lock_guard<std::mutex> isOverGuard(isOverLock_);
          isOver_ = true;
        }
        newState.ballDx *= -1;
        break;
      }
    } while (false);

    const double distanceTraveled = distance(
        state_.ballX, ax,
        state_.ballY, ay
    );
    percentage -= distanceTraveled / originalDistance;

    newState.ballX = ax;
    newState.ballY = ay;
    state_ = newState;
    newState = moveBall(state_, percentage);
  }
  state_ = newState;
}

void Game::updatePaddle(const Agent& agent, State* newState) {
  std::lock_guard<std::mutex> guard(agentToActionMapLock_);
  newState->paddleY = state_.paddleY + 
      PADDLE_MOVE_FACTOR * static_cast<int>(agentToActionMap_.at(&agent));

  if (newState->paddleY + PADDLE_LENGTH / 2.0 >= +1) {
    newState->paddleY = 1 - PADDLE_LENGTH / 2.0;
  }
  if (newState->paddleY - PADDLE_LENGTH / 2.0 <= -1) {
    newState->paddleY = -1 + PADDLE_LENGTH / 2.0;
  }
}

double Game::distance(
    const double x1, const double y1,
    const double x2, const double y2) {
  return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

State Game::moveBall(const State& oldState, const double percentage) {
  State newState;
  newState.paddleY = oldState.paddleY;
  newState.ballX = oldState.ballX + percentage * oldState.ballDx;
  newState.ballY = oldState.ballY + percentage * oldState.ballDy;
  newState.ballDx = oldState.ballDx;
  newState.ballDy = oldState.ballDy;
  return newState;
}

bool Game::ballIsInBounds(const double x, const double y) {
  return (-1 < x && x < +1) && (-1 < y && y < +1);
}

bool Game::isOver() const {
  return isOver_;
}

void Game::subscribeToGameOver(
    std::condition_variable* gameOverConditionVariable) {
  gameOverConditionVariables_.push_back(gameOverConditionVariable);
}

size_t Game::numberOfBounces() const {
  return numberOfBounces_;
}

State Game::getState(const Agent& agent) const {
  return state_;
}

Reward Game::performAction(const Agent& agent, const Action action) {
  if (!setAction(agent, action)) {
    return Reward::NONE;
  }
  std::unique_lock<std::mutex> tickCvGuard(tickCvLock_);
  tickConditionVariable_.wait(tickCvGuard);
  return agentToRewardMap_.at(&agent);
}

bool Game::setAction(const Agent& agent, const Action action) {
  std::lock_guard<std::mutex> guard(agentToActionMapLock_);
  const auto& search = agentToActionMap_.find(&agent);
  /* This should never happen if the logic in Manager.C is correct. */
  if (search == agentToActionMap_.end()) {
    std::cerr << "Invalid agent trying to perform action: "
              << std::addressof(agent) << "." << std::endl;
    return false;
  }
  agentToActionMap_[&agent] = action;
  return true;
}

} // namespace pong
