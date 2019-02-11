// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#define FMT_HEADER_ONLY
#include "game.h"
#include <fmt/format.h>
#include "../graphics.h"

namespace scenes {
game::game(map_size_t width, map_size_t height) {
  map_ = map(width, height);
  hero_prev_pos_ = map_.hero()->pos();
}

void game::input(int command) {
  // TODO arrow control, kbd consts
  auto &hero = map_.hero();
  switch (command) {
    case 87:
    case 119: // W
      hero->move(0, -1);
      break;
    case 65:
    case 97: // A
      hero->move(-1, 0);
      break;
    case 83:
    case 115: // S
      hero->move(0, 1);
      break;
    case 68:
    case 100: // D
      hero->move(1, 0);
      break;
    default:
      break;
  }
}
void game::tick() {
  bool level_won = false;
  auto &chars = map_.characters();
  auto &hero = map_.hero();

  // Cleaning up
  // FIXME the existence of this snippet is questionable
  for (auto it = chars.begin(); it != chars.end(); ++it) {
    if ((*it) == nullptr) {
      chars.erase(it);
    }
  }

  // Hero collisions
  for (auto &i : chars) {
    if (hero->pos() == i->pos()) {
      namespace char_vis = characters::visitors;
      char_vis::wall_visitor wall;
      hero->accept(wall, *i);
      if (wall.collided()) {
        hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
      }
      char_vis::attack_visitor attack;
      char_vis::win_cond_visitor win_cond;
      hero->accept(attack, *i);
      hero->accept(win_cond, *i);
      level_won = win_cond.won();
      hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
    }
  }
  // Move mobs then collide them
  for (int i = 0; i < chars.size(); ++i) {
    // TODO field of view of characters for characters
    auto prev_pos = chars[i]->pos();
    chars[i]->tick(hero->pos());
    // Check collisions after each tick
    for (int j = 0; j < chars.size(); ++j) {
      if (i == j) continue;
      if (chars[i]->pos() == chars[j]->pos()) {
        namespace char_vis = characters::visitors;
        char_vis::wall_visitor wall;
        chars[i]->accept(wall, *chars[j]);
        if (wall.collided()) {
          chars[i]->place(prev_pos.x, prev_pos.y);
        }
        char_vis::attack_visitor attack;
        chars[i]->accept(attack, *chars[j]);
        chars[i]->place(prev_pos.x, prev_pos.y);
      }
    }
  }

  // Check chars if mobs are dead...
  for (auto &i : chars) {
    if (i->is_dead()) {
      i.reset();
      i = nullptr;
    }
  }

  if (hero->is_dead()) {
    // TODO ЖАМЕ ЫВЕЯ
    finished_ = true;
  }

  // If the hero touches princess, he'll be full blown AIDS
  // ...ahem, no, he just goes to the next level
  if (level_won) {
    auto hero_hp = hero->hp();
    map_.regenerate(map_generators::box);
    hero->hp(hero_hp);
  }

  hero_prev_pos_ = hero->pos();
}
void game::render() {
  using namespace graphics;

  auto &m = map_;
  auto &h = m.hero();
  for (map_size_t i = 0; i < m.height(); ++i) {
    for (map_size_t j = 0; j < m.width(); ++j) {
      std::shared_ptr<characters::Character> c = m.at(j, i);
      sym_t sym = '.';
      if (c != nullptr) {
        sym = c->sym();
      }
      engine::draw_sym(sym, j, i);
    }
  }

  auto h_pos = h->pos();
  engine::draw_sym(h->sym(), h_pos.x, h_pos.y);

  auto hp_str = fmt::format("HP: {}/{}", h->hp(), h->max_hp());
  engine::write_string(hp_str, 0, engine::height()-1);
#ifdef DEBUG
  auto pos_str = fmt::format("Pos: ({};{})", h_pos.x, h_pos.y);
  engine::write_string(pos_str, 80 - pos_str.size(), engine::height()-1);
#endif
}
}