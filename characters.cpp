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
  if (hp_ > max_hp_) {
    hp_ = max_hp_;
  }
}
hp_t Character::max_hp() const {
  return max_hp_;
}
void Character::max_hp(hp_t amount) {
  max_hp_ = amount;
  if (hp_ > max_hp_) {
    hp_ = max_hp_;
  }
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
bool Character::is_dead() const {
  return hp_ <= 0;
}
void Character::hurt(hp_t amount) {
  hp_ -= amount;
  if (hp_ > max_hp_) {
    hp_ = max_hp_;
  }
}
std::vector<std::shared_ptr<Character>> Character::projectiles() {
  auto ret = spawned_projectiles_;
  spawned_projectiles_.clear();
  return ret;
}
bool Character::is_projectile() const {
  return false;
}

void init_from_config(Character &c, const std::string &char_name) {
  auto &conf = Config::get();
  auto character_conf = conf.characters().get(char_name); // TODO could be typeid().name(), it's not that slow
  c.sym(character_conf.sym);
  c.max_hp(character_conf.max_hp);
  c.hp(character_conf.max_hp);
  c.damage(character_conf.damage);
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
void Knight::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Knight::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}
void Knight::tick(map_point) {}

Princess::Princess(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Princess");
}
Princess::Princess(map_point pos) : Princess(pos.x, pos.y) {}
void Princess::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Princess::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}
bool Princess::is_dead() const {
  return false;
}
void Princess::hurt(hp_t) {}
void Princess::tick(map_point) {}

Zombie::Zombie(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Zombie");
}
Zombie::Zombie(map_point pos) : Zombie(pos.x, pos.y) {}
void Zombie::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Zombie::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}

Dragon::Dragon(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Dragon");
}
Dragon::Dragon(map_point pos) : Dragon(pos.x, pos.y) {}
void Dragon::tick(map_point hero_pos) {
  PRNG gen;
  auto fireball_chance = gen.get(1, 10) == 1;
  if (fireball_chance) {
    if (hero_pos.x == pos_.x) {
      if (hero_pos.y > pos_.y) {
        auto fireball = std::make_shared<Fireball>(pos_.x, pos_.y + 1, 0, 1);
        spawned_projectiles_.push_back(fireball);
        return;
      } else if (hero_pos.y < pos_.y) {
        auto fireball = std::make_shared<Fireball>(pos_.x, pos_.y - 1, 0, -1);
        spawned_projectiles_.push_back(fireball);
        return;
      }
    } else if (hero_pos.y == pos_.y) {
      if (hero_pos.x > pos_.x) {
        auto fireball = std::make_shared<Fireball>(pos_.x + 1, pos_.y, 1, 0);
        spawned_projectiles_.push_back(fireball);
        return;
      } else if (hero_pos.x < pos_.x) {
        auto fireball = std::make_shared<Fireball>(pos_.x - 1, pos_.y, -1, 0);
        spawned_projectiles_.push_back(fireball);
        return;
      }
    }
  }
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
void Dragon::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Dragon::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}

Wall::Wall(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Wall");
}
Wall::Wall(map_point pos) : Wall(pos.x, pos.y) {}
void Wall::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Wall::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}
bool Wall::is_dead() const {
  return false;
}
void Wall::hurt(hp_t) {}
void Wall::tick(map_point) {}

bool PickupItem::is_dead() const {
  return picked_up_;
}
void PickupItem::pick_up() {
  picked_up_ = true;
}
void PickupItem::hurt(hp_t) {}

AidKit::AidKit(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "AidKit");
}
AidKit::AidKit(map_point pos) : AidKit(pos.x, pos.y) {}
hp_t AidKit::damage() const {
  return damage_;
}
void AidKit::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void AidKit::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}
void AidKit::tick(map_point) {}

void Projectile::speed(map_size_t dx, map_size_t dy) {
  speed_x_ = dx;
  speed_y_ = dy;
}
void Projectile::tick(map_point) {
  pos_.x += speed_x_;
  pos_.y += speed_y_;
}
bool Projectile::is_projectile() const {
  return true;
}

Fireball::Fireball(map_size_t x, map_size_t y) {
  pos_ = {x, y};
  init_from_config(*this, "Fireball");
}
Fireball::Fireball(map_size_t x, map_size_t y, map_size_t dx, map_size_t dy)
  : Fireball(x, y) {
  speed(dx, dy);
}
Fireball::Fireball(map_point pos) : Fireball(pos.x, pos.y) {}
void Fireball::accept(visitors::base_visitor &v, Character &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Knight &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Princess &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Wall &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Zombie &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Dragon &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, AidKit &with) {
  v.visit(*this, with);
}
void Fireball::accept(visitors::base_visitor &v, Fireball &with) {
  v.visit(*this, with);
}

namespace visitors {
void base_visitor::visit(Knight&, Knight&){}
void base_visitor::visit(Princess&, Knight&){}
void base_visitor::visit(Wall&, Knight&){}
void base_visitor::visit(Zombie&, Knight&){}
void base_visitor::visit(Dragon&, Knight&){}
void base_visitor::visit(AidKit&, Knight&){}
void base_visitor::visit(Fireball&, Knight&){}
void base_visitor::visit(Knight&, Princess&){}
void base_visitor::visit(Princess&, Princess&){}
void base_visitor::visit(Wall&, Princess&){}
void base_visitor::visit(Zombie&, Princess&){}
void base_visitor::visit(Dragon&, Princess&){}
void base_visitor::visit(AidKit&, Princess&){}
void base_visitor::visit(Fireball&, Princess&){}
void base_visitor::visit(Knight&, Wall&){}
void base_visitor::visit(Princess&, Wall&){}
void base_visitor::visit(Wall&, Wall&){}
void base_visitor::visit(Zombie&, Wall&){}
void base_visitor::visit(Dragon&, Wall&){}
void base_visitor::visit(AidKit&, Wall&){}
void base_visitor::visit(Fireball&, Wall&){}
void base_visitor::visit(Knight&, Zombie&){}
void base_visitor::visit(Princess&, Zombie&){}
void base_visitor::visit(Wall&, Zombie&){}
void base_visitor::visit(Zombie&, Zombie&){}
void base_visitor::visit(Dragon&, Zombie&){}
void base_visitor::visit(AidKit&, Zombie&){}
void base_visitor::visit(Fireball&, Zombie&){}
void base_visitor::visit(Knight&, Dragon&){}
void base_visitor::visit(Princess&, Dragon&){}
void base_visitor::visit(Wall&, Dragon&){}
void base_visitor::visit(Zombie&, Dragon&){}
void base_visitor::visit(Dragon&, Dragon&){}
void base_visitor::visit(AidKit&, Dragon&){}
void base_visitor::visit(Fireball&, Dragon&){}
void base_visitor::visit(Knight&, AidKit&){}
void base_visitor::visit(Princess&, AidKit&){}
void base_visitor::visit(Wall&, AidKit&){}
void base_visitor::visit(Zombie&, AidKit&){}
void base_visitor::visit(Dragon&, AidKit&){}
void base_visitor::visit(AidKit&, AidKit&){}
void base_visitor::visit(Fireball&, AidKit&){}
void base_visitor::visit(Knight&, Fireball&){}
void base_visitor::visit(Princess&, Fireball&){}
void base_visitor::visit(Wall&, Fireball&){}
void base_visitor::visit(Zombie&, Fireball&){}
void base_visitor::visit(Dragon&, Fireball&){}
void base_visitor::visit(AidKit&, Fireball&){}
void base_visitor::visit(Fireball&, Fireball&){}
}
}