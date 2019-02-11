// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#define CONFIGURU_IMPLEMENTATION 1
#include <chrono>
#include <thread>
#include <configuru.hpp>
#include "config.h"
#include "graphics.h"
#include "scenes/game.h"

using namespace std::literals::chrono_literals;

int main() {
  Config::init();
  graphics::init();
  auto init_scene = std::make_shared<scenes::game>(scenes::game(120, 120));
  game state(init_scene);
  int key_code;
#ifndef NO_REAL_TIME
  auto tick_time = 1s / 15; // 15 FPS
#endif

  while (!state.terminated()) {
    graphics::draw_state(state);
    graphics::render_frame();
    graphics::input(key_code);
    if (key_code != 0) {
      state.input(key_code);
    }
    state.tick();
    key_code = 0;
#ifndef NO_REAL_TIME
    std::this_thread::sleep_for(tick_time);
#endif
  }
  return 0;
}