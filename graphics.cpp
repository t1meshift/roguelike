// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "graphics.h"

namespace graphics {
void init() {
  engine::init_graphics();
  atexit(engine::shutdown_graphics);
}

void render_frame() {
  engine::flush_screen();
  engine::render_frame();
}

void input(int &key_code) {
  key_code = engine::get_input();
}
void draw_state(game &state) {
  state.render();
}
}
