#include "../graphics_include_protector.h"
#include <string>
#include <vector>
#include <iostream>
#include "../../characters/types.h"
#include "../kbd_keys.h"

namespace graphics {
namespace engine {

void flush_screen();
void init_graphics();
void shutdown_graphics();
int width();
int height();
void draw_sym(sym_t symbol, int x, int y);
void write_string(const std::string &str, int x, int y);
void render_frame();

int get_input();
}
}

