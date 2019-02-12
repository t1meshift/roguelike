// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "ncurses.h"
#include <ncurses.h>

KEY_(NOTHING, ERR);
KEY_(ARROW_UP, KEY_UP);
KEY_(ARROW_DOWN, KEY_DOWN);
KEY_(ARROW_LEFT, KEY_LEFT);
KEY_(ARROW_RIGHT, KEY_RIGHT);

namespace graphics {
namespace engine {
int w_, h_;
void flush_screen() {
  clear();
}
void init_graphics() {
  initscr();
  getmaxyx(stdscr, h_, w_);
  noecho();
  curs_set(0);
  keypad(stdscr, true);
#ifndef NO_REAL_TIME
  timeout(200);
#endif
}
void shutdown_graphics() {
  curs_set(1);
  endwin();
  refresh();
}
int width() {
  return w_;
}
int height() {
  return h_;
}

void draw_sym(sym_t symbol, int x, int y) {
  if (y < 0 || y >= h_) return;
  if (x < 0 || x >= w_) return;
  mvaddch(y, x, symbol);
}

void render_frame() {
  refresh();
}

int get_input() {
  int c = getch();
  if (c == KEY_RESIZE) {
    getmaxyx(stdscr, h_, w_);
    return Key::NOTHING;
  }
  return c;
}
void write_string(const std::string &str, int x, int y) {
  mvaddstr(y, x, str.c_str());
}
}
}