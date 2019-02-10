#pragma once
#include "base_scene.h"

namespace scenes {
class game : public base_scene {
 public:
  game(map_size_t width, map_size_t height);
  void input(int command) override;
  void tick() override;
  void render() override;
 private:
  map map_;
  map_point hero_prev_pos_;
};
}
