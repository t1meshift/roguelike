#pragma once
#include "base_scene.h"

namespace scenes {
class game : public base_scene {
 public:
  game(map_size_t width, map_size_t height);
  void input(int command) override;
  void tick() override;
  void render() override;

  map &current_map();
  map_point offset() const;
  std::vector<std::shared_ptr<characters::Projectile>> &hero_projectiles();
 private:
  map map_;
  map_point hero_prev_pos_;
  map_size_t offset_x_, offset_y_;
  bool key_pressed_;
  std::vector<std::shared_ptr<characters::Projectile>> hero_projs_;

  void calc_offsets();
};
}
