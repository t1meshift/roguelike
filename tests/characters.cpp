// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include "../utils/PRNG.h"
#include "../characters.h"
#include "../config.h"

namespace chr = characters;
namespace vis = chr::visitors;

TEMPLATE_TEST_CASE("Character creation", "[character]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball,
    chr::Wall) {
  Config::init();
  TestType c(map_point{1, 1});
  REQUIRE(!c.is_dead());
  REQUIRE(c.hp() == c.max_hp());
  REQUIRE(c.pos() == map_point{1, 1});
  REQUIRE(c.projectiles().empty());
  REQUIRE_NOTHROW(c.tick(map_point{0, 0}));
  REQUIRE_NOTHROW(c.is_projectile());
}

TEMPLATE_TEST_CASE("Character basic parameters", "[character]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball,
    chr::Wall) {
  Config::init();
  TestType c(map_point{0, 0});
  SECTION("HP does not exceed its maximum") {
    c.max_hp(1);
    c.hp(2);
    REQUIRE(c.hp() == 1);

    c.max_hp(10);
    c.hp(10);
    c.max_hp(8);
    REQUIRE(c.hp() == 8);
  }
  SECTION("Movement works") {
    c.place(2, 2);
    REQUIRE(c.pos() == map_point{2, 2});

    c.move(1, -1);
    REQUIRE(c.pos() == map_point{3, 1});
  }
  SECTION("Symbol change works") {
    c.sym('A');
    REQUIRE(c.sym() == 'A');
  }
  SECTION("Char can't be healed over its max HP") {
    c.max_hp(10);
    c.hp(9);
    c.hurt(-11);
    REQUIRE(!c.is_dead());
    REQUIRE(c.hp() <= 10);
  }
}

TEMPLATE_TEST_CASE("Mortal characters' parameters", "[characters]",
    chr::Knight, chr::Zombie, chr::Dragon) {
  TestType c(0, 0);
  SECTION("Damage over char's HP kills it") {
    c.hp(10);
    c.hurt(11);
    REQUIRE(c.is_dead());
  }
}

TEMPLATE_TEST_CASE("Pickup items' parameters", "[character][pickup]",
    chr::AidKit) {
  TestType c(map_point{0, 0});
  SECTION("Picking up kills item") {
    c.pick_up();
    REQUIRE(c.is_dead());
  }
}

TEMPLATE_TEST_CASE("Invincible characters' parameters", "[character][invincible]",
    chr::Princess, chr::Wall) {
  TestType c(map_point{0, 0});
  SECTION("Simple damage doesn't kill character") {
    c.hp(10);
    c.hurt(11);
    REQUIRE(!c.is_dead());
  }
}

TEST_CASE("AidKit specific test cases", "[character][pickup][aidkit]") {
  chr::AidKit aids(map_point{0, 0});
  SECTION("AidKit gives exact damage as was set") {
    hp_t dmg = -10;
    aids.damage(dmg);
    // Catch2's generators are kind of black magic
    // Googling "undefined reference to `vtable for Catch::Generators::GeneratorUntypedBase'"
    // gives no real results
    for (int i = 0; i < 5; ++i) {
      REQUIRE(aids.damage() == dmg);
    }
  }
}

TEMPLATE_TEST_CASE("Mobs' damage test", "[character]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon) {
  TestType c(map_point{0, 0});
  SECTION("The damage is never lower its minimum") {
    c.damage(1);
    for (int i = 0; i < 5; ++i) {
      REQUIRE(c.damage() >= 0);
    }
    c.damage(-1);
    for (int i = 0; i < 5; ++i) {
      REQUIRE(c.damage() >= -1);
    }
  }
  SECTION("The damage does not exceed its maximum") {
    c.damage(3);
    for (int i = 0; i < 5; ++i) {
      REQUIRE(c.damage() <= 3);
    }
    c.damage(-3);
    for (int i = 0; i < 5; ++i) {
      REQUIRE(c.damage() <= 0);
    }
  }
}

TEST_CASE("Zombie AI test", "[character][zombie][ai]") {
  chr::Zombie c(map_point{0, 0});
  PRNG rnd;
  SECTION("Zombie moves to hero's position") {
    for (int i = 0; i < 20; ++i) {
      auto hx = rnd.get<map_size_t>(-1, 1);
      auto hy = rnd.get<map_size_t>(-1, 1);
      int dist = std::abs(hx) + std::abs(hy);
      if (dist != 0) {
        c.tick(map_point{hx, hy});
        int new_dist = std::abs(c.pos().x - hx) + std::abs(c.pos().y - hy);
        REQUIRE(new_dist < dist);
      }
      c.place(0, 0);
    }
  }
  SECTION("Zombie stays still if he is on hero's position") {
    auto hx = rnd.get<map_size_t>(-10, 10);
    auto hy = rnd.get<map_size_t>(-10, 10);
    c.place(hx, hy);
    c.tick(map_point{hx, hy});
    REQUIRE(c.pos() == map_point{hx, hy});
  }
}

