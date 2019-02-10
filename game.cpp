// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "game.h"
game::game(const std::shared_ptr<scenes::base_scene> &entry_point) {
  current_scene_ = entry_point;
  terminated_ = false;
}

void game::input(int command) {
  if (current_scene_ == nullptr) {
    terminated_ = true;
    return;
  }
  current_scene_->input(command);
}
void game::tick() {
  if (current_scene_ == nullptr) {
    terminated_ = true;
    return;
  }
  current_scene_->tick();
  if (current_scene_->finished()) {
    current_scene_ = current_scene_->next_scene();
  }
}

bool game::terminated() const {
  return terminated_;
}
void game::render() {
  if (current_scene_ == nullptr) {
    terminated_ = true;
    return;
  }
  current_scene_->render();
}
