// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "basic_console.h"
#warning "Please note that basic_console does not support real-time gameplay."
#warning "Also you have to use WASD control and press Return after each key press."

namespace graphics {
namespace engine {
std::vector<std::string> buff;

void flush_buff() {
  buff.assign(24, std::string(80, ' '));
}
void flush_screen() {
  for (auto i = 0; i < buff.size(); ++i) {
    std::cout << std::endl; // clear screen
  }
}
void init_graphics() {
  flush_buff();
}
void shutdown_graphics() {}
int width() {
  return 80;
}
int height() {
  return 24;
}

void draw_sym(sym_t symbol, int x, int y) {
  if (y < 0 && y >= buff.size()) return;
  if (x < 0 && x >= buff[y].size()) return;
  buff[y][x] = symbol;
}

void render_frame() {
  for (const auto &row : buff) {
    std::cout << row << std::endl;
  }
  flush_buff();
}

int get_input() {
  int c = std::cin.get();
#ifdef DEBUG
  std::cerr << c << std::endl;
#endif
  return c;
}
void write_string(const std::string &str, int x, int y) {
  for (auto c : str) {
    draw_sym(c, x++, y);
  }
}
}
}