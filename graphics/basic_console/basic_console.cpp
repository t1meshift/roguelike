// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "basic_console.h"
#warning "Please note that basic_console does not support real-time gameplay."
#warning "Also you have to use WASD control and press Return after each key press."

KEY_(NOTHING, -1);
KEY_(ARROW_UP, 119);
KEY_(ARROW_DOWN, 115);
KEY_(ARROW_LEFT, 97);
KEY_(ARROW_RIGHT, 100);
KEY_(THROW_UP, 'i');
KEY_(THROW_DOWN, 'k');
KEY_(THROW_LEFT, 'j');
KEY_(THROW_RIGHT, 'l');

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
  return buff[0].size();
}
int height() {
  return buff.size();
}

void draw_sym(sym_t symbol, int x, int y) {
  if (y < 0 || y >= buff.size()) return;
  if (x < 0 || x >= buff[y].size()) return;
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
  return c;
}
void write_string(const std::string &str, int x, int y) {
  for (auto c : str) {
    draw_sym(c, x++, y);
  }
}
}
}