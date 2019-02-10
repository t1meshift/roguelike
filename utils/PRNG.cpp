// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "PRNG.h"
#include <chrono>


PRNG::PRNG() {
  auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  gen_.seed(seed);
}

PRNG::PRNG(long seed) {
  gen_.seed(seed);
}


