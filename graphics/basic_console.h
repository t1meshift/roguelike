#define NO_REAL_TIME
#include "internal/graphics_include_protector.h"
#include <string>
#include <vector>
#include <iostream>
#include "../characters/types.h"

namespace graphics {
namespace engine {

void flush_buff();
void flush_screen();
void init_graphics();
void shutdown_graphics();
void draw_sym(sym_t symbol, int x, int y);
void write_string(const std::string &str, int x, int y);
void render_frame();

int get_input();
}
}