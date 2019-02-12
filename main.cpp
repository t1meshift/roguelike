// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#define CONFIGURU_IMPLEMENTATION 1
#include <chrono>
#include <thread>
#include <configuru.hpp>
#include "config.h"
#include "graphics.h"
#include "graphics/kbd_keys.h"
#include "scenes/game.h"

using namespace std::literals::chrono_literals;

int main() {
  Config::init();
  graphics::init();
  auto init_scene = std::make_shared<scenes::game>(scenes::game(120, 120));
  game state(init_scene);
  int key_code = Key::NOTHING;

  while (!state.terminated()) {
    graphics::clear();
    graphics::draw_state(state);
    graphics::render_frame();
    graphics::input(key_code);
    if (key_code != Key::NOTHING) {
      state.input(key_code);
    }
    state.tick();
    key_code = Key::NOTHING;
  }

  return 0;
}