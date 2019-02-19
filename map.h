#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "utils/PRNG.h"
#include "map/types.h"
#include "characters.h"

typedef std::vector<std::shared_ptr<characters::Character>> chars_container;
typedef struct {
  std::shared_ptr<characters::Character> hero;
  chars_container characters;
} map_container;
typedef std::function<map_container(map_size_t, map_size_t)> map_generator;

namespace map_generators {
map_container box(map_size_t w, map_size_t h);
map_container snake(map_size_t w, map_size_t h);
}

class map {
 public:
  map(map_size_t width, map_size_t height, map_generator gen = map_generators::snake);

  map_size_t width() const;
  map_size_t height() const;

  std::shared_ptr<characters::Character> &hero();
  chars_container &characters();

  void regenerate(map_generator gen);
 private:
  map_size_t width_;
  map_size_t height_;
  std::shared_ptr<characters::Character> hero_;
  chars_container characters_;
};