TEST_CASE("Dragon AI test", "[character][dragon][ai]") {
  chr::Dragon c(map_point{0, 0});
  PRNG rnd;
  SECTION("Dragon moves to hero's position or throws a fireball in his direction") {
    for (int i = 0; i < 100; ++i) {
      auto hx = rnd.get<map_size_t>(-2, 2);
      auto hy = rnd.get<map_size_t>(-2, 2);
      int dist = std::abs(hx) + std::abs(hy);
      if (dist != 0) {
        c.tick(map_point{hx, hy});
        int new_dist = std::abs(c.pos().x - hx) + std::abs(c.pos().y - hy);
        bool flag = new_dist < dist;
        if (new_dist == dist) {
          auto projs = c.projectiles();
          int proj_dist = 1000;
          if (!projs.empty()) {
            proj_dist = std::abs(projs[0]->pos().x - hx) + std::abs(projs[0]->pos().y - hy);
          }
          flag |= projs.size() == 1 && proj_dist < new_dist;
        }
        REQUIRE(flag);
      }
      c.place(0, 0);
    }
  }
}

TEMPLATE_TEST_CASE("Projectiles' test", "[character][projectile]",
    chr::Fireball) {
  SECTION("Classes given above are projectiles") {
    TestType c(0, 0);
    REQUIRE(c.is_projectile());
  }
  SECTION("Constucting with speed params make it move") {
    TestType c(0, 0, 1, 1);
    c.tick(map_point{0, 0});
    REQUIRE(c.pos() == map_point{1, 1});
  }
  SECTION("Speed setter works") {
    TestType c(0, 0, 1, 1);
    c.speed(-1, -1);
    c.tick(map_point{0, 0});
    REQUIRE(c.pos() == map_point{-1, -1});
  }
}

TEMPLATE_TEST_CASE("Basic visitors test (no dispatching)", "[character][visitor]",
    chr::Knight, chr::Princess, chr::Wall, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball) {
  TestType a(0, 0);
  // I wish generators worked T_T
  SECTION("Collide with Knight") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Knight b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with Princess") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Princess b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with Wall") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Wall b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with Zombie") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Zombie b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with Dragon") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Dragon b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with AidKit"){
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::AidKit b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
  SECTION("Collide with Fireball") {
    vis::attack_visitor attack;
    vis::wall_visitor wall;
    vis::win_cond_visitor win_cond;
    chr::Fireball b(0, 0);
    attack.visit(a, b);
    wall.visit(a, b);
    win_cond.visit(a, b);
  }
}

TEMPLATE_TEST_CASE("Attack visitor handles AidKit",
    "[character][visitor][pickup][aidkit]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon) {
  SECTION("To AidKit") {
    vis::attack_visitor att;
    TestType a(0, 0);
    chr::AidKit b(0, 0);
    a.accept(att, b);
    REQUIRE(b.is_dead());
  }
  SECTION("From AidKit") {
    vis::attack_visitor att;
    chr::AidKit a(0, 0);
    TestType b(0, 0);
    a.accept(att, b);
    REQUIRE(a.is_dead());
  }
}

// TODO fireball tests, they'd be same as AidKit test above

TEMPLATE_TEST_CASE("Wall visitor handles Wall", "[character][visitor][wall]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::Fireball, chr::Wall) {
  SECTION("To Wall") {
    vis::wall_visitor v;
    TestType a(0, 0);
    chr::Wall b(0, 0);
    a.accept(v, b);
    REQUIRE(v.collided());
  }
  SECTION("From Wall") {
    vis::wall_visitor v;
    chr::Wall a(0, 0);
    TestType b(0, 0);
    a.accept(v, b);
    REQUIRE(v.collided());
  }
}

TEST_CASE("Win condition visitor works when Knight collides Princess", "[character][visitor]") {
  chr::Knight knight(0, 0);
  chr::Princess princess(0, 0);
  SECTION("Knight -> Princess") {
    vis::win_cond_visitor v;
    knight.accept(v, princess);
    REQUIRE(v.won());
  }
  SECTION("Princess -> Knight") {
    vis::win_cond_visitor v;
    princess.accept(v, knight);
    REQUIRE(v.won());
  }
}

TEMPLATE_TEST_CASE("Double dispatching test", "[character][visitor]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball,
    chr::Wall) {
  using namespace chr;
 class test_visitor : public vis::base_visitor {
  public:
   bool visited() const {
     return visited_;
   }
   void visit(Knight &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Princess &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Zombie &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Dragon &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(AidKit &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Fireball &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Wall &a, Character &b) override {
     b.accept(*this, a);
   }
   void visit(Knight &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Princess &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Zombie &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Dragon &a, Knight &b) override {
     visited_ = true;
   }
   void visit(AidKit &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Fireball &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Wall &a, Knight &b) override {
     visited_ = true;
   }
   void visit(Knight &a, Princess &b) override {
     visited_ = true;
   }
   void visit(Knight &a, Zombie &b) override {
     visited_ = true;
   }
   void visit(Knight &a, Dragon &b) override {
     visited_ = true;
   }
   void visit(Knight &a, AidKit &b) override {
     visited_ = true;
   }
   void visit(Knight &a, Fireball &b) override {
     visited_ = true;
   }
   void visit(Knight &a, Wall &b) override {
     visited_ = true;
   }
  private:
   bool visited_ = false;
 };
  std::shared_ptr<chr::Character> a = std::make_shared<TestType>(0, 0);
  std::shared_ptr<chr::Character> b = std::make_shared<chr::Knight>(0, 0);
  SECTION("To Knight") {
    test_visitor v;
    a->accept(v, *b);
    REQUIRE(v.visited());
  }
  SECTION("From Knight") {
    test_visitor v;
    b->accept(v, *a);
    REQUIRE(v.visited());
  }
}