#include "rt_random.hpp"

#include <random>

float rt::random::randF() {
	return std::rand() / (float)RAND_MAX;
};
