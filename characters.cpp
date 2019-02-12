// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "utils/PRNG.h"
#include "characters.h"
#include "config.h"

namespace characters {
void Character::place(map_size_t x, map_size_t y) {
  pos_.x = x;
  pos_.y = y;
}

void Character::move(speed_t dx, speed_t dy) {
  place(pos().x + dx, pos().y + dy);
}

hp_t Character::hp() const {
  return hp_;
}
void Character::hp(hp_t amount) {
  hp_ = amount;
}
hp_t Character::max_hp() const {
  return max_hp_;
}
void Character::max_hp(hp_t amount) {
  max_hp_ = amount;
}
hp_t Character::damage() const {
  PRNG rnd;
  return rnd.get<hp_t>(0, damage_);
}
void Character::damage(hp_t amount) {
  damage_ = amount;
}
sym_t Character::sym() const {
  return sym_;
}
void Character::sym(sym_t symbol) {
  sym_ = symbol;
}
map_point Character::pos() const {
  return pos_;
}
bool Character::solid() const {
  return solid_;
}
void Character::solid(bool value) {
  solid_ = value;
}
bool Character::is_dead() const {
  return hp_ <= 0;
}
void Character::hurt(hp_t amount) {
  hp_ -= amount;
  if (hp_ > max_hp_) {
    hp_ = max_hp_;
  }
}

void init_from_config(Character &c, const std::string &char_name) {
  auto &conf = Config::get();
  auto character_conf = conf.characters().get(char_name); // TODO could be typeid().name(), it's not that slow
  c.sym(character_conf.sym);
  c.hp(character_conf.max_hp);
  c.max_hp(character_conf.max_hp);
  c.damage(character_conf.damage);
  c.solid(character_conf.solid);
}

void Monster::tick(map_point hero_pos) {
  if (hero_pos.x > pos_.x) {
    move(1, 0);
  } else if (hero_pos.x < pos_.x) {
    move(-1, 0);
  } else {
    if (hero_pos.y > pos_.y) {
      move(0, 1);
    } else if (hero_pos.y < pos_.y) {
      move(0, -1);
    }
  }
}

Knight::Knight(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Knight");
}
Knight::Knight(map_point pos) : Knight(pos.x, pos.y) {}

Princess::Princess(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Princess");
}
Princess::Princess(map_point pos) : Princess(pos.x, pos.y) {}

Zombie::Zombie(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Zombie");
}
Zombie::Zombie(map_point pos) : Zombie(pos.x, pos.y) {}

Dragon::Dragon(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Dragon");
}
Dragon::Dragon(map_point pos) : Dragon(pos.x, pos.y) {}

Wall::Wall(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Wall");
}
Wall::Wall(map_point pos) : Wall(pos.x, pos.y) {}

bool PickupItem::is_dead() const {
  return picked_up_;
}
void PickupItem::pick_up() {
  picked_up_ = true;
}

AidKit::AidKit(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "AidKit");
}
AidKit::AidKit(map_point pos) : AidKit(pos.x, pos.y) {}

