#include "graphics/basic_console.h"
#include "game.h"

namespace graphics {
  void init();
  void draw_state(game &state);
  void render_frame();

  void input(int &key_code);
}
