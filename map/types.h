#pragma once
#include <cstdint>
#include <vector>
#include <functional>

typedef int16_t map_size_t;

struct map_point {
  map_size_t x, y;
  friend bool operator==(const map_point &a, const map_point &b);
};