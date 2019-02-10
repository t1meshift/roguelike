// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "game.h"
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
  std::vector<std::pair<map_point, bool>> prev_pos;
  bool hero_collided = false;
  bool level_won = false;
  auto &chars = map_.characters();
  auto &hero = map_.hero();

  for (auto &character : chars) {
    // TODO field of view of characters for characters
    prev_pos.emplace_back(character->pos(), false);
    character->tick(hero->pos());
  }
  // Collide player first...
  for (int i = 0; i < chars.size(); ++i) {
    if (hero->pos() == chars[i]->pos()) {
      namespace char_vis = characters::visitors;
      char_vis::attack_visitor attack;
      char_vis::win_cond_visitor win_cond;
      hero->accept(attack, *chars[i]);
      hero->accept(win_cond, *chars[i]);
      level_won = win_cond.won();
      hero_collided = true;
      prev_pos[i].second = true;
    }
  }
  // ...then collide other chars
  for (int i = 0; i < chars.size(); ++i) {
    for (int j = i + 1; j < chars.size(); ++j) {
      if (chars[i]->pos() == chars[j]->pos()) {
        namespace char_vis = characters::visitors;
        char_vis::attack_visitor attack;
        chars[i]->accept(attack, *chars[j]);
        prev_pos[i].second = true;
        prev_pos[j].second = true;
      }
    }
  }
  // Check chars if they are collided or even dead...
  for (int i = 0; i < chars.size(); ++i) {
    if (prev_pos[i].second) {
      chars[i]->place(prev_pos[i].first.x, prev_pos[i].first.y);
      if (chars[i]->is_dead()) {
        chars[i].reset();
        chars[i] = nullptr;
      }
    }
  }
  // ...then hero
  if (hero_collided) {
    hero->place(hero_prev_pos_.x, hero_prev_pos_.y);
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
  engine::draw_sym(h->sym(), h->pos().x, h->pos().y);
  engine::write_string("HP: " + std::to_string(h->hp()), 0, 23); // TODO get window height
}
}