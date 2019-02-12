#pragma once
#include <string>
#include "characters/types.h"
#include "map/types.h"

#define __char_invincible \
bool is_dead() const override { \
  return false; \
}\
void hurt(hp_t) override {}


namespace characters {
class Character;
class Monster;
class PickupItem;
class Knight;
class Princess;
class Wall;
class Zombie;
class Dragon;
class AidKit;

namespace visitors {
class base_visitor {
 public:
  virtual void visit(Knight&, Knight&);
  virtual void visit(Princess&, Knight&);
  virtual void visit(Wall&, Knight&);
  virtual void visit(Zombie&, Knight&);
  virtual void visit(Dragon&, Knight&);
  virtual void visit(AidKit&, Knight&);

  virtual void visit(Knight&, Princess&);
  virtual void visit(Princess&, Princess&);
  virtual void visit(Wall&, Princess&);
  virtual void visit(Zombie&, Princess&);
  virtual void visit(Dragon&, Princess&);
  virtual void visit(AidKit&, Princess&);

  virtual void visit(Knight&, Wall&);
  virtual void visit(Princess&, Wall&);
  virtual void visit(Wall&, Wall&);
  virtual void visit(Zombie&, Wall&);
  virtual void visit(Dragon&, Wall&);
  virtual void visit(AidKit&, Wall&);

  virtual void visit(Knight&, Zombie&);
  virtual void visit(Princess&, Zombie&);
  virtual void visit(Wall&, Zombie&);
  virtual void visit(Zombie&, Zombie&);
  virtual void visit(Dragon&, Zombie&);
  virtual void visit(AidKit&, Zombie&);

  virtual void visit(Knight&, Dragon&);
  virtual void visit(Princess&, Dragon&);
  virtual void visit(Wall&, Dragon&);
  virtual void visit(Zombie&, Dragon&);
  virtual void visit(Dragon&, Dragon&);
  virtual void visit(AidKit&, Dragon&);

  virtual void visit(Knight&, AidKit&);
  virtual void visit(Princess&, AidKit&);
  virtual void visit(Wall&, AidKit&);
  virtual void visit(Zombie&, AidKit&);
  virtual void visit(Dragon&, AidKit&);
  virtual void visit(AidKit&, AidKit&);

  virtual void visit(Character&, Character&) = 0;
  virtual void visit(Knight&, Character&) = 0;
  virtual void visit(Princess&, Character&) = 0;
  virtual void visit(Wall&, Character&) = 0;
  virtual void visit(Zombie&, Character&) = 0;
  virtual void visit(Dragon&, Character&) = 0;
  virtual void visit(AidKit&, Character&) = 0;
  
};
class wall_visitor : public base_visitor {
 public:
  virtual bool collided() const;
  void visit(Character &a, Character &b) override;
  void visit(Knight &a, Character &b) override;
  void visit(Princess &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;
  void visit(Knight &a, Wall &b) override;
  void visit(Princess &a, Wall &b) override;
  void visit(Zombie &a, Wall &b) override;
  void visit(Dragon &a, Wall &b) override;
  void visit(Wall &a, Wall &b) override;
  void visit(Wall &a, Knight &b) override;
  void visit(Wall &a, Princess &b) override;
  void visit(Wall &a, Zombie &b) override;
  void visit(Wall &a, Dragon &b) override;
 private:
  bool collided_ = false;
};
class attack_visitor : public base_visitor {
 public:
  void visit(Character &a, Character &b) override;
  void visit(Knight &a, Character &b) override;
  void visit(Princess &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;

  void visit(Knight &a, AidKit &b) override;
  void visit(Princess &a, AidKit &b) override;
  void visit(Zombie &a, AidKit &b) override;
  void visit(Dragon &a, AidKit &b) override;
  void visit(AidKit &a, Knight &b) override;
  void visit(AidKit &a, Princess &b) override;
  void visit(AidKit &a, Zombie &b) override;
  void visit(AidKit &a, Dragon &b) override;
 private:
  bool done_ = false;
};
class win_cond_visitor : public base_visitor {
 public:
  bool won() {
    return won_;
  }
  void visit(Character &a, Character &b) override;
  void visit(Knight &a, Character &b) override;
  void visit(Wall&, Character&) override;
  void visit(Zombie&, Character&) override;
  void visit(Dragon&, Character&) override;
  void visit(AidKit&, Character&) override;

  void visit(Princess &a, Character &b) override;
  void visit(Knight &a, Princess &b) override;
  void visit(Princess &a, Knight &b) override;
 private:
  bool won_ = false;
};
}

class Character {
 public:
  hp_t hp() const;
  void hp(hp_t amount);
  hp_t max_hp() const;
  void max_hp(hp_t amount);
  virtual hp_t damage() const;
  void damage(hp_t amount);
  sym_t sym() const;
  void sym(sym_t symbol);
  map_point pos() const;
  bool solid() const;
  void solid(bool value);
  virtual bool is_dead() const;

  virtual void accept(visitors::base_visitor &v, Character &with) = 0;
  virtual void accept(visitors::base_visitor &v, Knight &with) = 0;
  virtual void accept(visitors::base_visitor &v, Princess &with) = 0;
  virtual void accept(visitors::base_visitor &v, Wall &with) = 0;
  virtual void accept(visitors::base_visitor &v, Zombie &with) = 0;
  virtual void accept(visitors::base_visitor &v, Dragon &with) = 0;
  virtual void accept(visitors::base_visitor &v, AidKit &with) = 0;

  virtual void tick(map_point hero_pos) = 0;
  virtual void move(speed_t dx, speed_t dy);
  virtual void hurt(hp_t amount);
  void place(map_size_t x, map_size_t y);
 protected:
  hp_t hp_;
  hp_t max_hp_;
  hp_t damage_;
  sym_t sym_;
  bool solid_;
  map_point pos_;
};

class Knight : public Character {
 public:
  Knight(map_size_t x, map_size_t y);
  explicit Knight(map_point pos);
  void tick(map_point) override {};
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};

class Princess : public Character {
 public:
  Princess(map_size_t x, map_size_t y);
  explicit Princess(map_point pos);
  __char_invincible;
  void tick(map_point) override {};
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};

class Wall : public Character {
 public:
  Wall(map_size_t x, map_size_t y);
  explicit Wall(map_point pos);
  __char_invincible;
  void tick(map_point) override {};
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};

class Monster : public Character {
 public:
  void tick(map_point hero_pos) override;
};

class Zombie : public Monster {
 public:
  Zombie(map_size_t x, map_size_t y);
  explicit Zombie(map_point pos);
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};

class Dragon : public Monster {
 public:
  Dragon(map_size_t x, map_size_t y);
  explicit Dragon(map_point pos);
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};

class PickupItem : public Character {
 public:
  bool is_dead() const override;
  void hurt(hp_t) override {}
  virtual void pick_up();
 protected:
  bool picked_up_ = false;
};

class AidKit : public PickupItem {
 public:
  AidKit(map_size_t x, map_size_t y);
  explicit AidKit(map_point pos);
  void tick(map_point) override {};
  void accept(visitors::base_visitor &v, Character &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Knight &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Princess &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Wall &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Zombie &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, Dragon &with) override {
    v.visit(*this, with);
  }
  void accept(visitors::base_visitor &v, AidKit &with) override {
    v.visit(*this, with);
  }
};
}