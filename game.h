#pragma once
#include "scenes/base_scene.h"

class game {
 public:
  explicit game(const std::shared_ptr<scenes::base_scene> &entry_point);
  void input(int command);
  void tick();
  void render();
  bool terminated() const;
 private:
  std::shared_ptr<scenes::base_scene> current_scene_;
  bool terminated_;
};