namespace visitors {
void wall_visitor::visit(Character &a, Character &b) {
  if (collided_) return;
  a.accept(*this, b);
}
void wall_visitor::visit(Knight &a, Character &b) {
  if (collided_) return;
  b.accept(*this, a);
}
void wall_visitor::visit(Princess &a, Character &b) {
  if (collided_) return;
  b.accept(*this, a);
}
void wall_visitor::visit(Wall &a, Character &b) {
  collided_ = true;
}
void wall_visitor::visit(Zombie &a, Character &b) {
  if (collided_) return;
  b.accept(*this, a);
}
void wall_visitor::visit(Dragon &a, Character &b) {
  if (collided_) return;
  b.accept(*this, a);
}
void wall_visitor::visit(AidKit &a, Character &b) {
  if (collided_) return;
  b.accept(*this, a);
}
void wall_visitor::visit(Knight &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Princess &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Zombie &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Dragon &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Wall &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Knight &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Princess &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Zombie &b) {
  collided_ = true;
}
void wall_visitor::visit(Wall &a, Dragon &b) {
  collided_ = true;
}
bool wall_visitor::collided() const {
  return collided_;
}

void attack_visitor::visit(Character &a, Character &b) {
  if (done_) return;
  a.accept(*this, b);
}
void attack_visitor::visit(Wall &a, Character &b) {
  if (done_) return;
  a.hurt(b.damage());
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(Princess &a, Character &b) {
  if (done_) return;
  a.hurt(b.damage());
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(Zombie &a, Character &b) {
  if (done_) return;
  a.hurt(b.damage());
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(Dragon &a, Character &b) {
  if (done_) return;
  a.hurt(b.damage());
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(Knight &a, Character &b) {
  if (done_) return;
  a.hurt(b.damage());
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(AidKit &a, Character &b) {
  b.hurt(a.damage());
  a.pick_up();
  done_ = true;
  b.accept(*this, a);
}
void attack_visitor::visit(Knight &a, AidKit &b) {
  if (done_) return;
  a.hurt(b.damage());
  b.pick_up();
  done_ = true;
}
void attack_visitor::visit(Princess &a, AidKit &b) {
  if (done_) return;
  a.hurt(b.damage());
  b.pick_up();
  done_ = true;
}
void attack_visitor::visit(Zombie &a, AidKit &b) {
  if (done_) return;
  a.hurt(b.damage());
  b.pick_up();
  done_ = true;
}
void attack_visitor::visit(Dragon &a, AidKit &b) {
  if (done_) return;
  a.hurt(b.damage());
  b.pick_up();
  done_ = true;
}


void win_cond_visitor::visit(Character &a, Character &b) {
  if (won_) return;
  a.accept(*this, b);
}
void win_cond_visitor::visit(Knight &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Princess &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Wall &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Zombie &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Dragon &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(AidKit &a, Character &b) {
  if (won_) return;
  b.accept(*this, a);
}
void win_cond_visitor::visit(Knight &a, Princess &b) {
  if (!a.is_dead()) won_ = true;
}
void win_cond_visitor::visit(Princess &a, Knight &b) {
  if (!a.is_dead()) won_ = true;
}

void base_visitor::visit(Knight&, Knight&){}
void base_visitor::visit(Princess&, Knight&){}
void base_visitor::visit(Wall&, Knight&){}
void base_visitor::visit(Zombie&, Knight&){}
void base_visitor::visit(Dragon&, Knight&){}
void base_visitor::visit(AidKit&, Knight&){}
void base_visitor::visit(Knight&, Princess&){}
void base_visitor::visit(Princess&, Princess&){}
void base_visitor::visit(Wall&, Princess&){}
void base_visitor::visit(Zombie&, Princess&){}
void base_visitor::visit(Dragon&, Princess&){}
void base_visitor::visit(AidKit&, Princess&){}
void base_visitor::visit(Knight&, Wall&){}
void base_visitor::visit(Princess&, Wall&){}
void base_visitor::visit(Wall&, Wall&){}
void base_visitor::visit(Zombie&, Wall&){}
void base_visitor::visit(Dragon&, Wall&){}
void base_visitor::visit(AidKit&, Wall&){}
void base_visitor::visit(Knight&, Zombie&){}
void base_visitor::visit(Princess&, Zombie&){}
void base_visitor::visit(Wall&, Zombie&){}
void base_visitor::visit(Zombie&, Zombie&){}
void base_visitor::visit(Dragon&, Zombie&){}
void base_visitor::visit(AidKit&, Zombie&){}
void base_visitor::visit(Knight&, Dragon&){}
void base_visitor::visit(Princess&, Dragon&){}
void base_visitor::visit(Wall&, Dragon&){}
void base_visitor::visit(Zombie&, Dragon&){}
void base_visitor::visit(Dragon&, Dragon&){}
void base_visitor::visit(AidKit&, Dragon&){}
void base_visitor::visit(Knight&, AidKit&){}
void base_visitor::visit(Princess&, AidKit&){}
void base_visitor::visit(Wall&, AidKit&){}
void base_visitor::visit(Zombie&, AidKit&){}
void base_visitor::visit(Dragon&, AidKit&){}
void base_visitor::visit(AidKit&, AidKit&){}
}
}