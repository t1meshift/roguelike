#pragma once
#include "../map.h"

namespace scenes {
class base_scene {
 public:
  virtual void input(int command) = 0;
  virtual void tick() = 0;
  virtual void render() = 0;

  bool finished() const;
  std::shared_ptr<base_scene> next_scene() const;
 protected:
  bool finished_ = false;
  std::shared_ptr<base_scene> next_scene_;
};
}