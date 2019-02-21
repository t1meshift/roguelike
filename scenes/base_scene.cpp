// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "base_scene.h"
namespace scenes {
bool base_scene::finished() const {
  return finished_;
}
std::shared_ptr<base_scene> base_scene::next_scene() const {
  return next_scene_;
}
}