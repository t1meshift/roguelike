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
  offset_x_ = map_.hero()->pos().x - graphics::width() / 2;
  offset_y_ = map_.hero()->pos().y - graphics::height() / 2;
  calc_offsets();
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
  auto hpos = hero->pos();
  if (hpos.x < 0 || hpos.x >= map_.width() ||
    hpos.y < 0 || hpos.y >= map_.height()) {
    hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
  }
}
void game::tick() {
  bool level_won = false;
  auto &chars = map_.characters();
  auto &hero = map_.hero();

  namespace char_vis = characters::visitors;

  // Hero collisions
  for (auto &i : chars) {
    if (hero->pos() == i->pos()) {
      char_vis::wall_visitor wall;
      hero->accept(wall, *i);
      if (wall.collided()) {
        hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
      }
      char_vis::attack_visitor attack;
      char_vis::win_cond_visitor win_cond;
      hero->accept(attack, *i);
      hero->accept(win_cond, *i);
      if (!level_won) {
        level_won = win_cond.won();
      }
      if (!i->is_dead()) {
        hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
      }
    }
  }
  // Move mobs then collide them
  for (int i = 0; i < chars.size(); ++i) {
    // TODO field of view of characters for characters
    auto prev_pos = chars[i]->pos();
    chars[i]->tick(hero->pos());
    auto cpos = chars[i]->pos();
    if (cpos.x < 0 || cpos.x >= map_.width() ||
        cpos.y < 0 || cpos.y >= map_.height()) {
      chars[i]->place(hero_prev_pos_.x, hero_prev_pos_.y);
    }
    if (cpos == prev_pos) continue;
    // Check collisions after each tick
    if (chars[i]->pos() == hero->pos()) {
      char_vis::attack_visitor attack;
      chars[i]->accept(attack, *hero);
      if (!hero->is_dead()) {
        chars[i]->place(prev_pos.x, prev_pos.y);
      }
    }
    for (int j = 0; j < chars.size(); ++j) {
      if (i == j) continue;
      if (chars[i]->pos() == chars[j]->pos()) {
        char_vis::wall_visitor wall;
        chars[i]->accept(wall, *chars[j]);
        if (wall.collided()) {
          chars[i]->place(prev_pos.x, prev_pos.y);
        }
        char_vis::attack_visitor attack;
        chars[i]->accept(attack, *chars[j]);
        if (!chars[j]->is_dead()) {
          chars[i]->place(prev_pos.x, prev_pos.y);
        }
      }
    }
  }

  // Cleaning up corpses...
  for (auto it = chars.begin(); it != chars.end();) {
    if ((*it)->is_dead()) {
      chars.erase(it);
    } else {
      ++it;
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
    map_.regenerate(map_generators::snake);
    hero->hp(hero_hp);
  }

  hero_prev_pos_ = hero->pos();
  calc_offsets();
}
void game::render() {
  using namespace graphics;

  auto &m = map_;
  auto &h = m.hero();
  auto &chars = m.characters();
  calc_offsets();

  auto y_start = offset_y_;
  auto y_end = offset_y_ + graphics::height() - 1;
  auto x_start = offset_x_;
  auto x_end = offset_x_ + graphics::width();

  for (map_size_t y = 0; y < graphics::height() - 1; ++y) {
    for (map_size_t x = 0; x < graphics::width(); ++x) {
      engine::draw_sym('.', x , y);
    }
  }
  for (auto &c : chars) {
    auto p = c->pos();
    if (p.x >= x_start && p.x < x_end) {
      if (p.y >= y_start && p.y < y_end) {
        engine::draw_sym(c->sym(), p.x - x_start, p.y - y_start);
      }
    }
  }

  auto h_pos = h->pos();
  engine::draw_sym(h->sym(), h_pos.x - x_start, h_pos.y - y_start);

  auto hp_str = fmt::format("HP: {}/{}", h->hp(), h->max_hp());
  engine::write_string(hp_str, 0, engine::height()-1);
#ifdef DEBUG
  auto pos_str = fmt::format("Pos: ({};{})", h_pos.x, h_pos.y);
  engine::write_string(pos_str, 80 - pos_str.size(), engine::height()-1);
#endif
}

void game::calc_offsets() {
  const auto field_w = graphics::width();
  const auto field_h = graphics::height() - 1;
  const auto center_rect_w = field_w / 3;
  const auto center_rect_h = field_h / 3;
  const auto map_w = map_.width();
  const auto map_h = map_.height();
  const auto center_rect_x = offset_x_ + (field_w - center_rect_w) / 2;
  const auto center_rect_y = offset_y_ + (field_h - center_rect_h) / 2;
  auto hero_pos = map_.hero()->pos();

  if (hero_pos.x < center_rect_x) {
    if (offset_x_ > 0) --offset_x_;
  }
  if (hero_pos.x >= center_rect_x + center_rect_w) {
    if (offset_x_ + field_w < map_w) ++offset_x_;
  }
  if (hero_pos.y < center_rect_y) {
    if (offset_y_ > 0) --offset_y_;
  }
  if (hero_pos.y >= center_rect_y + center_rect_h) {
    if (offset_y_ + field_h < map_h) ++offset_y_;
  }


  if (offset_x_ + field_w > map_w) {
    offset_x_ = map_w - field_w;
  }
  if (offset_x_ < 0) {
    offset_x_ = 0;
  }

  if (offset_y_ + field_h > map_h) {
    offset_y_ = map_h - field_h;
  }
  if (offset_y_ < 0) {
    offset_y_ = 0;
  }
}
}