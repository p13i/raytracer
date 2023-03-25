#include "rt_random.hpp"

#include <random>

float rt::random::randF() {
  return std::rand() / static_cast<float>(RAND_MAX);
};