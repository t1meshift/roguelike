#include "graphics/ncurses/ncurses.h"
#include "game.h"

namespace graphics {
  void init();

  int width();
  int height();
  void draw_state(game &state);
  void clear();
  void render_frame();

  void input(int &key_code);
}
